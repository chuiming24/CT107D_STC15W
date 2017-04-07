#ifndef __WAVE_H_
#define __WAVE_H_

sbit TX = P1^0;
sbit RX = P1^1;

uint Distance_Get(void);
void Send_Wave(void);
void Init_Timer1(void);
void Delay12us();		//@11.0592MHz

#endif
