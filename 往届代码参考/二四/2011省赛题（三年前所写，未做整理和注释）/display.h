#ifndef _DSPLAY_H_
#define _DSPLAY_H_

#define setY3() P2 = ((P2&0x1f) | 0x60); P2=P2&0x1f	//8255的CE
#define setY4() P2 = ((P2&0x1f) | 0x80); P2=P2&0x1f	//LED
#define setY5() P2 = ((P2&0x1f) | 0xA0); P2=P2&0x1f	//UNL2003
#define setY6() P2 = ((P2&0x1f) | 0xC0); P2=P2&0x1f	//数码管位选
#define setY7() P2 = ((P2&0x1f) | 0xE0); P2=P2&0x1f	//数码管段选


extern uchar dspbuf[];
void display(void);
void setup(uchar number);
void setdown(uchar number);
void settemp(uchar number);

#endif