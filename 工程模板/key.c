#include <config.h>
#include <key.h>
#include <display.h>

uchar keyPress = 0;
bit key_re;
uchar keyValue = 0;

uchar KeyBuffer[5];
uchar KeySub = 0;
uchar KeyTimeCnt = 10;
uchar KeyTemp = 0xff;

uchar GetKey(){
	uchar date;
	if(KeySub == 0){
		return 0xff;
	}else{
		KeySub--;
		date = KeyBuffer[KeySub];
		return date; 
	}	
}

uchar TextKey(){
	if(KeySub == 0)
		return 0;
	else
		return 1;
}


uchar read_keyboard(void)
{
    static unsigned char col;
    
	P3 = 0xf0; P42 = 1; P44 = 1;

    if((P3 != 0xf0)||(P42 != 0)||(P44 != 0)) //有按键按下
        keyPress++;
	else
		keyPress = 0;  //抖动
    
    if(keyPress == 3)
    {
		keyPress = 0;
		key_re = 1;
		
		if(P44 == 0)			col = 1;
		if(P42 == 0)			col = 2;
		if((P3 & 0x20) == 0)	col = 3;
		if((P3 & 0x10) == 0)	col = 4;
        
        P3 = 0x0F; P42 = 0; P44 = 0;

		if((P3&0x01) == 0)	keyValue = (col-1);
		if((P3&0x02) == 0)	keyValue = (col+3);
		if((P3&0x04) == 0)	keyValue = (col+7);
		if((P3&0x08) == 0)	keyValue = (col+11);
    }
    
	//连续三次检测到按键被按下，并且该按键已经释放
	P3 = 0x0f; P42 = 0; P44 = 0;
	
    if(((key_re == 1) && (P3 == 0x0f))&&(P42 == 0)&&(P44 == 0))
    {
        key_re = 0;
        return keyValue;
    }
    
    return 0xff;  //无按键按下或被按下的按键未被释放 
}

void KeyScan(){
	KeyTimeCnt--;
	if(KeyTimeCnt == 0){
		KeyTimeCnt = 10;
		KeyTemp = read_keyboard();
		if(KeyTemp != 0xff){
			KeyBuffer[KeySub] = KeyTemp;
			KeySub++;			
		}
	}
}