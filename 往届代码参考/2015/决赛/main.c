#include "common.h"
#include "iic.h"
#include "sonic.h"
#include <intrins.h>
// Nixie
// 0 1 2 3 4 5 6 7 8 9 - (blank out)
code uchar nixie_tab[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xbf, 0xff};
// Nixie buffer
uchar nixie_buf[8] = {NIXIE_BLANK,NIXIE_BLANK,NIXIE_BLANK,NIXIE_BLANK,NIXIE_BLANK,NIXIE_BLANK,NIXIE_BLANK,NIXIE_BLANK};
uchar nixie_buf_bak[8] = {0};

// Distance detect
uchar distance = 40;  // init as an invalid

// Transfet time
uchar trans_time_1 = 2;
uchar trans_time_2 = 4;

uchar misc_val = 0xbf;  // 关关  

// 保存计时器时间
uchar timer = 0;

typedef enum _load_status
{
	NOLOAD,
	OVERLOAD,
	OK,
	WORKING,
	UNDETERMINED
} load_status;

// Global variables
bit keyS4_allow = 1;
bit keyS5_allow = 0;
bit keyS6_allow = 0;
bit keyS7_allow = 0;

bit button_scan_flag = 0;
load_status ld_status = UNDETERMINED;
bit adc_refresh_flag = 0;
bit overload_alarm = 0;
bit sonic_refresh_flag = 0;
bit is_transfering = 0;
bit halt_alarm = 0;
bit set_trans_time_flag = 0;
bit nixie_34_blink_flag = 0;
bit nixie_34_on_off_status = 0;
bit nixie_67_blink_flag = 0;
bit nixie_67_on_off_status = 0;

// Declarations
void startup();
void nixie_show_all();
void nixie_buf_backup();
void nixie_buf_restore();
uchar button_return_code();
void key_handler(uchar key_code);
load_status get_load_status();
void load_status_indicate();
void nixie_buf_ditance_detect();
void nixie_buf_trans_status();
void trans_start();
void trans_halt();
void trans_time_set();
void nixie_buf_trans_time_set();
void nixie_buf_noload();

// ******************** main ********************
void main()
{
	uchar key_code = 0xff;
	
	startup();
	
	while(1)
	{
		if (button_scan_flag)
		{
			button_scan_flag = 0;
			key_code = button_return_code();
			if (key_code != KEY_NULL)
			{
				key_handler(key_code);
			}
			
		}
		
		if (is_transfering == 0 && halt_alarm == 0 && adc_refresh_flag)
		{
			adc_refresh_flag = 0;
			ld_status = get_load_status();
			load_status_indicate();
		}
		if (ld_status != NOLOAD && is_transfering == 0 && halt_alarm == 0 && sonic_refresh_flag)
		{
			sonic_refresh_flag = 0;
			distance = distance_measure();
			nixie_buf_ditance_detect();
		}
	}
	
}

// ******************** Startup code (run once) ********************
void startup()
{
	
	set_misc(MISC_nBUZZ_nRELAY);   // 关蜂鸣器
	set_lock();
	set_led(0xff);
	set_lock();
	
	// PCF8591
	pcf8591_init();
	
	// Timer
	TMOD = 0x01;
	TH0 = TH0_RLD;
	TL0 = TL0_RLD;
	
	TH1 = TH1_RLD;
	TL1 = TL1_RLD;

	ET0 = 0;
	ET1 = 1;

	TR0 = 0;
	TR1 = 1;

//	AUXR &= 0xFB;		//12T
//	T2H = T2H_RLD;
//	T2L = T2L_RLD;
	IE2 &= ~0x40;   // ET2 = 0
//	AUXR |= 0x10;
	
	EA = 1;
	
	trans_time_1 = at24c02_read(0x00);
	delay(50);
	trans_time_2 = at24c02_read(0x01);
	
}

// ******************** timer 0 isr ********************
//void timer0_isr() interrupt 1
//{
//	TR0 = 0;
//	TH0 = TH0_RLD;
//	TL0 = TL0_RLD;

//	

//	TR0 = 1;
//}

