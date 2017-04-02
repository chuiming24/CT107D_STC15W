#ifndef _EXTI_H_
#define _EXTI_H_

extern uint SoftTimer0Cnt;
extern bit SoftTimer0Over;

extern uint SoftTimer1Cnt;
extern bit SoftTimer1Over;

extern uint SoftTimer2Cnt;
extern bit SoftTimer2Over;

extern uint SoftTimer3Cnt;
extern bit SoftTimer3Over;

extern uint SoftTimer4Cnt;
extern bit SoftTimer4Over;


void Timer0Init(void);
void Timer1Init(void);
uint getFreCnt();
#endif