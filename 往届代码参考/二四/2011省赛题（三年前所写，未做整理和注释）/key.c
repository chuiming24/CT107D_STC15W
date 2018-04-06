#include <config.h>
#include <key.h>

uchar keyValue = 0xff;
uchar keyPress = 0;
uchar key_re = 0;

uchar scanKey(){
	if(!key_re)
	{
		if(P30 == 0 || P31 == 0 || P32 == 0 || P33 == 0){
			keyPress++;
		}else{
			keyPress = 0;
		}
		if(keyPress == 3){
			keyPress = 0;
			key_re = 1;
			
			if(P30 == 0)	keyValue = 1;
			if(P31 == 0)	keyValue = 2;
			if(P32 == 0)	keyValue = 3;
			if(P33 == 0)	keyValue = 4;
        	return keyValue;			
		}
    }else if(key_re == 1 &&	(P3 & 0x0f) == 0x0f)
    {
        key_re = 0;

    }

	return 0xff;
}
