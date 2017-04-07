#ifndef _UART_H_
#define _UART_H_

extern bit uart_get_flag;
extern char uart_get_data;

void UartInit();
void SendData(char dat);
void SendString(char *s);

#endif
