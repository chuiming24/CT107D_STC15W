#include "config.h"
#include "exti.h"
#include "display.h"
#include "key.h"
#include "frecounter.h"


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

void Timer0Init(void)		//1����@12.000MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x20;		//���ö�ʱ��ֵ
	TH0 = 0xD1;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ 
}



void time0_isr() interrupt 1{
	display();
	KeyScan();
	FreCntScan();

	if(SoftTimer0Cnt)		 //�����ʱ��0���д���
	{
		SoftTimer0Cnt--;
		if(SoftTimer0Cnt==0)
			SoftTimer0Over=1;
	}	
	if(SoftTimer1Cnt)		 //�����ʱ��1���д���
	{
		SoftTimer1Cnt--;
		if(SoftTimer1Cnt==0)
			SoftTimer1Over=1;
	}
	if(SoftTimer2Cnt)		 //�����ʱ��2���д���
	{
		SoftTimer2Cnt--;
		if(SoftTimer2Cnt==0)
			SoftTimer2Over=1;
	}
	if(SoftTimer3Cnt)		 //�����ʱ��3���д���
	{
		SoftTimer3Cnt--;
		if(SoftTimer3Cnt==0)
			SoftTimer3Over=1;
	}	
	if(SoftTimer4Cnt)		 //�����ʱ��4���д���
	{
		SoftTimer4Cnt--;
		if(SoftTimer4Cnt==0)
			SoftTimer4Over=1;
	}			
}