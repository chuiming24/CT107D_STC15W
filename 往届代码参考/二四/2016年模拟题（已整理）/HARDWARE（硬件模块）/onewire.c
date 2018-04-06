/*
  程序说明: 单总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台
  日    期: 2011-8-9
*/

#include "onewire.h"

//单总线延时函数
void Delay_OneWire(unsigned int t)
{
	uchar i;
	while(t--){
 		 for(i=0;i<12;i++);
	}
}

//DS18B20芯片初始化
bit Init_DS18B20(void)
{
	bit initflag = 0;
	DQ = 1;
	Delay_OneWire(12);
	DQ = 0;
	Delay_OneWire(80); //480us
	DQ = 1;		//	
	Delay_OneWire(10); //60us
	initflag = DQ;    //检测是否初始化完成	
	Delay_OneWire(5);
  
	return initflag;
}

//通过单总线向DS18B20写一个字节//8位


void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat & 0x01;//0或者1
		Delay_OneWire(5);//30us
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//从DS18B20读取一个字节
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
		//30us
	}
	return dat;
}

int getTemp(void){
	uchar a, b;
	uint temp;//临时变量
	float f_temp;//临时变量//可以计算小数部分
	
	//初始化
	Init_DS18B20();//基本初始化
	Write_DS18B20(0xcc);	//选择温度
	Write_DS18B20(0x44);	//转化温度

	
	//读取温度
	Init_DS18B20();
	Write_DS18B20(0xcc);//选取温度芯片
	Write_DS18B20(0xbe);//读取数据

	a = Read_DS18B20(); //回传一个16位的温度数据。
	b = Read_DS18B20();	//A是低位，B是高位
	
	//比如温度给的数据是0000 1111 0000 1011
	//                 b = 0000 1111, a = 0000 1011
	//1.b 是char,他只有8位不够放。所以先放在temp里，
	//	temp = b => temp = 0000 0000 0000 1111
	//2.把temp里面的数据移动到高位，因为b本来属于高位
	//  temp <<= 8 => temp = 0000 1111 0000 0000
	//3.把temp 和A合在一起，变成原来的数据
	//  temp |= a =>  temp = 0000 1111 0000 1011
	
	//数据处理
		//首先要把A,B合在一起
	temp = b;
	temp <<= 8;
	temp |= a;
		//转换成我们实际的温度
	
	f_temp = temp;
	f_temp = f_temp *0.0625;		//得到实际的温度
				// temp >> 4
				
				
		//四舍五入		f_temp = 24.5625
	temp = f_temp + 0.5;
	
	
	return temp; //25
  
	
	
	
}

//读char 写char
//命令式，我们要写一个命令和一个数据  
//Write_DS18B20(0x01);	//命令
//Read_DS18B20(data); //数据


//寄存器的方式。
//Write_DS18B20(0x01);	//读取还是写入
//Write_DS18B20(addr); //寄存器的地址
//Read_DS18B20(data); //寄存器的书
