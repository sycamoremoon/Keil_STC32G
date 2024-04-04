#include "config.h"
#include "timer.h"
#include  "iic.h"
#include "gyroscope.h"
#include "motor.h"
#include "encoder.h"

#pragma userclass (near=CEVENT)	
	CEVENT_EXPORT(0,NULL,NULL);
#pragma userclass (near=default)

int main(void)
{	
	int speed = 0;
	/******************************************************************/
	ceventInit();
	ceventPost(0);
	iic_init();
	icm20602_init();
	pit_timer_ms(TIM_0, 10);

	//初始化结束
	/******************************************************************/
	EA =1;
	/******************************************************************/
	//主循环
	while(1)
	{	
//		printf("gyro_x : %d,gyro_y: %d,gyro_z:%d\n",gyro_gyro_x,gyro_gyro_y,gyro_gyro_z);
		printf("Left encoder : %d, Right encoder : %d\n",get_EncoderL(),get_EncoderR());
		for (speed = 0;speed <= MAXSPEED; speed += 10)
		{
			Set_Lmotor_Speed(speed);
			Set_Rmotor_Speed(speed);
			delay_ms(100);
		}

	}

}
