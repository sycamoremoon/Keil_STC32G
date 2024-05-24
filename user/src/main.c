#include "config.h"
#include "signal_acq.h"

#pragma userclass (near=CEVENT)	
	CEVENT_EXPORT(0,NULL,NULL);
#pragma userclass (near=default)

int main(void)
{	
	uint16 speed = 0;
	int8 answer = 0;
	/******************************************************************/
	ceventInit();
	ceventPost(0);
	Signal_Init();
	icm20602_init();
	Encoder_init();
	motor_init();
	//初始化结束
	/******************************************************************/
	EA =1;
	/******************************************************************/
	
	//主循环

	while(1)
	{	
		Sample_All_Chanel();
		answer = Get_Regularized_Signal_Data();

		printf("answer: %f\n",answer);
		printf("0: %f\n",All_Signal_Data[0]);
		printf("1: %f\n",All_Signal_Data[1]);
		printf("2: %f\n",All_Signal_Data[2]);
		printf("3: %f\n",All_Signal_Data[3]);
		 
		printf("channel 0: %d\n",Get_DMA_ADC_Result(0));
		printf("channel 1: %d\n",Get_DMA_ADC_Result(1));
		printf("channel 2: %d\n",Get_DMA_ADC_Result(2));
		printf("channel 3: %d\n",Get_DMA_ADC_Result(3));

		
		delay_ms(200);
		
	}

}
