#define setY3 P2 = ((P2&0x1f) | 0x60); _nop_(); _nop_(); P2=P2&0x1f	//8255的CE
#define setY4 P2 = ((P2&0x1f) | 0x80); _nop_(); _nop_(); P2=P2&0x1f	//LED
#define setY5 P2 = ((P2&0x1f) | 0xA0); _nop_(); _nop_(); P2=P2&0x1f	//UNL2003
#define setY6 P2 = ((P2&0x1f) | 0xC0); _nop_(); _nop_(); P2=P2&0x1f	//数码管位选
#define setY7 P2 = ((P2&0x1f) | 0xE0); _nop_(); _nop_(); P2=P2&0x1f	//数码管段选


code uchar table[] = {0xc0,0xf9,0xa4,0xb0,
					 0x99,0x92,0x82,0xf8,
					 0x80,0x90,0x88,0x83,
					 0xc6,0xa1,0x86,0x8e,
					 0x40,0x79,0x24,0x30,
					 0x19,0x12,0x02,0x78,
					 0x00,0x10,0x08,0x03,
					 0x46,0x21,0x06,0x0e,
					 0xff,0xbf};
uchar dspbuf[] = {32, 32, 32, 32, 32, 32, 32, 32};
uchar dspcom = 0;


void display(void){
  	P0 = 0x00;
	setY6;
  	P0 = table[dspbuf[7-dspcom]];
	setY7;
  	P0 = (1<<dspcom);
	setY6;

	dspcom++;
	if(dspcom == 8){
		dspcom = 0;
	}		
}

void setNum(u32 a, uchar isNoHignZero){
	uchar i = 7;
	dspbuf[0] = a %10;
	dspbuf[1] = a /10 %10;
	dspbuf[2] = a /100 %10;
	dspbuf[3] = a /1000 %10;
	dspbuf[4] = a /10000 %10;
	dspbuf[5] = a /100000 %10;
	dspbuf[6] = a /1000000 %10;
	dspbuf[7] = a /10000000 %10;
	if(isNoHignZero == 1){
		while(i > 0){
			if(dspbuf[i] == 0){
				dspbuf[i] = 32;
			}else{
				break;
			}
			i--;
		}
	}
}

void setDsp1(uchar a){
	dspbuf[0] = a %10;
	dspbuf[1] = a /10 %10;
	dspbuf[2] = 33;
	dspbuf[3] = 32;
	dspbuf[4] = 32;
	dspbuf[5] = 32;
	dspbuf[6] = 32;
	dspbuf[7] = 32;
}

void setDsp2(uchar hour, min, sec){
	dspbuf[0] = sec %10;
	dspbuf[1] = sec /10 %10;
	if(sec % 2){
		dspbuf[2] = 33;
		dspbuf[5] = 33;		
	}else{
		dspbuf[2] = 32;
		dspbuf[5] = 32;			
	}
	dspbuf[3] = min %10;
	dspbuf[4] = min /10 %10;

	dspbuf[6] = hour %10;
	dspbuf[7] = hour /10 %10;
}

void setDsp3(uchar num, temp){
	dspbuf[0] = temp %10;
	dspbuf[1] = temp /10 %10;
	dspbuf[2] = 33;
	dspbuf[3] = 32;
	dspbuf[4] = 32;
	dspbuf[5] = num %10;
	dspbuf[6] = num /10 %10;
	dspbuf[7] = 33;
}


void setLed(uchar i, uchar isOPen){
	TR0 = 0;


	P2=P2&0x1f;
  	if(isOPen == 0){
		P0 = 0xff;
	}else{
		P0 = ~(1<<i);
	}
	setY4;

	TR0 = 1;
}
