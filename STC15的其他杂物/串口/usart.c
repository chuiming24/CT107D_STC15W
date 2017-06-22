#include <stc15_usart.h>

bit Send_ed = 1;   				//发送数据结束标志位
bit New_rec = 0;				//收到数据标志位
char xdata Rec_data[25] = {0};	//缓冲区
char Rec_data_down = 0;			//缓冲区下标
char ustData[25] = {0};		  	//数据
char ustData_down = 0;			//数据下标

char ustStat = 0;			//判断状态，0为识别码判断，1为接收状态，2为结束码判断
char data up[10] = "AT";  //前缀
char data upLoog = 2;	  //前缀长度
//后缀
char down[10] = "QQQQQ"; //后缀
char ustDataDownDown = 0;	//后缀识别下表
char data downLoog = 3;	  //后缀长度

void SendString(char *s){
	while(*s){
		if(SendData(*s) == 1){
			s++;
		}
	}
}
void revData(char a){
	Rec_data[Rec_data_down] = a;
	switch(ustStat){
		case 0:
			if(Rec_data_down < upLoog && Rec_data[Rec_data_down] == up[Rec_data_down]){
				Rec_data_down++;
			}else{
				Rec_data_down = 0;	
			}
			if(Rec_data_down == upLoog){
				ustStat = 1;
			}
			break;
		case 1:
			//判断是不是后缀
			if(Rec_data[Rec_data_down] == down[ustDataDownDown]){
				ustDataDownDown++;
			}else{
				ustDataDownDown = 0;
			}
			if(ustDataDownDown == downLoog){	//确定是后识别码正确
				//清零缓冲区数据
				ustDataDownDown = 0;
				Rec_data_down = 0;
				ustStat = 0;
				New_rec = 1;
				//处理接受数据变量，去掉后面的识别码
				ustData[ustData_down-2] = '\0';
				//清零数据区数据
				ustData_down = 0;
				break;				
			}
			//将缓冲区的数据丢入数据区，并且下标进位
			ustData[ustData_down] = Rec_data[Rec_data_down];
			ustData_down++;
			Rec_data_down++;
			break;
	}
}