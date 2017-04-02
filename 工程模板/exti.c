#include <config.h>
#include <exti.h>
#include <display.h>
#include <key.h>
#include <frecounter.h>


uint SoftTimer0Cnt = 0;
bit SoftTimer0Over = 0;

uint SoftTimer1Cnt = 0;
bit SoftTimer1Over = 0;

uint SoftTimer2Cnt = 0;
bit SoftTimer2Over = 0;

uint SoftTimer3Cnt = 0;
bit SoftTimer3Over = 0;

uint SoftTimer4Cnt = 0;
bit SoftTimer4Over = 0;

void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时 
}



void time0_isr() interrupt 1{
	display();
	KeyScan();
	FreCntScan();

	if(SoftTimer0Cnt)		 //软件定时器0运行处理
	{
		SoftTimer0Cnt--;
		if(SoftTimer0Cnt==0)
			SoftTimer0Over=1;
	}	
	if(SoftTimer1Cnt)		 //软件定时器1运行处理
	{
		SoftTimer1Cnt--;
		if(SoftTimer1Cnt==0)
			SoftTimer1Over=1;
	}
	if(SoftTimer2Cnt)		 //软件定时器2运行处理
	{
		SoftTimer2Cnt--;
		if(SoftTimer2Cnt==0)
			SoftTimer2Over=1;
	}
	if(SoftTimer3Cnt)		 //软件定时器3运行处理
	{
		SoftTimer3Cnt--;
		if(SoftTimer3Cnt==0)
			SoftTimer3Over=1;
	}	
	if(SoftTimer4Cnt)		 //软件定时器4运行处理
	{
		SoftTimer4Cnt--;
		if(SoftTimer4Cnt==0)
			SoftTimer4Over=1;
	}			
}