#include "config.h"
#include "wave.h"

uint Distance_Get(void)
{
	uint Distance;
	uchar Count = 0;

//	EA = 0;
	RX = 1;
	TR1 = 1;
	Send_Wave();		
	while (RX && !TF1);
	TR1 = 0;
	if (TF1)
	{
		TF1 = 0;
		Distance = 0;
	}
	else
	{
		Distance = TH1;
		Distance <<= 8;
		Distance |= TL1;
		Distance = Distance * 0.17;	//�Ŵ�1000��	
	}
	TH1 = TL1 = 0;
//	EA = 1;
	
	return Distance;	
}
void Init_Timer1(void)
{
	TMOD |= 0x10;
	TH1 = TL1 = 0;
}

void Send_Wave(void)
{
	uchar i;

	for (i = 0; i < 8; i++)
	{
		TX = 1;
		Delay12us();
		TX = 0;
		Delay12us();
	}
}

void Delay12us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	_nop_();
	i = 30;
	while (--i);
}
