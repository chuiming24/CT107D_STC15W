#include "common.h"
#include "ds1302.h"
//#include "ds18b20.h"
#include "iic.h"

// 数码管段码表
// 0 1 2 3 4 5 6 7 8 9 - (blank out)
code uchar nixie_tab[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xbf, 0xff};
// 数码管显示缓冲区
uchar nixie_buf[8] = {9,8,NIXIE_HYPHEN,3,9,NIXIE_BLANK,5,0};
uchar nixie_buf_bak[8] = {0};


// 旗标变量
//bit nixie_show_flag = 0;
bit keyboard_scan_flag = 0;
bit ds1302_gettime_flag = 0;
bit pcf8591_adc_flag = 0;
bit auto_manual_toggle = 0;  // 0: auto, 1: manual
bit buzzer_status = 1;  // 0: off, 1: on
bit alarm_watchdog_flag = 0;

// 保存读出来的时间
uchar hour, minute, second;

// rom内容
uchar rom_content = 0x00;

// 湿度阈值
uchar humidity_threshould = 50;

// U9锁存器的值
uchar misc_val = 0xff;

// ******************** 函数声明 ********************
void startup();
void nixie_show_all();
uchar key_scan_step_1();
uchar keyboard_return_code();
void humidity_thresh_adjust();
void nixie_buf_backup();
void nixie_buf_time();
//void nixie_buf_temp(uchar temp);
void nixie_buf_adc(uchar dat);
void nixie_buf_humidity_adjust();
void key_handler(key_code);
void buzzer_toggle();
void relay_on();
void relay_off();
void auto_manual_led();
void alarm_watchdog(uchar current_humidity);
void debug_nixie_buf_rom();
void debug_rom_view();



// ******************** main ********************

void main()
{
	// 按键值
	uchar key_code = 0xff;
	uchar nAdc;
	
	startup();

	while(1)
	{
		if (keyboard_scan_flag)
		{
			keyboard_scan_flag = 0;
			key_code = keyboard_return_code();
			key_handler(key_code);
		}
		if (ds1302_gettime_flag)
		{
			ds1302_gettime_flag = 0;
			nixie_buf_time();
		}
		if (pcf8591_adc_flag)
		{
			pcf8591_adc_flag = 0;
			delay(50);
			nAdc = pcf8591_adc();
			nAdc = get_himidity(nAdc);
			nixie_buf_adc(nAdc);
		}
		if (alarm_watchdog_flag)
		{
			alarm_watchdog_flag = 0;
			alarm_watchdog(nAdc);
		}

	}
}


// ******************** 基本函数 ********************
void startup()
{
	TMOD = 0x11;
	TH0 = TH0_RLD;
	TL0 = TL0_RLD;
	TH1 = TH1_RLD;
	TL1 = TL1_RLD;

	ET0 = 1;
	ET1 = 1;

	TR0 = 1;
	TR1 = 1;

	EA = 1;

	set_led(0xff);
	set_lock();
	
	auto_manual_led();
	
	// buzzer init (off)
	misc_val = (misc_val & 0xbf);
	set_misc(misc_val);
	set_lock();
			
		
	// relay init (off)
	misc_val = (misc_val | 0x10);
	set_misc(misc_val);
	set_lock();
	
	ds1302_reset();
	delay(50);
	pcf8591_init();
	delay(50);
}


void timer0_isr() interrupt 1
{
	TR0 = 0;
	TH0 = TH0_RLD;
	TL0 = TL0_RLD;

	keyboard_scan_flag = 1;

	TR0 = 1;
}


void timer1_isr() interrupt 3
{
	static uint ds1302_gettime_rate = 0;
	static uint pcf8591_adc_rate = 0;
	static uint alarm_watchdog_rate = 0;
	TR1 = 0;
	TH1 = TH1_RLD;
	TL1 = TL1_RLD;

	nixie_show_all();
	
	if (++ds1302_gettime_rate == 250)
	{
		ds1302_gettime_rate = 0;
		ds1302_gettime_flag = 1;
	}

	if (++pcf8591_adc_rate == 200)
	{
		pcf8591_adc_flag = 1;
		pcf8591_adc_rate = 0;
	}
	if (++alarm_watchdog_rate == 150)
	{
		alarm_watchdog_flag = 1;
		alarm_watchdog_rate = 0;
	}

	TR1 = 1;
}


