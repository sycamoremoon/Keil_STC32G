#include "config.h"
#include "uart.h"
#include "wireless.h"
#include "control.h"
#include "timer.h"
#include "screen.h"
#include "gyroscope.h"
#include "iic.h"
#include "exti.h"

#pragma userclass (near=CEVENT)	
	CEVENT_EXPORT(0,NULL,NULL);
#pragma userclass (near=default)

int main(void)
{	
	/******************************************************************/
	ceventInit();
	ceventPost(0);

//	Screen_Init();
	wireless_uart_init();
	
//	Signal_Init();
//	motor_init();
//	Encoder_init();
//	iic_init();
	
	EA = 1;
//	
//	icm20602_init();
//	timer_init();
	
	/******************************************************************/

	/******************************************************************/


	//≥ı ºªØΩ· ¯
	/******************************************************************/

	while (1)
	{
		delay_ms(10);
		wireless_PID();
		
	}
}
