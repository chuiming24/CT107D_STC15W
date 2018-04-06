/*
  程序说明: IIC总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台(12MHz)
  日    期: 2011-8-9
*/

#include "iic.h"

//#define DELAY_TIME 10

//总线启动条件
void iic_delay(uchar n)
{
	do
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		
		_nop_();
		_nop_();
	}
	while(n--);
	
}


//??????
void IIC_Start(void)
{
	SDA = 1;
	SCL = 1;
	iic_delay(5);
	SDA = 0;
	iic_delay(5);
	SCL = 0;	
}

//??????
void IIC_Stop(void)
{
	SDA = 0;
	SCL = 1;
	iic_delay(5);
	SDA = 1;
	iic_delay(5);
}

//?????
void IIC_Ack(unsigned char ackbit)
{
	
	SCL = 0;
	SDA = ackbit;
	iic_delay(5);
	SCL = 1;
	iic_delay(5);
	SCL = 0;
	SDA = 1; 
	iic_delay(5);
}

//????
bit IIC_WaitAck(void)
{
	bit temp;
	SDA = 1;
	iic_delay(5);
	SCL = 1;
	iic_delay(5);
	temp = SDA;
	SCL = 0;
	return temp;
}

//??I2C??????
void IIC_SendByte(unsigned char byt)
{
	unsigned char i;
	EA = 0;
	for(i=0;i<8;i++)
	{   
		if(byt&0x80) 
		{	
			SDA = 1;
		}
		else 
		{
			SDA = 0;
		}
		iic_delay(5);
		SCL = 1;
		byt <<= 1;
		iic_delay(5);
		SCL = 0;
	}
	EA = 1;
}

//?I2C???????
unsigned char IIC_RecByte(void)
{
	unsigned char da;
	unsigned char i;
	EA = 0;
	for(i=0;i<8;i++)
	{   
		SCL = 1;
		iic_delay(5);
		da <<= 1;
		if(SDA) 
		da |= 0x01;
		SCL = 0;
		iic_delay(5);
	}
	EA = 1;
	return da;
}

// 初始化进行如下操作
// 1.对PCF8591进行总线寻址（同时写入“写”）
// 2.告诉被寻址到的PCF8591控制字（选择通道三）
// 初始化的原因：避开第一个读到的无效的80H
// (In A/D) After a Power-on reset condition the first byte read is a hexadecimal 80
void pcf8591_init()
{
	EA = 0;
	IIC_Start();
	IIC_SendByte(PCF8591_HWADDR_W);
	
	IIC_WaitAck();

	IIC_SendByte(0x03);  // ch3
	IIC_WaitAck();

	IIC_Stop();
	EA = 1;

}

// adc时序：
// S -> Host总线寻址（写入硬件地址）（最低位为“写”(0)） -> 8591 ACK -> 控制字(选择通道3) ->
// 8591 ACK -> Data Bytes... -> Host ACK -> P
uchar pcf8591_adc()
{
	uchar tmp;
	EA = 0;
	
	IIC_Start();
	IIC_SendByte(PCF8591_HWADDR_W);
	
	IIC_WaitAck();
	IIC_SendByte(0x03);  // ch3
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(PCF8591_HWADDR_R);
	IIC_WaitAck();
	tmp = IIC_RecByte();
	IIC_Ack(1);
	IIC_Stop();
	delay(50);
	EA = 1;
	return tmp;

}


void at24c02_write(uchar iic_addr, uchar iic_data)
{
	EA = 0;
	IIC_Start();
	IIC_SendByte(SlaveAddrW);
	IIC_WaitAck();

	IIC_SendByte(iic_addr);
	IIC_WaitAck();

	IIC_SendByte(iic_data);
	IIC_WaitAck();

	IIC_Stop();
	EA = 1;

}

uchar at24c02_read(uchar iic_addr)
{
	uchar tmp;

	EA = 0;
	IIC_Start();
	IIC_SendByte(SlaveAddrW);
	IIC_WaitAck();

	IIC_SendByte(iic_addr);
	IIC_WaitAck();

	
	IIC_Start();
	IIC_SendByte(SlaveAddrR);
	IIC_WaitAck();

	tmp = IIC_RecByte();
	IIC_WaitAck();
	IIC_Stop();
	EA = 1;
	return tmp;
	
}