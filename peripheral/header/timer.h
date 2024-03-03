#include "cevent.h"
#include "STC32G_Timer.h"

#ifndef __TIMER_H__
#define __TIMER_H__


//此枚举定义不允许用户修改
typedef enum    // 枚举ADC通道
{
    CTIM0_P34=0,    
	CTIM1_P35,    	
	CTIM2_P12,     	
	CTIM3_P04,
	CTIM4_P06,
}CTIMN_enum;


//此枚举定义不允许用户修改
typedef enum    // 枚举ADC通道
{
    TIM_0,    
	TIM_1,    	
	TIM_2,     	
	TIM_3,
	TIM_4,
}TIMN_enum;


#define TIM2_CLEAR_FLAG		AUXINTIF &= ~0x01; 
#define TIM3_CLEAR_FLAG		AUXINTIF &= ~0x02; // 清中断标志
#define TIM4_CLEAR_FLAG		AUXINTIF &= ~0x04; // 清中断标志


void   ctimer_count_init(CTIMN_enum tim_n);
void   ctimer_count_clean(CTIMN_enum tim_n);
uint16 ctimer_count_read(CTIMN_enum tim_n);

void pit_timer_ms(TIMN_enum tim_n,uint16 time_ms);


#endif