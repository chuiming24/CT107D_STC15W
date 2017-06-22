#include <config.h>
#include <usart.h>

void main(){
	UartInit();
	ES = 1;		  	//打开串口中断
	EA = 1;			//打开总中断
	strcpy(up, "QTAT");		   //设定识别码前缀内容
	upLoog = 4;				   //设定识别码前缀长度
	strcpy(down, "QQQ");		   //设定识别码后缀内容
	downLoog = 3;				   //设定识别码后缀长度
	while(1){
	 	if(New_rec == 1){
			New_rec = 0;
			SendString(ustData);
		}
	}
}