// ******************** timer 1 isr ********************
void timer1_isr() interrupt 3
{
	static uchar button_scan_rate = 0;
	static uchar adc_refresh_rate = 0;
	static uchar sonic_refresh_rate = 0;
	static uchar isr_cnt_1 = 0;
	static uint isr_cnt_2 = 0;
	static uint isr_cnt_3 = 0;
	static uchar isr_cnt_4 = 0;
	static bit led3_on_off_status = 0;
	static bit led4_on_off_status = 0;
//	TR1 = 0;
//	TH1 = TH1_RLD;
//	TL1 = TL1_RLD;

	nixie_show_all();
	
	if (overload_alarm)
	{
		if (++isr_cnt_1 == 250)
		{
			isr_cnt_1 = 0;
			if (led3_on_off_status)
			{
				set_led(~0x04);
	//			set_lock();
			}
			else
			{
				set_led(0xff);
	//			set_lock();
			}
			
			led3_on_off_status = !led3_on_off_status;
		}
	}
	
	if (halt_alarm)
	{
		if (++isr_cnt_3 == 250)
		{
			isr_cnt_3 = 0;
			if (led4_on_off_status)
			{
				set_led(~0x08);
	//			set_lock();
			}
			else
			{
				set_led(0xff);
	//			set_lock();
			}
			
			led4_on_off_status = !led4_on_off_status;
		}
	}
	
	if (is_transfering)
	{
		nixie_buf_trans_status();
		++isr_cnt_2;
		if(isr_cnt_2 == 550)
		{
			isr_cnt_2 = 0;
			if (timer != 1 )
			{
				nixie_buf_trans_status();
				--timer;
			}
			else   // transfer complete
			{
				is_transfering = 0;
				set_misc(MISC_nBUZZ_nRELAY);
				keyS4_allow = 1;
				keyS5_allow = 0;
				keyS6_allow = 0;
				keyS7_allow = 0;
			}
		}
	}
	
	if (nixie_34_blink_flag)
	{
		++isr_cnt_4;
		if (isr_cnt_4 == 125)
		{
			isr_cnt_4 = 0;
			nixie_34_on_off_status = !nixie_34_on_off_status;
		}
	}
	
	if (nixie_67_blink_flag)
	{
		++isr_cnt_4;
		if (isr_cnt_4 == 125)
		{
			isr_cnt_4 = 0;
			nixie_67_on_off_status = !nixie_67_on_off_status;
		}
	}
	
	if (++button_scan_rate == 50)
	{
		button_scan_flag = 1;
		button_scan_rate = 0;
	}
	
	if (++sonic_refresh_rate == 75)
	{
		sonic_refresh_flag = 1;
		sonic_refresh_rate = 0;
	}
	
	if (++adc_refresh_rate == 200)
	{
		adc_refresh_flag = 1;
		adc_refresh_rate = 0;
	}
	
	
//	TR1 = 1;
}
// ******************** timer 2 isr ********************
//void timer2_isr() interrupt 12
//{
//	T2_flag = 1;
//}

// ******************** 延时 ********************
void delay(uint ms)
{
	#define DELAYING 200
	uchar i,j;
	while (ms)
	{
		for (j = 0 ; j < 12 ; j++)
		{
			for (i = 0 ; i < DELAYING ; i++);
		}
		ms--;
	}
}

// ******************** 数码管显示缓冲区的全部 ********************
void nixie_show_all()
{
	static uchar com_ptr = 0;
	set_nixie_dat(0xff);
	
	if (nixie_34_on_off_status && com_ptr == 3)
	{
		com_ptr = 5;
	}
	
	if (nixie_67_on_off_status && com_ptr == 6)
	{
		com_ptr = 0;
	}
	
	set_nixie_com(1 << com_ptr);
	
	set_nixie_dat( nixie_tab[ nixie_buf[com_ptr] ]);
	
	if (++com_ptr == 8)
		com_ptr = 0;
	
	set_lock();
}

// ******************** 数码管备份 ********************
//void nixie_buf_backup()
//{
//	uchar i;
//	for (i = 0; i < 8 ; i++)
//	{
//		nixie_buf_bak[i] = nixie_buf[i];
//	}
//}

