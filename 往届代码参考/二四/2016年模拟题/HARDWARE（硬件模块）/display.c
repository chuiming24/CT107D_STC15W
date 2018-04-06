//文件名：display.c
#include "stc_config.h"
#include "display.h"

//数码管变量
uchar table[] = {0xc0,0xf9,0xa4,0xb0,
0x99,0x92,0x82,0xf8,
0x80,0x90,0xff,0xbf};//0-10,mie,-
uchar dspbuf[] = {10, 10, 10, 10, 10, 10, 2, 10};
uchar dspcom = 0;

void display(){
	P0 = 0x00;
	setWei;
	
	P0 = table[dspbuf[dspcom]];
	setDuan;
	
	P0 = (1<<dspcom);
	setWei;
		
	dspcom++;
	if(dspcom == 8){
		dspcom = 0;
	}
}