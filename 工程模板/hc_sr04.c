#include <config.h>
#include <hc_sr04.h>

sbit tx = P1^0;
sbit rx = P1^1;

void Hcsr04Init_Timer1(void)
{
	TMOD |= 0x10;
	TH1 = TL1 = 0;
}

void Delay14us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 36;
	while (--i);
}

void SendWave(){
	tx = 0;
	tx = 1;
	Delay14us();
	tx = 0;
	while(!tx);
}

uint GetDistance(){
	uint Distance;
	rx = 0;
	SendWave();
	TR1 = 1;	
	while(rx && !TF1);
	TR1 = 0;
	if(TF1 == 1){
	   	TF1 = 0;
		Distance = 65535;
	}else{
		Distance = TH1;
		Distance <<= 8;
		Distance |= TL1;
		Distance = Distance * 0.17;	//·Å´ó1000±¶		
	}
	TH1 = TL1 = 0;
	return Distance;
}

