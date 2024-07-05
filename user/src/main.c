#include "config.h"
#include "signal_acq.h"
#include "encoder.h"
#include "motor.h"
#include "control.h"
#include "timer.h"
#include "screen.h"
#include "exti.h"

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
	timer_init();
	motor_init();
	Encoder_init();
	Exti_config();
	EA =1;
	/******************************************************************/

	/******************************************************************/
	Screen_Show_String(0,0,BLACK,WHITE,"channel 3:");
	Screen_Show_String(0,16,BLACK,WHITE,"channel 2:");
	Screen_Show_String(0,32,BLACK,WHITE,"channel 1:");
	Screen_Show_String(0,48,BLACK,WHITE,"channel 0:");
	Screen_Show_String(0,64,BLACK,WHITE,"diff :");
	Screen_Show_String(0,80,BLACK,WHITE,"result :");
	
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
		Screen_ShowInt(80,64,BLACK,WHITE,Get_DMA_ADC_Result(0)-Get_DMA_ADC_Result(3));
//		Sample_All_Chanel();
		Screen_ShowInt(80,80,BLACK,WHITE,Get_Regularized_Signal_Data(All_Signal_Data));
//		printf("F: %d,%d,%d,%d,%d\n",Get_DMA_ADC_Result(0),Get_DMA_ADC_Result(1),Get_DMA_ADC_Result(2),Get_DMA_ADC_Result(3),Get_Regularized_Signal_Data(All_Signal_Data));

	}

}
