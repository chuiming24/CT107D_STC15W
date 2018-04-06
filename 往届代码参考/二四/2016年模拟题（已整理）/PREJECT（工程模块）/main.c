//文件名：main.c

//每10ms扫描一次按键，把按键的数值传到数码管上

//系统头文件
#include "stc_config.h"

//外设头文件
#include "display.h"
#include "key.h"
#include "onewire.h"

//功能函数头文件
#include "word_gui.h"
#include "set_gui.h"



//系统变量
uchar sys_stat = WORD_GUI;
uchar sys_temp_upper = 18;
uchar sys_temp_lower = 20;
uchar sys_led_cache = 0xff;


//定时器变量
uint Key_count = 0;	//键盘计数变量
bit Key_Flag = 0;		//标志位

uint Temp_count = 0;	//温度计数变量
bit Temp_Flag = 0;		//标志位

uint led_count = 0; 	//led软件定时器计数变量
uchar led_usedFlag = 0; //led软件定时器使用标志


void Timer0Init(void)	
{
	AUXR &= 0x7F;	
	TMOD &= 0xF0;	
	TL0 = 0xCD;	
	TH0 = 0xF8;	
	TF0 = 0;		
	TR0 = 1;	
	ET0 = 1;
	EA = 1;
}



void main(){
	
	//初始化
	uchar Key_temp = 0xff;
	int Temp_temp = 0;
	
	
	P0 = 0x00;
	setOpen;
	P0 = 0xff;
	setLed;

	
	
	Timer0Init();
	while(1){
		switch(sys_stat){
			case WORD_GUI:
				word_gui_funtion();
			
				break;
			case SET_GUI:
				set_gui_funtion();
			
				break;
		}
	}
}



void timer0() interrupt 1{		//1ms
	Key_count++;
	if(Key_count == 10){
		Key_count = 0;
		Key_Flag = 1;
	}
	
	Temp_count++;
	if(Temp_count ==250){
		Temp_count = 0;
		Temp_Flag = 1;
	}
	
	if(led_count != 0){
		led_count--;
	}
	
	
	display();
}