// ******************** 数码管恢复 ********************
//void nixie_buf_restore()
//{
//	uchar i;
//	for (i = 0; i < 8 ; i++)
//	{
//		nixie_buf[i] = nixie_buf_bak[i];
//	}
//}

// ******************** button scan (not for user call!) ********************
uchar button_scan_step_1()
{
	uchar tmp;
	P3 = KEY_NULL;
	tmp = (P3 & KEY_NULL);
	if ( tmp == KEY_NULL)
		return KEY_NULL;    // 没按键
	else
		return tmp;
}

// ******************** 返回按键值 ********************
uchar button_return_code()
{
	uchar tmp[3];
	// 消抖
	tmp[0] = button_scan_step_1();
	delay(5);
	tmp[1] = button_scan_step_1();
	delay(5);
	tmp[2] = button_scan_step_1();
	if (tmp[0] != tmp[1] || tmp[1] != tmp[2])
		return KEY_NULL;
	do
	{
		P3 = KEY_NULL;
		tmp[1] = (P3 & KEY_NULL);
	} while(tmp[1] != KEY_NULL);  // 这个键仍然被按下，循环...
	return tmp[0];
}

// ******************** 按键处理 ********************
void key_handler(uchar key_code)
{
	switch(key_code)
	{
		case KEY_S4:  // transfer start
			if (keyS4_allow)
			{
				keyS4_allow = 0;
				keyS5_allow = 1;
				keyS6_allow = 0;
				keyS7_allow = 0;
				trans_start();
			}
			break;
		case KEY_S5:  // halt
			if (keyS5_allow)
			{
				keyS4_allow = 0;
				keyS5_allow = 1;
				keyS6_allow = 0;
				keyS7_allow = 0;
				trans_halt();
			}
			break;
		case KEY_S6:
			if (keyS6_allow)
			{
				keyS4_allow = 0;
				keyS5_allow = 0;
				keyS6_allow = 1;
				keyS7_allow = 1;
				trans_time_set();
				
			}
			break;
	}
}

// ******************** 负载状态 ********************
load_status get_load_status()
{
	uchar adc_val;
	adc_val = pcf8591_adc();
	if (adc_val < (uchar)(0.2 * 255.0))
	{
		return NOLOAD;
	}
	else if (adc_val >= (uchar)(0.8 * 255.0))
	{
		return OVERLOAD;
	}
	else
	{
		return OK;
	}
}


// ******************** 指示工作状态 ********************
void load_status_indicate()
{
	switch (ld_status)
	{
		case NOLOAD:
			overload_alarm = 0;
			set_led(~0x01);
			set_misc(MISC_nBUZZ_nRELAY);
		if (set_trans_time_flag == 0)
		{
			nixie_buf_noload();
		}
		else
		{
			nixie_buf_trans_time_set();
		}
			keyS4_allow = 0;
			keyS5_allow = 0;
			keyS6_allow = 1;
			keyS7_allow = 1;
			break;
		case OK:
			overload_alarm = 0;
			set_led(~0x02);
			set_misc(MISC_nBUZZ_nRELAY);
			keyS4_allow = 1;
			keyS5_allow = 0;
			keyS6_allow = 0;
			keyS7_allow = 0;
			nixie_34_blink_flag = 0;
			nixie_67_blink_flag = 0;
			nixie_34_on_off_status = 0;
			nixie_67_on_off_status = 0;
			break;
		case OVERLOAD:  // 这里是要闪烁的
			overload_alarm = 1;
			set_misc(MISC_BUZZ_nRELAY);
			keyS4_allow = 0;
			keyS5_allow = 0;
			keyS6_allow = 0;
			keyS7_allow = 0;
			nixie_34_blink_flag = 0;
			nixie_67_blink_flag = 0;
			nixie_34_on_off_status = 0;
			nixie_67_on_off_status = 0;
			break;
	}
}