// ******************** 延时 ********************
void delay(uint ms)
{
	uchar i;
	#define DELAYING 200
	while (ms)
	{
		for (i = 0 ; i < DELAYING ; i++);
		--ms;
	}
}


// ******************** 键盘扫描返回键值(Not for user call!) ********************
uchar key_scan_step_1()
{
	uchar tmp, x_temp = 0x0f, y_temp = 0xf0;
	P3 = 0x0f;
	P42 = 0;
	P44 = 0;

	x_temp = P3 & 0x0f;
	if (x_temp == 0x0f)
		return 0xff;
	else
	{
		P3 = 0xf0;
		P42 = P44 = 1;
		y_temp = (P3 & (P42 == 0 ? 0xbf : 0xff) & (P44 == 0 ? 0x7f : 0xff) )& 0xf0;
		if (y_temp == 0xf0)
			return 0xff;

		tmp = x_temp | y_temp;
		return tmp;
	}
}

// ******************** 键盘扫描返回键值(step 2) ********************
uchar keyboard_return_code()
{
	uchar tmp0, tmp1 ,kcode;

	// 消抖
	tmp0 = key_scan_step_1();
	tmp1 = key_scan_step_1();
	if (tmp0 != tmp1)
		kcode = 0xff;
	else
	{
		tmp0 = key_scan_step_1();
		if (tmp0 != tmp1)
		kcode = 0xff;
	}
	kcode = tmp0;

	// 等待弹起
	if (kcode != 0xff)
	{
		do
		{
			P3 = 0x0f;
			P42 = 0;
			P44 = 0;
			tmp0 = P3 & 0x0f;
		} while (tmp0 != 0x0f);
	}
	else
		kcode = 0xff;
	return kcode;
}

// ******************** 数码管显示 ********************
void nixie_show_all()
{
	static uchar com_ptr = 0;
	set_nixie_dat(0xff);
	set_nixie_com(1 << com_ptr);
	set_nixie_dat( nixie_tab[ nixie_buf[com_ptr] ]);
	if (++com_ptr == 8)
		com_ptr = 0;
}

// ******************** 保存数码管缓冲区当前内容 ********************
void nixie_buf_backup()
{
	uchar i;
	for (i = 0; i < 8 ; i++)
	{
		nixie_buf_bak[i] = nixie_buf[i];
	}
}

// ******************** 从存档中读取数码管缓冲区的内容 ********************
void nixie_buf_restore()
{
	uchar i;
	for (i = 0; i < 8 ; i++)
	{
		nixie_buf[i] = nixie_buf_bak[i];
	}
}

void nixie_buf_humidity_adjust()
{
	uchar tmp;
	tmp = humidity_threshould;
	nixie_buf[0] = nixie_buf[1] = NIXIE_HYPHEN;
	nixie_buf[2] = nixie_buf[3] = nixie_buf[4] = nixie_buf[5] = NIXIE_BLANK;
	nixie_buf[7] = tmp % 10;
	tmp /= 10;
	nixie_buf[6] = tmp % 10;
}


// ******************** 显示时间 ********************

void nixie_buf_time()
{
	uchar tmp;
	ds1302_gettime(&second, &minute, &hour);
	tmp = hour;
	nixie_buf[1] = tmp % 10;
	tmp /= 10;
	nixie_buf[0] = tmp % 10;
	tmp = minute;
	nixie_buf[4] = tmp % 10;
	tmp /= 10;
	nixie_buf[3] = tmp % 10;
}

// ******************** 显示温度 ********************
//void nixie_buf_temp(uchar nTemp)
//{
//	uchar tmp;
//	tmp = nTemp;
//	nixie_buf[7] = tmp % 10;
//	tmp /= 10;
//	nixie_buf[6] = tmp % 10;
//}

// ******************** 显示ADC ********************
void nixie_buf_adc(uchar dat)
{
	uchar tmp;
	tmp = dat;
	nixie_buf[7] = tmp % 10;
	tmp /= 10;
	nixie_buf[6] = tmp % 10;
//	tmp /= 10;
//	nixie_buf[5] = tmp % 10;
}

