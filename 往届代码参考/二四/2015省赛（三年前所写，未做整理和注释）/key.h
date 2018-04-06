uchar keyflag = 0;
uchar keyNum = 0;
void Delay10ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 117;
	j = 184;
	do
	{
		while (--j);
	} while (--i);
}

void scanKey(){
	if(P30 == 0){
		Delay10ms();
		if(P30 == 0){
			keyflag = 1;
			keyNum = 1;
			if(stat == 2){
				stat = 0;
				setLed(0, 0);
			}
		}
		while(!P30);	
	}

	if(P31 == 0){
		Delay10ms();
		if(P31 == 0){
			keyflag = 1;
			keyNum = 2;
			if(stat == 2){
				stat2_Cnt++;
				if(stat2_Cnt == 10){
					stat2_Cnt = 0;
				}
				setLed(0, 0);
			}
		}
		while(!P31);	
	}

	if(P32 == 0){
		Delay10ms();
		if(P32 == 0){
			keyflag = 1;
			keyNum = 3;
			if(stat == 0){
				stat = 1;
				stat1_ds18b20NumCount = 0;					 
			}
		}
		while(!P32);	
	}

	if(P33 == 0){
		Delay10ms();
		if(P33 == 0){
			keyflag = 1;
			keyNum = 4;
			if(stat == 0){
				if(++stat0_setSecNumCount == 4) stat0_setSecNumCount = 0;
					 
			}
			if(stat == 1){
				 stat = 2;
				 setLed(0, 1);
			}
		}
		while(!P33);	
	}
}