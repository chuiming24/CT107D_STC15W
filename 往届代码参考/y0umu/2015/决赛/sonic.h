#ifndef _sonic_h_
#define _sonic_h_
//#include "stc15f2k60s2.h"
#include "common.h"
#include <intrins.h>

sbit TX =P1^0;
sbit RX =P1^1; 

void sonic_gen();
uchar distance_measure();

#endif