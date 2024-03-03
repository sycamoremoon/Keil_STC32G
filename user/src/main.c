#include "config.h"
#include "screen.h"
#include "gyroscope.h"


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
	//初始化结束
	/******************************************************************/
	EA =1;
	/******************************************************************/
	mpu6050_init();
	//主循环
	while(1)
	{	
		mpu6050_get_accdata();
		mpu6050_get_gyro();

		delay_ms(50);
		printf("mpu6050_acc_x:%d\nmpu6050_acc_y:%d\nmpu6050_acc_z:%d\n\n\n",mpu6050_acc_x,mpu6050_acc_y,mpu6050_acc_z);
	}

}
