#include "config.h"
#include "iic.h"
#include "telemeter.h"
#pragma userclass (near=CEVENT)	
	CEVENT_EXPORT(0,NULL,NULL);
#pragma userclass (near=default)
//
int main(void)
{	
	WTST = 0;		//设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
	EAXSFR();		//扩展SFR(XFR)访问使能 
	CKCON = 0;      //提高访问XRAM速度
	
	/******************************************************************/
	ceventInit();
	ceventPost(0);
	iic_init();
	dl1b_init();
	//初始化结束
	/******************************************************************/
	EA =1;
	/******************************************************************/
	//主循环
	while(1)
	{	
		printf("distance : %d\n",dl1b_get_distance());
		delay_ms(50);
	}

}
