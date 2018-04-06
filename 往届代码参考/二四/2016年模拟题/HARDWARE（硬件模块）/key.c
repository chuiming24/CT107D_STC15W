//文件名：key.c
#include <stc_config.h>


sbit KEY1 = P3^0;
sbit KEY2 = P3^1;
sbit KEY3 = P3^2;
sbit KEY4 = P3^3;


void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}


//用于扫描键盘
//返回：键值
uchar scanKey(){
	uchar output = 0xff;
	uchar temp = 0;
	
	//P3.6 == P42,  
	//P3.7 == P44
	
	P44 = 0;
	P42 = 1;
	P3 = 0xff & ~(1<<7);  //1<<7  1000 0000  ~(1<<7)   0111 1111
	temp = P3 & 0x0f;
	if(temp != 0x0f){
		Delay10ms();
		
		temp = P3 & 0x0f;
		if(temp != 0x0f){
			switch(temp){
				case 0x07: output = 0; break;//0111
				case 0x0b: output = 7; break;//1011
				case 0x0d: output = 4; break;//1101
				case 0x0e: output = 1; break;//1110
			}
		}
		
		while(temp != 0x0f){
			temp = P3 & 0x0f;
		}
	}
	
	P44 = 1;
	P42 = 0;
	P3 = 0xff & ~(1<<6);  //1<<7  1000 0000  ~(1<<7)   1011 1111
	temp = P3 & 0x0f;
	if(temp != 0x0f){
		Delay10ms();
		
		temp = P3 & 0x0f;
		if(temp != 0x0f){
			switch(temp){
				case 0x07: output = 16; break;
				case 0x0b: output = 8; break;
				case 0x0d: output = 5; break;
				case 0x0e: output = 2; break;
			}
		}
		
		while(temp != 0x0f){
			temp = P3 & 0x0f;
		}
	}

	P44 = 1;
	P42 = 1;
	P3 = 0xff & ~(1<<5);  //1<<7  1000 0000  ~(1<<7)   1101 1111
	temp = P3 & 0x0f;
	if(temp != 0x0f){
		Delay10ms();
		
		temp = P3 & 0x0f;
		if(temp != 0x0f){
			switch(temp){
				case 0x07: output = 17; break;
				case 0x0b: output = 9; break;
				case 0x0d: output = 6; break;
				case 0x0e: output = 3; break;
			}
		}
		
		while(temp != 0x0f){
			temp = P3 & 0x0f;
		}
	}

	
	return output;
}