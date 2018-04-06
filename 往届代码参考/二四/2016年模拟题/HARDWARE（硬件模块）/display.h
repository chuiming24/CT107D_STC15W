//ÎÄ¼þÃû£ºdisplay.h
#ifndef _DISPLAY_H_
#define _DISPLAY_H_


#define setWei P2=((P2&0x1f)|0xC0);_nop_();_nop_();P2=(P2&0x1f);
#define setDuan P2=((P2&0x1f)|0xE0);_nop_();_nop_();P2=(P2&0x1f);

extern uchar dspbuf[]; //huanchongqu
void display();		//xianshihanshu

#endif