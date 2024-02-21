#include "encoder2.h"
#include "intrins.h"
#include "STC32G_Timer.h"
#include "timer.h"


#define R_Wheel 	0.03		// 轮子半径
#define CyclePulses 512		// 转一圈的脉冲数 

u16 Encoder1_cnt, Encoder2_cnt;
float speedL, speedR;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      2个编码器对应的GPIO初始化
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:	需要修改要初始化的GPIO口
//-------------------------------------------------------------------------------------------------------------------
void Encoder_init()
{
	Encoder1_init();
	Encoder2_init();
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取编码器1的计数值、速度
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:	调用函数之后，直接使用变量Encoder1_cnt, speedL
//-------------------------------------------------------------------------------------------------------------------
void Encoder1()
{
	// pit_timer_ms(TIM_0, 10);
	Encoder1_cnt = ctimer_count_read(CTIM3_P04);
	speedL = (Encoder1_cnt / 5) * (2*PI*R_Wheel / CyclePulses);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取编码器2的计数值、速度
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:	调用函数之后，直接使用变量Encoder2_cnt, speedR
//-------------------------------------------------------------------------------------------------------------------
void Encoder2()
{
	//pit_timer_ms(TIM_0, 10);
	Encoder2_cnt = ctimer_count_read(CTIM4_P06);
	speedR = (Encoder2_cnt / 5) * (2*PI*R_Wheel / CyclePulses);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器1对应的GPIO口初始化(内部调用，用户无需关心)
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void Encoder1_init()
{
	GPIO_InitTypeDef	GPIO_InitStructure;					//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_HIGH | GPIO_Pin_4;	//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_PullUp;					//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);				//初始化
	
	ctimer_count_init(CTIM3_P04);							//定时器3初始化作为外部计数
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器2对应的GPIO口初始化(内部调用，用户无需关心)
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void Encoder2_init()
{
	GPIO_InitTypeDef	GPIO_InitStructure;					//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_HIGH | GPIO_Pin_6;	//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_PullUp;					//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);				//初始化
	
	ctimer_count_init(CTIM4_P06);							//定时器4初始化作为外部计数
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      Timer3/4与GPIO初始化
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:	需要修改要初始化的GPIO口
//-------------------------------------------------------------------------------------------------------------------
//void Timer_config(u8 TimerX)
//{
//	TIM_InitTypeDef		TIM_InitStructure;					//结构定义
//	GPIO_InitTypeDef	GPIO_InitStructure;					//结构定义
//	
//	if(TimerX == Timer3)
//	{
//		GPIO_InitStructure.Pin  = GPIO_Pin_HIGH | GPIO_Pin_4;	//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
//		GPIO_InitStructure.Mode = GPIO_PullUp;					//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
//		GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);				//初始化
//		
//		TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
//		TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_Ext;			//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
//		TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
//		TIM_InitStructure.TIM_Value     = (u16)(65536UL - (MAIN_Fosc / 100000UL));		//初值,
//		TIM_InitStructure.TIM_PS        = 0;					//8位预分频器(n+1), 0~255
//		TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
//		Timer_Inilize(Timer0,&TIM_InitStructure);				//初始化Timer0	  Timer0,Timer1,Timer2,Timer3,Timer4
//		NVIC_Timer0_Init(ENABLE,Priority_0);					//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
//	}
//	else if(TimerX == Timer4)
//	{
//		GPIO_InitStructure.Pin  = GPIO_Pin_HIGH | GPIO_Pin_6;	//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
//		GPIO_InitStructure.Mode = GPIO_PullUp;					//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
//		GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);				//初始化
//		
//		TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
//		TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_Ext;			//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
//		TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
//		TIM_InitStructure.TIM_Value     = (u16)(65536UL - (MAIN_Fosc / 100000UL));		//初值,
//		TIM_InitStructure.TIM_PS        = 0;					//8位预分频器(n+1), 0~255
//		TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
//		Timer_Inilize(Timer1,&TIM_InitStructure);				//初始化Timer1	  Timer0,Timer1,Timer2,Timer3,Timer4
//		NVIC_Timer0_Init(ENABLE,Priority_0);					//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
//	}
//	
//}
