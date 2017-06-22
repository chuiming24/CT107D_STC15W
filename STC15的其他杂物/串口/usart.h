#ifndef _USART_H_
#define _USART_H_

extern char data up[10];  		//前缀
extern char data upLoog;	  	//前缀长度

extern char down[10]; 			//后缀
extern char data downLoog;	  	//后缀长度

extern bit New_rec;				//收到数据标志位
extern char ustData[25];		  	//数据

void UartInit(void);		
void SendString(char *s);

#endif