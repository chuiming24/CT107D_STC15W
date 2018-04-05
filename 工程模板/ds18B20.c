#include "config.h"
#include "onewire.h"
#include "ds18b20.h"


uchar Get_Temp(void)
{
	uchar TH, TL, temp;
	Init_DS18B20();
	Write_DS18B20(OW_SKIP_ROM);
	Write_DS18B20(DS18B20_CONVERT);
	Delay_OneWire(200);
	
	Init_DS18B20();
	Write_DS18B20(OW_SKIP_ROM);
	Write_DS18B20(DS18B20_READ);
	TL = Read_DS18B20();
	TH = Read_DS18B20();
	temp = TH << 4;
	temp |=	(TL >> 4);

	return temp;
}
