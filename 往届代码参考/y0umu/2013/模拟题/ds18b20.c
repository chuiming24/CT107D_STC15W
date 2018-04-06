#include "ds18b20.h"

uchar ds18b20_data_8 = 0;

//???????
void Delay_OneWire(unsigned int t)
{
  unsigned char i;
	do
	{
		_nop_();
		_nop_();
		i = 66;
		while (--i);
	}
	while(t--);
}

//DS18B20?????
bit Init_DS18B20(void)
{
	bit initflag = 0;
	DQ = 1;
	Delay_OneWire(12);
	DQ = 0;
	Delay_OneWire(80); 
	DQ = 1;
	Delay_OneWire(10); 
	initflag = DQ;    
	Delay_OneWire(5);
  
	return initflag;
}

//??????DS18B20?????
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//?DS18B20??????
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
	}
	return dat;
}

uchar ds18b20_return_temp()
{
	uchar high,low, temperature;
	EA = 0;
	Init_DS18B20();
	Write_DS18B20(SKIP_ROM);
	Write_DS18B20(CVT_T);
	Delay_OneWire(200);
	EA = 1;
	
	EA = 0;
	Init_DS18B20();
	Write_DS18B20(SKIP_ROM);
	Write_DS18B20(RD_SCRATCH);
	low = Read_DS18B20();
	high = Read_DS18B20();
	temperature = (high << 4) | (low >> 4);
	EA = 1;
	
	return temperature;
}