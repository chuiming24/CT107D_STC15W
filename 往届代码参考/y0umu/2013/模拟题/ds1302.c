#include "ds1302.h"
#include "common.h"
/********************************************************************/ 


enum
{
	DS1302_SEC_W = 0X80,
	DS1302_SEC_R,
	DS1302_MINUTE_W,
	DS1302_MINUTE_R,
	DS1302_HOUR_W,
	DS1302_HOUR_R,
	DS1302_DATE_W,
	DS1302_DATE_R,
	DS1302_MONTH_W,
	DS1302_MONTH_R,
	DS1302_DAY_W,
	DS1302_DAY_R,
	DS1302_YEAR_W,
	DS1302_YEAR_R,
	DS1302_WP_W,
	DS1302_WP_R
};


#define ds1302_write_enable() {Ds1302_Single_Byte_Write(ds1302_control_addr,0x00);}
#define ds1302_write_disable() {Ds1302_Single_Byte_Write(ds1302_control_addr,0x80);}

uchar bcd2dec(uchar bcd)
{
	uchar tmp;
	tmp = (bcd >> 4) * 10 + (bcd & (0x0f));
	return tmp;
}


/*单字节写入一字节数据*/
void Write_Ds1302_Byte(unsigned char dat) 
{
	unsigned char i;
	SCK = 0;
	for (i=0;i<8;i++) 
	{ 
		if (dat & 0x01) 	// 等价于if((addr & 0x01) ==1) 
		{
			SDA_SET;		//#define SDA_SET SDA=1 /*电平置高*/
		}
		else 
		{
			SDA_CLR;		//#define SDA_CLR SDA=0 /*电平置低*/
		}		 
		SCK_SET;
		SCK_CLR;		
		dat = dat >> 1; 
	} 
}
/********************************************************************/ 
/*单字节读出一字节数据*/
unsigned char Read_Ds1302_Byte(void) 
{
	unsigned char i, dat=0;	
	for (i=0;i<8;i++)
	{	
		dat = dat >> 1;
		if (SDA_R) 	  //等价于if(SDA_R==1)    #define SDA_R SDA /*电平读取*/	
		{
			dat |= 0x80;
		}
		else 
		{
			dat &= 0x7F;
		}
		SCK_SET;
		SCK_CLR;
	}
	return dat;
}

/********************************************************************/ 
/*向DS1302 单字节写入一字节数据*/
void Ds1302_Single_Byte_Write(unsigned char addr, unsigned char dat)
{ 

	RST_CLR;			/*RST脚置低，实现DS1302的初始化*/
	SCK_CLR;			/*SCK脚置低，实现DS1302的初始化*/

	RST_SET;			/*启动DS1302总线,RST=1电平置高 */
	addr = addr & 0xFE;	 
	Write_Ds1302_Byte(addr); /*写入目标地址：addr,保证是写操作,写之前将最低位置零*/	
	Write_Ds1302_Byte(dat);	 /*写入数据：dat*/
	RST_CLR;				 /*停止DS1302总线*/
}

/********************************************************************/ 
/*从DS1302单字节读出一字节数据*/
unsigned char Ds1302_Single_Byte_Read(unsigned char addr) 
{ 
	unsigned char temp;
	RST_CLR;			/*RST脚置低，实现DS1302的初始化*/
	SCK_CLR;			/*SCK脚置低，实现DS1302的初始化*/

	RST_SET;	/*启动DS1302总线,RST=1电平置高 */	
	addr = addr | 0x01;	 
	Write_Ds1302_Byte(addr); /*写入目标地址：addr,保证是读操作,写之前将最低位置高*/
	temp=Read_Ds1302_Byte(); /*从DS1302中读出一个字节的数据*/		
	
	RST=0;
	_nop_();
 	RST=0;
	SCK=0;
	_nop_();
	SCK=1;
	_nop_();
	SD=0;
	_nop_();
	SD=1;
	_nop_();
//	RST_CLR;	/*停止DS1302总线*/
	return temp;
}

void ds1302_reset()
{
//	EA = 0;
	ds1302_write_enable();
	Ds1302_Single_Byte_Write(ds1302_min_addr, 0x30);
	Ds1302_Single_Byte_Write(ds1302_hr_addr, 0x08);
	Ds1302_Single_Byte_Write(ds1302_sec_addr, 0x00);
	ds1302_write_disable();
//	EA = 1;
}

void ds1302_gettime(uchar *sec, uchar *min, uchar *hr)
{
	uchar tmp;
	EA = 0;
	tmp = Ds1302_Single_Byte_Read(ds1302_sec_addr+1);
	tmp &= 0x7f;
	*sec = bcd2dec(tmp);
	
	tmp = Ds1302_Single_Byte_Read(ds1302_min_addr+1);
	*min = bcd2dec(tmp);
	
	tmp = Ds1302_Single_Byte_Read(ds1302_hr_addr+1);
	tmp &= 0x7f;
	*hr = bcd2dec(tmp);
	
	EA = 1;
}