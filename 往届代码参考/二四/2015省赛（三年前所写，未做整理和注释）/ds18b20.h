#define OW_SKIP_ROM 0xcc
#define DS18B20_CONVERT 0x44
#define DS18B20_READ 0xbe

//IC引脚定义
sbit DQ = P1^4;
uint ds18b20Num = 0;
uchar theTemp = 0;


//单总线延时函数
void Delay_OneWire(unsigned int t)
{
	uchar i;
 	while(t--){
		for (i = 0; i < 12; i++);
	}
}

//DS18B20芯片初始化
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

//通过单总线向DS18B20写一个字节
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
	}
	return dat;
}

void readTemp(){
	uchar a, b;	
	

	Init_DS18B20();
	TR0 = 0;
	Write_DS18B20(OW_SKIP_ROM);
	Write_DS18B20(DS18B20_CONVERT);
	TR0 = 1;

	Delay_OneWire(200);

	
	Init_DS18B20();
	TR0 = 0;
	Write_DS18B20(OW_SKIP_ROM);
	Write_DS18B20(DS18B20_READ);
	TR0 = 1;

	a = Read_DS18B20();
	b = Read_DS18B20();

	ds18b20Num = a;
	ds18b20Num = ds18b20Num << 8;
	ds18b20Num = ds18b20Num|b;
	ds18b20Num = ds18b20Num * 0.0625;
	ds18b20Num = ds18b20Num + 50;
	ds18b20Num = ds18b20Num / 100;
	theTemp = ds18b20Num;
}

