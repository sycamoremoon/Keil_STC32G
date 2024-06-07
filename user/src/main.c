#include "config.h"
#include "signal_acq.h"
#include "gyroscope.h"
#include "encoder.h"
#include "motor.h"
#include "screen.h"
#include "iic.h"

#pragma userclass (near=CEVENT)	
	CEVENT_EXPORT(0,NULL,NULL);
#pragma userclass (near=default)
extern uint16 All_Signal_Data[4];

int main(void)
{	
	/******************************************************************/
	ceventInit();
	ceventPost(0);
	Screen_Init();
	Signal_Init();

	/******************************************************************/
	EA =1;
	/******************************************************************/
	
	Signal_Init();
	iic_init();
	icm20602_init();
//	Encoder_init();
	
	//初始化结束

	/******************************************************************/
	//主循环

	while(1)
	{	

		
	}

}
