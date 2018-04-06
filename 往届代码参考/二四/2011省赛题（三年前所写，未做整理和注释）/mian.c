#include <config.h>
#include <init.h>
#include <display.h>
#include <key.h>
#include <onewire.h>
#include <at24c02.h> 

uchar inr = 0;	  //按键计数
uchar key_flag = 0;			//按键标志
uchar key_temp = 0;			//当前按下按键存储

uint temp_inr = 0;		   //温度读取计数，每250ms读一次
uchar temp_flag = 0;	   //温度标志
uchar tempValue = 0;	   //当前读取温度

uchar temp_up = 30;		   //上限
uchar temp_down = 20;	   //下限

void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}

void Timer1Init(void)		//100微秒@12.000MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x50;		//设置定时初值
	TH1 = 0xFB;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
}


void main(){
	gpio();					//初始化针脚
	Timer0Init();			//初始化计数器0 用于计数
	Timer1Init();
	EA = 1;				  
	PT1 = 1;			  //将计数器1的优先度调为最高
	PT0 = 0;			  //将计数器0的优先度调为0，及最低
	ET0 = 1;
	temp_up = readEeprom(0x00);					 //读取上下限
	temp_down = readEeprom(0x01);
	if(temp_down >= temp_up){
		temp_down = temp_up;
	}							 //当读取数据混乱的时候进行一定处理，使其符合萝莉
	while(1){
		if(key_flag){
			key_flag = 0;
		   	key_temp = scanKey();	  //扫描独立按键，该独立按键为状态机，无法同时识别俩次，无按下时候返回0xff
			if(key_temp != 0xff){
				switch(key_temp){
					case 1:
						if(temp_down > 0){
							temp_down--;
						}
						break;
					case 2:
						if(temp_up > temp_down){
							temp_up--;
						}
						break;
					case 3:
						if(temp_down < temp_up){
							temp_down++;
						}
						break;
					case 4:
						if(temp_up < 99){
							temp_up++;
						}
						break;
				}
				writeEeprom(0x00, temp_up);				//显示上下限
				writeEeprom(0x01, temp_down);				
			}
		}
		if(temp_flag){
			temp_flag = 0;
			tempValue = (uchar)(readTheTemp() /100);
			settemp(tempValue);
			if(tempValue > temp_up){
				ET1 = 1;				  //打开PWM

				ET0 = 0;				  //暂时关闭显示定时器，防止干扰到P0口
				P0 = 0x00; 
				setY5();
				ET0 = 1;
			}else if(tempValue < temp_down){
				ET1 = 0;
				P34 = 1;

				ET0 = 0;
				P0 = 0x10; 
				setY5();
				ET0 = 1;
			}else{
				ET1 = 0;
				P34 = 1;

				ET0 = 0;
				P0 = 0x00; 
				setY5();
				ET0 = 1;				
			}
		}

		setdown(temp_down);			//显示上下限
		setup(temp_up);
	}
}

void time0_isr() interrupt 1{
	display();	
	if(++inr == 10){
		inr = 0;
		key_flag = 1;
	}
	if(++temp_inr == 250){
		temp_inr = 0;
		temp_flag = 1;
	}
}

void time1_isr() interrupt 3{
	static uchar cnt;
	if(++cnt == 3){
		P34 = 0;
	}else if(cnt >= 10){
		cnt = 0;
		P34 = 1;
	}		
}