#include <config.h>
#include <init.h>
#include <exti.h>
#include <display.h>
#include <key.h>
#include <at24c02.h>
#include <ds18b20.h>
#include <time.h>
#include <frecounter.h>
									 
uint temp;
uchar temp1;

void main(){
	gpio();					//初始化针脚
	P0 = 0xff;setY4;
	P0 = 0x00;setY5;
	Timer0Init();			//初始化计数器0 用于计数
	FreCntInit();
	EA = 1;
	ET0 = 1;
	dspbuf[1] = EEPROM_Read(0x00);
	set_rtc();
	OpenFreCnt();
	while(1){
		if(TextKey()){
			 temp1 = GetKey();
			 dspbuf[0] = temp1;
			 EEPROM_Write(0x00, temp1);
		}


		if(SoftTimer1Cnt == 0) SoftTimer1Cnt = 500;
		if(_testbit_(SoftTimer1Over))

		{
			read_rtc();
			dspbuf[2] = time_data[5]/10%10;
			dspbuf[3] = time_data[5]%10;
		}

		temp = getFreCntValue();		
		dspbuf[4] = temp /1000 %10;
		dspbuf[5] = temp /100 %10; 
		dspbuf[6] = temp /10 %10;
		dspbuf[7] = temp %10;
	}
}

