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

	Screen_Init();
	wireless_uart_init();
	
	Signal_Init();
	motor_init();
	Encoder_init();
	iic_init();
	
	EA = 1;

	icm20602_init();
	timer_init();
	
	/******************************************************************/

	/******************************************************************/


	//初始化结束
	/******************************************************************/
	//主循环
	while(1)
	{	
//		Set_Motors(10000,10000);
		Screen_ShowInt(80,0,BLACK,WHITE,Get_DMA_ADC_Result(0));
		Screen_ShowInt(80,16,BLACK,WHITE,Get_DMA_ADC_Result(1));
		Screen_ShowInt(80,32,BLACK,WHITE,Get_DMA_ADC_Result(2));
		Screen_ShowInt(80,48,BLACK,WHITE,Get_DMA_ADC_Result(3));
//		printf("F: %d,",Get_DMA_ADC_Result(0));
//		printf("%d,",Get_DMA_ADC_Result(1));
//		printf("%d,",Get_DMA_ADC_Result(2));
//		printf("%d,",Get_DMA_ADC_Result(3));
//		delay_ms(8);
		//Screen_ShowInt(80,64,BLACK,WHITE,Get_DMA_ADC_Result(0)-Get_DMA_ADC_Result(3));
//		Sample_All_Chanel();
//		printf("%d\n",Get_Regularized_Signal_Data(All_Signal_Data));
//		printf("F: %d,%d,%d,%d,%d\n",Get_DMA_ADC_Result(0),Get_DMA_ADC_Result(1),Get_DMA_ADC_Result(2),Get_DMA_ADC_Result(3),Get_Regularized_Signal_Data(All_Signal_Data));
	}
}
