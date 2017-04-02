#include <config.h>
#include <iic.h>
#include <at24c02.h>
#include <delay.h>

void EEPROM_Write(uchar Address, uchar Data)
{
	EA = 0;
	IIC_Start();
	IIC_SendByte(SlaveAddrW);
	IIC_WaitAck();
	IIC_SendByte(Address);
	IIC_WaitAck();
	IIC_SendByte(Data);
	IIC_WaitAck();
	IIC_Stop();
//	Delay_ms(10);
	EA = 1;
}

uchar EEPROM_Read(uchar Address)
{
	uchar Data;

	EA = 0;
	IIC_Start();
	IIC_SendByte(SlaveAddrW);
	IIC_WaitAck();
	IIC_SendByte(Address);
	IIC_WaitAck();
	IIC_Start();
	IIC_SendByte(SlaveAddrR);
	IIC_WaitAck();
	Data = IIC_RecByte();
	IIC_Ack(0);
	IIC_Stop();
	EA = 1;	
	
	return Data;	
}
