#include "config.h"
#include "frecounter.h"

bit OpenFreCntFlag = 1;
uint FreCntTimeCnt = 1000;
uint FreCntValue = 0;


void OpenFreCnt(){
	OpenFreCntFlag = 1;
}
void CloseFreCnt(){
	OpenFreCntFlag = 0;
}

void FreCntInit(void){
	TMOD |= 0x50;
	TH1 = 0;
	TL1 = 0;
	TR1 = 0;
}

uint getFreCntValue(){
	if(OpenFreCntFlag == 0){
		return 0xffff;
	}else{
		return FreCntValue;
	}
}

uint getFreCnt(){
	uint temp;
	TR1 = 0;
	temp = ((uint)TH1 *256 + TL1);
	TH1 = 0;
	TL1 = 0;

	return temp;
}

void FreCntScan(){
	if(OpenFreCntFlag){
		TR1 = 1;
		FreCntTimeCnt--;
		if(FreCntTimeCnt == 0){
			FreCntTimeCnt = 1000;
			FreCntValue = getFreCnt();
		}
	}
}