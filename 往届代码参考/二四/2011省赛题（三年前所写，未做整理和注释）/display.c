#include <config.h>
#include <display.h>


code uchar table[] = {0xc0,0xf9,0xa4,0xb0,
					 0x99,0x92,0x82,0xf8,
					 0x80,0x90,0x88,0x83,
					 0xc6,0xa1,0x86,0x8e,
					 0x40,0x79,0x24,0x30,
					 0x19,0x12,0x02,0x78,
					 0x00,0x10,0x08,0x03,
					 0x46,0x21,0x06,0x0e,
					 0xff,0xbf};
uchar dspbuf[] = {32, 32, 32, 32, 32, 32, 32, 0};
uchar dspcom = 0;

/*/////////////////////////////////////////////
函数名：	display
输入值：	无
返回值：	无
备注  ：	动态显示函数，2ms运行一次最为稳定
**********************************************/
void display(void){
  	P0 = 0x00;
	setY6();
  	P0 = table[dspbuf[dspcom]];
	setY7();
  	P0 = (1<<dspcom);
	setY6();

	dspcom++;
	if(dspcom == 8){
		dspcom = 0;
	}		
}



void setup(uchar number){
	dspbuf[0] =	number /10 %10;
	dspbuf[1] = number %10;
}

void setdown(uchar number){
	dspbuf[2] =	number /10 %10;
	dspbuf[3] = number %10;
}

void settemp(uchar number){
	dspbuf[6] =	number /10 %10;
	dspbuf[7] = number %10;
}