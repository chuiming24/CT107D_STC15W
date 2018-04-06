

#ifndef __ds18b20_H__
#define __ds18b20_H__


#include "common.h"
#include <absacc.h>
#include <INTRINS.H>

#define SKIP_ROM 0XCC
#define CVT_T 0X44
#define RD_SCRATCH 0XBE
sbit DQ = P1^4;


//???????
void Delay_OneWire(unsigned int t);


//DS18B20?????
bit Init_DS18B20(void);

//??????DS18B20?????
void Write_DS18B20(unsigned char dat);

//?DS18B20??????
unsigned char Read_DS18B20(void);

uchar ds18b20_return_temp();


#endif