#include "config.h"
#include "signal_acq.h"
#include "encoder.h"
#include "motor.h"
#include "control.h"
#include "timer.h"

#pragma userclass (near=CEVENT)	
	CEVENT_EXPORT(0,NULL,NULL);
#pragma userclass (near=default)
extern uint16 All_Signal_Data[4];

int main(void)
{	
	/******************************************************************/
	ceventInit();
	ceventPost(0);
//	Screen_Init();
	Signal_Init();
	timer_init();
	motor_init();
	Encoder_init();
	/******************************************************************/
	EA =1;
	/******************************************************************/
	
	
	//初始化结束

	/******************************************************************/
	//主循环

	while(1)
	{	
		//Set_Motors(300,300);
	}

}
