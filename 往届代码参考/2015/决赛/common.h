#ifndef COMMON_H
#define COMMON_H

//#include <REGX52.H>
#include "STC15F2K60S2.h"

typedef unsigned char uchar;
typedef unsigned int uint;

//#define USE_ABSACC_H

//#ifdef USE_ABSACC_H
//#include <absacc.h>
//#define set_led(value) { XBYTE[0x8000] = value;}
//#define set_nixie_dat(value) { XBYTE[0xe000] = value;}
//#define set_nixie_com(value) { XBYTE[0xc000] = value;}
//#else
//#define set_led(value) {P2 = ( P2 & 0x1f ) | 0x80; P0 = value;}
//#define set_nixie_dat(value) {P2 = ( P2 & 0x1f ) | 0xe0; P0 = value;}
//#define set_nixie_com(value) {P2 = ( P2 & 0x1f ) | 0xc0; P0 = value;}
//#endif  // USE_ABSACC_H

#define set_misc(val) {P2 = (P2 & 0x1f) | 0xa0 ; P0 = val;}
#define set_buzz(val) {P2 = (P2 & 0x1f) | 0xa0 ; P06 = val;}    // 蜂鸣器 1:on, 0:off
#define set_relay(val)  {P2 = (P2 & 0x1f) | 0xa0 ; P04 = val;}   // 1:off, 0:on
#define set_led(val) {P2 = (P2 & 0x1f) | 0x80 ; P0 = val;}
#define set_nixie_com(val) {P2 = (P2 & 0x1f) | 0xc0 ; P0 = val;}
#define set_nixie_dat(val) {P2 = (P2 & 0x1f) | 0xe0 ; P0 = val;}
#define set_lock() {P2 = (P2 & 0x1f) | 0x00;}

// 按键
#define KEY_S7 0X0E

#define KEY_S6  0X0D

#define KEY_S5 0X0B

#define KEY_S4 0X07


// ******************** 符号常数 ********************
#define TH0_RLD ( 0 )
#define TL0_RLD ( 0 )
#define TH1_RLD ( -2000 / 256 )
#define TL1_RLD ( -2000 % 256 )

#define T2H_RLD ( -12 / 256)
#define T2L_RLD ( -12 % 256)

// 数码管段码下标助记符
#define NIXIE_HYPHEN 10
#define NIXIE_BLANK 11

// misc助记符
#define MISC_BUZZ_RELAY 0xef
#define MISC_nBUZZ_RELAY 0xaf
#define MISC_nBUZZ_nRELAY 0xbf
#define MISC_BUZZ_nRELAY 0xff

// 无按键按下时P3口具有的电平
#define KEY_NULL 0x0f

void delay(uint ms);

#endif // COMMON_H