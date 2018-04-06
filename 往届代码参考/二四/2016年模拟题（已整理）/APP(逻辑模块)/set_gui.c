/*2016年模拟题

1.温度检测，温度检测采用 DS18B20 温度传感器，与用户设定的温度
上下限比较，确定当前温度所处的区间。
温度区间 0： 当前温度<TMIN 
温度区间 1： TMIN≤当前温度≤TMAX 
温度区间 2： 当前温度>TMAX 
可设定的最大温度区间：0℃～99℃ 

2.通过3*4矩阵键盘设定系统的工作参数，具体如下
0    1    2
3    4    5
6    7    8
9   设置 清除

3.“设置”按键按下后，进入工作参数设定界面。设定 TMAX为 35 摄氏度，
TMIN为 25 摄氏度为例说明参数设定过程：按下“设置”
按键，然后依次按下数字按键“3” “5” “2” “5”，再次按下“设
置”按键，完成参数设定，并退出参数设定界面。在输入过程中，按下“清除”按
键，将清除当前输入数据，若设定工作参数错误，如 TMAX<TMIN，L2 常亮，修正错误设定并保存参数后，L2 熄灭。

4.
执行机构由指示灯 L1 和继电器组成，用于报警和连接外部高低温执行机构。 
3.1 实时温度处在温度区间 0，继电器关闭，指示灯 L1 以 0.8 秒为间隔闪烁； 
3.2 实时温度处在温度区间 1，继电器关闭，指示灯 L1 以 0.4 秒为间隔闪烁； 
3.3 实时温度处在温度区间 2，继电器打开，指示灯 L1 以 0.2 秒为间隔闪烁。 

5.系统默认的温度上限（TMAX）为 30℃，温度下限(TMIN)为 20℃，可以通过矩阵键盘
修改。

*/
#include "stc_config.h"
#include "set_gui.h"

#include "display.h"
#include "key.h"


//缓冲区空间
uchar buffer_blank[4] = {0};
uchar buffer_upper = 0;

//注意事项：
//		上限
void addBufferData(uchar input){
	if(buffer_upper != 4){
		buffer_blank[buffer_upper] = input;
		buffer_upper++;
	}
}

//注意事项：NULL
void delBufferDate(){
	uchar i;
	for(i = 0; i < 4; i++){
		buffer_blank[i] = 0;
	}
	buffer_upper = 0;
}

//转换发送
//步骤：数据转换，数据判断，数据传送
//注意事项：上下限，数据处理有没有符合要求
uchar transBufferDate(){
	uchar output = 0;
	uchar temp_upper = 0;
	uchar temp_lower = 0;
	
	//数据转化
	temp_upper = buffer_blank[0] *10 +  buffer_blank[1];
	temp_lower = buffer_blank[2] *10 +  buffer_blank[3];
	
	//如果没有达到4个位，则output = 1；
	if(buffer_upper != 4){
		output = 1;
	}
	//上限不大于下限,则output = 2；
	else if(temp_upper <= temp_lower){
		output = 2;
	}
	//如果数据正常，则传给上下限
	else{
		output = 0;
		sys_temp_upper = temp_upper;
		sys_temp_lower = temp_lower;
	}
	
	return output;
}




void set_gui_funtion(void){
	uchar temp_key = 0xff;
	uchar temp_buffer = 0;
	uchar i;
	while(1){
	//事件
	//1.显示

		dspbuf[0] = 11;
		dspbuf[1] = buffer_blank[0];
		dspbuf[2] = buffer_blank[1];	
		dspbuf[5] = 11;
		dspbuf[6] = buffer_blank[2];
		dspbuf[7] = buffer_blank[3];		

		//跳转判断+事件处理
		temp_key = scanKey();
		if(temp_key != 0xff){
			//缓冲区输入
			//数据输入
			if(temp_key!= 16 && temp_key != 17){
				addBufferData(temp_key);
			}
			//清除
			if(temp_key == 17){
				delBufferDate();
			}
			
			
			//跳转判断。保存
			if(temp_key == 16){	//按下了设置按键
				
				//如果没有问题，再保存跳转
				temp_buffer = transBufferDate();
				if(temp_buffer == 0){
					ET0 = 0;
					
					sys_led_cache |= 0x02;
					P0 = sys_led_cache;
					
					setLed;
					ET0 = 1;					
					for(i = 0; i < 8; i++){
						dspbuf[i] = 10;
					}
					sys_stat = WORD_GUI;
					break;
				}
				//常亮
				else if(temp_buffer == 2){
					ET0 = 0;
					
					sys_led_cache &= ~(1<<1);
					P0 = sys_led_cache;
					
					
					
					setLed;
					ET0 = 1;
				}

			}
		}		
	}
}

