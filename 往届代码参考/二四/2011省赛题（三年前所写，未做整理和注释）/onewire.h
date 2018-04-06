#ifndef _ONEWIRE_H_
#define	_ONEWIRE_H_
#include<stc15.h>
#define OW_SKIP_ROM 0xcc
#define DS18B20_CONVERT 0x44
#define DS18B20_READ 0xbe
#define uchar unsigned char
#define uint unsigned int
//IC引脚定义
sbit DQ = P1^4;

/*
  程序说明: 单总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台
  日    期: 2011-8-9
*/



//单总线延时函数
void Delay_OneWire(unsigned int t);
//DS18B20芯片初始化
bit Init_DS18B20(void);
//通过单总线向DS18B20写一个字节
void Write_DS18B20(unsigned char dat);
//从DS18B20读取一个字节
unsigned char Read_DS18B20(void);
//读取温度

uint readTheTemp(void);
#endif