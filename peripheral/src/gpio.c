#include "gpio.h"

#pragma userclass (near=CEVENT)	
//	CEVENT_EXPORT(0,GPIO_config,NULL);
#pragma userclass (near=default)

void GPIO_config(void)
{
	P3_MODE_IO_PU(GPIO_Pin_All);		//P3.0~P3.7 设置为准双向口
	P3_PULL_UP_ENABLE(GPIO_Pin_All);//P3 口内部上拉电阻使能
	P2_MODE_IO_PU(GPIO_Pin_All);	//P4.6,P4.7 设置为准双向口
}
