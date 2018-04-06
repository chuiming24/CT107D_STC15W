#include <STC15F2K60S2.H>
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int 
#define u32 unsigned long int

#include <gpio.h>
#include <dsp.h>


#include <ds18b20.h>
#include <ds1302.h>

uchar time_keyFlag = 0;
uchar time_keyCnt = 0;
uchar time_ds18b20Flag = 0;
uchar time_ds18b20Cnt = 0;
uchar time_ds1302Flag = 0;
uchar time_ds1302Cnt = 0;
uchar time_SecCnt = 0;


uchar stat = 0;
uchar stat0_setSecNum[4] = {1, 5, 30, 60};
uchar stat0_setSecNumCount = 0;


uchar stat1_ds18b20Num[10] = {0};
uchar stat1_ds18b20NumCount = 0;
uchar stat1_setSecNumMax[4] = {2, 10, 60, 120};


uchar stat2_Cnt = 0;

#include <key.h>

void Timer0Init(void)		//2毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x40;		//设置定时初值
	TH0 = 0xA2;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}



void main(){
	initGpio();
	P0 = 0xff;setY4;
	P0 = 0x00;setY5;

	Timer0Init();
	ET0 = 1;
	EA = 1;	

	set_rtc();
	while(1){
		if(time_keyFlag == 1){
			time_keyFlag = 0;
			scanKey();
		}

		if(time_ds1302Flag == 1){
			time_ds1302Flag = 0;
			read_rtc();	
			setDsp2(time_data[4], time_data[5], time_data[6]);						
		}
		if(stat == 1){
			if(time_ds18b20Flag == 1){
				time_ds18b20Flag = 0;
				time_SecCnt++;
				if(time_SecCnt == stat1_setSecNumMax[stat0_setSecNumCount]){
					time_SecCnt = 0;
					readTemp();					
					stat1_ds18b20Num[stat1_ds18b20NumCount] = theTemp;
					stat1_ds18b20NumCount++;
					if(stat1_ds18b20NumCount == 10){
						stat1_ds18b20NumCount = 0;
						stat = 2;
						setLed(0, 1);
					}					 
				}
			}
		}

		switch(stat){
			case 0:
				setDsp1(stat0_setSecNum[stat0_setSecNumCount]);
				break;
			case 1:	
				setDsp2(time_data[4], time_data[5], time_data[6]);				
				break;
			case 2:
				setDsp3(stat2_Cnt+1, stat1_ds18b20Num[stat2_Cnt]);
				break;
		};	
	}
}

void timer0() interrupt 1{
	display();
	time_keyCnt++;
	if(time_keyCnt == 5){
		time_keyCnt = 0;
		time_keyFlag = 1;	
	}

	if(stat == 1){
		time_ds1302Cnt++;
		if(time_ds1302Cnt == 50){
			time_ds18b20Cnt = 0;
			time_ds1302Flag = 1;	
		}
	}
	if(stat == 1){
		time_ds18b20Cnt++;
		if(time_ds18b20Cnt == 250){
			time_ds18b20Cnt = 0;			
			time_ds18b20Flag = 1;
		}
	}
}