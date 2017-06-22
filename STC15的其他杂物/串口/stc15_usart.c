#include <config.h>

void revData(char a);
extern bit Send_ed;



void UartInit(void)		//9600bps@11.0592MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xBF;		//定时器1时钟为Fosc/12,即12T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xE8;		//设定定时初值
	TH1 = 0xFF;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
}
char SendData(char dat){
	if(Send_ed == 1){
	   	Send_ed = 0;
		SBUF = dat;	
		return 1;	
	}
	return 0;
}

void ser() interrupt 4{
	char a;
	if(TI){
		TI = 0;
		Send_ed = 1;
	}
	if(RI){
		RI = 0;
		a = SBUF;
		revData(a);
	}
}