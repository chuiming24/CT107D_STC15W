#ifndef COMMON_H
#define COMMON_H

//#include <REGX52.H>
#include "STC15F2K60S2.h"

typedef unsigned char uchar;
typedef unsigned int uint;

#define set_misc(val) {P2 = (P2 & 0x1f) | 0xa0 ; P0 = val;}
#define set_led(val) {P2 = (P2 & 0x1f) | 0x80 ; P0 = val;}
#define set_nixie_com(val) {P2 = (P2 & 0x1f) | 0xc0 ; P0 = val;}
#define set_nixie_dat(val) {P2 = (P2 & 0x1f) | 0xe0 ; P0 = val;}
#define set_lock() {P2 = (P2 & 0x1f) | 0x00;}

// 按键
#define KEY_S7 0X7E
#define KEY_S11 0XBE
#define KEY_S15 0XDE
#define KEY_S19 0XEE
#define KEY_S6  0X7D
#define KEY_S10 0XBD
#define KEY_S14 0XDD
#define KEY_S18 0XED
#define KEY_S5 0X7B
#define KEY_S9 0XBB
#define KEY_S13 0XDB
#define KEY_S17 0XEB
#define KEY_S4 0X77
#define KEY_S8 0XB7
#define KEY_S12 0XD7
#define KEY_S16 0XE7

// ******************** 符号常数 ********************
#define TH0_RLD ( -2000 / 256 )
#define TL0_RLD ( -2000 % 256 )
#define TH1_RLD ( -2000 / 256 )
#define TL1_RLD ( -2000 % 256 )


// 数码管段码下标助记符
#define NIXIE_HYPHEN 10
#define NIXIE_BLANK 11

void delay(uint ms);

#endif // COMMON_H