// ******************** 主界面按键处理 ********************
void key_handler(key_code)
{
	switch (key_code)
	{
		case KEY_S7:
			auto_manual_toggle = !auto_manual_toggle;
			auto_manual_led();
			break;
		case KEY_S6:
			if (auto_manual_toggle == 0)
			{
				humidity_thresh_adjust();
			}
			else
			{
				buzzer_toggle();
			}
			break;
		case KEY_S5:
			if (auto_manual_toggle == 1)
			{
				relay_on();
			}
			break;
		case KEY_S4:
			if (auto_manual_toggle == 1)
			{
				relay_off();
			}
			break;
		case KEY_S16:
			debug_rom_view();
		default: break;
	}
}

// ******************** 手动调整程序********************
void humidity_thresh_adjust()
{
	bit loop_on = 1;
	uchar which_key = 0xff;
	
	nixie_buf_backup();
	nixie_buf_humidity_adjust();
	do
	{
		delay(10);
		which_key = keyboard_return_code();
		if (which_key != 0xff)
		{
			if (which_key == KEY_S5)  // +1
			{
				humidity_threshould == 99 ?
					humidity_threshould = 0 :
					++humidity_threshould;
			}
			if (which_key == KEY_S4)  // -1
			{
				humidity_threshould == 0 ?
					humidity_threshould = 99:
					--humidity_threshould;
			}
			if (which_key == KEY_S6)
				loop_on = 0;
		}
		nixie_buf_humidity_adjust();
	} while (loop_on);
	delay(50);
	at24c02_write(0x00,humidity_threshould);
	delay(50);
	nixie_buf_restore();
}

// ******************** 蜂鸣器开/关 ********************
void buzzer_toggle()
{
	buzzer_status = !buzzer_status;
}

// ******************** 继电器 off ********************
void relay_off()
{
	misc_val = (misc_val | 0x10);
	set_misc(misc_val);
	set_lock();
}

// ******************** 继电器 on ********************
void relay_on()
{
	misc_val = (misc_val & 0xef);
	set_misc(misc_val);
	set_lock();
}

// ******************** 自动/手动指示 ********************
void auto_manual_led()
{
	if (auto_manual_toggle ==0)
	{
		set_led(0xff);
		set_led(~0x01);
	}
	else
	{
		set_led(0xff);
		set_led(~0x02);
	}
}

// ******************** 蜂鸣器动作 ********************
// 仅当auto_manual_toggle == 1有效
void alarm_watchdog(uchar current_humidity)
{
	if (auto_manual_toggle == 1)  // manual
	{
		if (buzzer_status == 1 && current_humidity < humidity_threshould)
		{
			misc_val = (misc_val | 0x40);
			set_misc(misc_val);
			set_lock();
		}
		else
		{
			misc_val = (misc_val & 0xbf);
			set_misc(misc_val);
			set_lock();
		}
	}
	else  // auto
	{
		misc_val = (misc_val & 0xbf);
		set_misc(misc_val);
		set_lock();
		if (current_humidity < humidity_threshould)
		{
			relay_on();
		}
		else 
		{
			relay_off();
		}
	}
}


// ******************** 显示rom内容 ********************
void debug_nixie_buf_rom()
{
	uchar i;
	uchar tmp;
	for (i = 0; i < 5 ; i++)
		nixie_buf[i] = NIXIE_BLANK;
	tmp = rom_content;
	nixie_buf[7] = tmp % 10;
	tmp /= 10;
	nixie_buf[6] = tmp % 10;
	tmp /= 10;
	nixie_buf[5] = tmp % 10;
}

// ******************** 显示rom内容 ********************
void debug_rom_view()
{
	bit loop_on = 1;
	uchar kcode;
	nixie_buf_backup();
	delay(50);
	rom_content = at24c02_read(0x00);
	delay(50);
	debug_nixie_buf_rom();
	do
	{
		delay(10);
		kcode = keyboard_return_code();
		if (kcode != 0xff)
		{
			if (kcode == KEY_S16)
				loop_on = 0 ;
		}
	} while (loop_on);
	
	nixie_buf_restore();
}