// ******************** 数码管：货物类型检测界面 ********************
void nixie_buf_ditance_detect()
{
	uchar tmp;
	nixie_buf[0] = 1;
	nixie_buf[1] = nixie_buf[2] = NIXIE_BLANK;
	nixie_buf[4] = distance % 10;
	tmp = distance / 10;
	nixie_buf[3] = tmp % 10;
	nixie_buf[5] = nixie_buf[6] = NIXIE_BLANK;
	if (distance <= 30)
	{
		nixie_buf[7] = 1;
		timer = trans_time_1;
	}
	else
	{
		nixie_buf[7] = 2;
		timer = trans_time_2;
	}
}

// ******************** 数码管：货物传送状态界面 ********************
void nixie_buf_trans_status()
{
	uchar tmp;
	nixie_buf[0] = 2;
	nixie_buf[1] = nixie_buf[2] =nixie_buf[3] = nixie_buf[4] = nixie_buf[5] = NIXIE_BLANK;
	nixie_buf[7] = timer % 10;
	tmp = timer / 10;
	nixie_buf[6] = tmp % 10;
}

// ******************** 数码管：传送时间设置界面 ********************
void nixie_buf_trans_time_set()
{
	uchar tmp;
	nixie_buf[0] = 3;
	nixie_buf[1] = nixie_buf[2] =nixie_buf[5] = NIXIE_BLANK;
	nixie_buf[4] = trans_time_1 % 10;
	tmp = trans_time_1 / 10;
	nixie_buf[3] = tmp % 10;
	nixie_buf[7] = trans_time_2 % 10;
	tmp = trans_time_2 / 10;
	nixie_buf[6] = tmp % 10;
}


// ******************** NOLOAD显示 ********************
void nixie_buf_noload()
{
	uchar i;
	for (i = 0; i < 8 ; i++)
	{
		nixie_buf[i] = NIXIE_BLANK;
	}
}

// ******************** 开始传输 ********************
void trans_start()
{
	is_transfering = 1;
//	set_buzz(0);
	set_misc(MISC_nBUZZ_RELAY);
}

// ******************** 紧急停止 ********************
void trans_halt()
{
	if (halt_alarm == 0)
	{
		halt_alarm = 1;
		is_transfering = 0;
		set_lock();
		set_misc(MISC_nBUZZ_nRELAY);
		
	}
	else
	{
		halt_alarm = 0;
		is_transfering = 1;
		set_lock();
		set_misc(MISC_nBUZZ_RELAY);
		set_lock();
		set_led(~0x02);
		set_lock();
	}
	
}

// ******************** 设置传输时间 ********************
void trans_time_set()
{
	static uchar set_mode = 0;
	uchar key_code;
	bit loop_on = 1;
	set_trans_time_flag = 1;
	nixie_buf_trans_time_set();
	
	// 设置I类
	nixie_34_blink_flag = 1;
	nixie_67_blink_flag = 0;
	nixie_67_on_off_status = 0;	
	do
	{
		key_code = button_return_code();
		delay(50);
		if (key_code != KEY_NULL)
		{
			if (key_code == KEY_S6)
			{
				++set_mode;
				switch (set_mode)
				{
					default:  // 设置II类 // case 1:
					case 1:
						nixie_34_blink_flag = 0;
						nixie_67_blink_flag = 1;
						nixie_34_on_off_status = 0;
						break;
					case 2:  // 保存到EEPROM
						nixie_34_blink_flag = 0;
						nixie_67_blink_flag = 0;
						nixie_34_on_off_status = 0;
						nixie_67_on_off_status = 0;
						set_trans_time_flag = 0;
						loop_on = 0;
						set_mode = 0;
						break;
				}
			}
			if (key_code == KEY_S7)
			{
				if (set_mode == 0)
				{
					if(++trans_time_1 == 11)
						trans_time_1 = 1;
					nixie_buf_trans_time_set();
					at24c02_write(0x00, trans_time_1);
				}
				else if(set_mode == 1)
				{
					if(++trans_time_2 == 11)
						trans_time_2 = 1;
					nixie_buf_trans_time_set();
					at24c02_write(0x01, trans_time_2);
				}
			}
		}
	} while (loop_on);
}