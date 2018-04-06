#ifndef _IIC_H
#define _IIC_H

#include "common.h"
#include "intrins.h"

#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();\
				_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();\
				_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();\
				_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();\
				_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}

#define PCF8591_HWADDR_W 0x90
#define PCF8591_HWADDR_R 0x91

#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1

//总线引脚定义
sbit SDA = P2^1;  /* 数据线 */
sbit SCL = P2^0;  /* 时钟线 */

//函数声明
void iic_delay(uchar n);
void IIC_Start(void); 
void IIC_Stop(void);  
void IIC_Ack(unsigned char ackbit); 
void IIC_SendByte(unsigned char byt); 
bit IIC_WaitAck(void);  
unsigned char IIC_RecByte(void); 

void pcf8591_init();
uchar pcf8591_adc();


void at24c02_write(uchar iic_addr, uchar iic_data);
uchar at24c02_read(uchar iic_addr);
#endif