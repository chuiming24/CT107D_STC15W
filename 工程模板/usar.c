#include "config.h"
#include "usar.h"

bit busy = 0;
bit uart_get_flag = 0;
char uart_get_data = 0;

void uart_isr() interrupt 4{
	if(RI){
		RI = 0;
		uart_get_data = SBUF;
		uart_get_flag = 1;
	}
	if(TI){
		TI = 0;
		busy = 0;
	}
}

void SendData(char dat){
	while(busy);
   	busy = 1;
	SBUF = dat;
}

void SendString(char *s){
	while(*s){
		SendData(*s++);
	}
}

void UartInit(void)		//9600bps@11.0592MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xE0;		//设定定时初值
	TH1 = 0xFE;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
}