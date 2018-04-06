#include "stc_config.h"
#include "word_gui.h"

#include "onewire.h"
#include "display.h"
#include "key.h"




void reSetLed(){
	ET0 = 0;
	
	sys_led_cache = (~(sys_led_cache&0x01)) | (sys_led_cache&0xfe);
	P0 = sys_led_cache;
	setLed;
	
	ET0 = 1;
}


void word_gui_funtion(void){
	int temp_temp = 0;
	int temp_range = 0;
	int temp_key = 0;
	char i;
	
	led_count = 0;
	
	//在while中实现事件
	//当出现跳转条件的时候，修改now_sys_stat，并break出来
	//就实现了状态机的跳转
	
	while(1){
		//所谓事件，其实就是功能
		
		//1.温度的读取
		temp_temp =  getTemp();
		//2.区间的判断(上限，下限)
		if(temp_temp > sys_temp_upper){
			temp_range = 2;
		}else if(		 temp_temp <= sys_temp_upper 
							&& temp_temp >= sys_temp_lower){
			temp_range = 1;
		}else if(temp_temp < sys_temp_lower){
			temp_range = 0;
		}
		//3.根据区间进行判断，执行LED与继电器的操作
		if(temp_range == 0 || temp_range == 1){
			ET0 = 0;
			//继电器
			P0 = 0x00;
			setOpen;	
			if(temp_range == 0){
				//初始化
				if(led_usedFlag == 0){
					led_usedFlag = 1;
					led_count = 800;
				}
				
				//检测计数是否到0 
				//(同时也要检测他是否有被初始化，也就是有被使用)
				if(led_count == 0 && led_usedFlag == 1){
					led_usedFlag = 0;
					
					//LED反转
					reSetLed();
				}
				
			}else if(temp_range == 1){
				
				//初始化
				if(led_usedFlag == 0){
					led_usedFlag = 1;
					led_count = 400;
				}
				
				//检测计数是否到0 
				//(同时也要检测他是否有被初始化，也就是有被使用)
				if(led_count == 0 && led_usedFlag == 1){
					led_usedFlag = 0;
					
					//LED反转
					reSetLed();
				}
			}
			
			
			
			ET0 = 1;
		}else if(temp_range == 2){
			ET0 = 0;
			P0 = 0x10;
			setOpen;
			//初始化
			if(led_usedFlag == 0){
				led_usedFlag = 1;
				led_count = 200;
			}
			
			//检测计数是否到0 
			//(同时也要检测他是否有被初始化，也就是有被使用)
			if(led_count == 0 && led_usedFlag == 1){
				led_usedFlag = 0;
				
				//LED反转
				reSetLed();
			}		
			
			ET0 = 1;
		}
		//4.显示更新
		dspbuf[7] = temp_temp%10;
		dspbuf[6] = temp_temp/10%10;
		dspbuf[5] = 10;
		dspbuf[4] = 10;
		dspbuf[3] = 10;
		dspbuf[2] = 11;
		dspbuf[1] = temp_range;
		dspbuf[0] = 11;
		
		
		//跳转判断
		temp_key = scanKey();
		if(temp_key != 0xff){
			if(temp_key == 16){	//按下了设置按键
				led_count = 0;
				led_usedFlag = 0;
				for(i = 0; i < 8; i++){
					dspbuf[i] = 10;
				}
				sys_stat = SET_GUI;
				break;
			}
		}
		
		break;
	}
}