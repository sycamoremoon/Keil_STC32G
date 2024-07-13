#include "config.h"
#include "uart.h"
#include "wireless.h"
#include "control.h"
#include "timer.h"
#include "screen.h"
#include "gyroscope.h"
#include "telemeter.h"
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

	icm20602_init();
	dl1b_init();
	timer_init();
	
	/******************************************************************/

	/******************************************************************/
	P6_MODE_OUT_PP(GPIO_Pin_1);	//测试GET_ANGLE
	P3_MODE_OUT_PP(GPIO_Pin_4);	//蜂鸣器
	P34 = 0;

	//初始化结束
	/******************************************************************/
	//主循环
	while(1)
	{	
		wireless_PID();
//		printf("Speed:%ld,%ld\n",Left_Speed_State.actual, Right_Speed_State.actual);
		printf("state:%ld,%ld,%ld,",output_left,output_right,pid2_output);
		printf("%ld,%ld\n",pid3_output_left,pid3_output_right);
		printf("%ld,%ld\n",Left_Speed_State.actual,Right_Speed_State.actual);
//		printf("adc_state.actual:%ld\n",adc_state.actual);
//		Set_Motors(10000,10000);
//		Screen_ShowInt(0,0,BLACK,WHITE,Get_DMA_ADC_Result(0));
//		Screen_ShowInt(0,16,BLACK,WHITE,Get_DMA_ADC_Result(1));
//		Screen_ShowInt(0,32,BLACK,WHITE,Get_DMA_ADC_Result(2));
//		Screen_ShowInt(0,48,BLACK,WHITE,Get_DMA_ADC_Result(3));
//		Sample_All_Chanel();
//		printf("ADC:%d,%d,%d,%d,%d\n",Get_DMA_ADC_Result(0),Get_DMA_ADC_Result(1),Get_DMA_ADC_Result(2),Get_DMA_ADC_Result(3),Get_Regularized_Signal_Data(All_Signal_Data));

//		Screen_ShowInt(0,64,BLACK,WHITE,Get_Regularized_Signal_Data(All_Signal_Data));
//		printf("%d\n",Get_Regularized_Signal_Data(All_Signal_Data));
//		printf("F: %d,%d,%d,%d,%d\n",Get_DMA_ADC_Result(0),Get_DMA_ADC_Result(1),Get_DMA_ADC_Result(2),Get_DMA_ADC_Result(3),Get_Regularized_Signal_Data(All_Signal_Data));
	}
}
