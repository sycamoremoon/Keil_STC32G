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
	
	Signal_Init();
	motor_init();
	Encoder_init();
	iic_init();
	Exti_config();
	EA = 1;

//	icm20602_init();
	timer_init();
	
	/******************************************************************/

	/******************************************************************/

	P3_MODE_OUT_PP(GPIO_Pin_4);	//蜂鸣器
	printf("start\n");
	//初始化结束
	/******************************************************************/
	//主循环
	while(1)
	{	
//		printf("start\n");
		wireless_PID();
//		
//		printf("adc_state.actual:%ld\n",adc_state.actual);
//		Set_Motors(10000,10000);
//		Screen_ShowInt(0,0,BLACK,WHITE,Get_DMA_ADC_Result(0));
//		Screen_ShowInt(0,16,BLACK,WHITE,Get_DMA_ADC_Result(1));
//		Screen_ShowInt(0,32,BLACK,WHITE,Get_DMA_ADC_Result(2));
//		Screen_ShowInt(0,48,BLACK,WHITE,Get_DMA_ADC_Result(3));
//		Sample_All_Chanel();
//		printf("ADC:%d,%d,%d,%d,%d\n",Get_DMA_ADC_Result(0),Get_DMA_ADC_Result(1),Get_DMA_ADC_Result(2),Get_DMA_ADC_Result(3),Get_Regularized_Signal_Data(All_Signal_Data));

//		delay_ms(8);

//		Screen_ShowInt(0,64,BLACK,WHITE,Get_Regularized_Signal_Data(All_Signal_Data));
//		printf("%d\n",Get_Regularized_Signal_Data(All_Signal_Data));
//		printf("F: %d,%d,%d,%d,%d\n",Get_DMA_ADC_Result(0),Get_DMA_ADC_Result(1),Get_DMA_ADC_Result(2),Get_DMA_ADC_Result(3),Get_Regularized_Signal_Data(All_Signal_Data));
	}
}
