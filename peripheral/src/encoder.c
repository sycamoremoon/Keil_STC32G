#include "encoder.h"
#include "intrins.h"
#include "STC32G_Timer.h"
#include "timer.h"

#define R_Wheel 		0.03	// 轮子半径
#define CyclePulses 	512		// 转一圈的脉冲数 
#define Timer_Period	10		// 定时器周期

u16 Encoder1_cnt, Encoder2_cnt;
float speedL, speedR;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      2个编码器对应的定时器及GPIO初始化
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
//  @brief      读取2个编码器的计数值和速度
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:	调用该函数后，直接使用变量Encoder1_cnt, speedL, Encoder2_cnt, speedR
//-------------------------------------------------------------------------------------------------------------------
void Encoder()
{
	Encoder1();
	Encoder2();
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取编码器1的计数值、速度	(内部调用，用户无需关心)
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:	调用函数之后，直接使用变量Encoder1_cnt, speedL
//-------------------------------------------------------------------------------------------------------------------
void Encoder1()
{
	ctimer_count_clean(CTIM3_P04);			// 清除计数
	//pit_timer_ms(TIM_0, Timer_Period);
	Encoder1_cnt = ctimer_count_read(CTIM3_P04);
	speedL = (Encoder1_cnt / CyclePulses * (2*PI*R_Wheel ) / Timer_Period);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取编码器2的计数值、速度	(内部调用，用户无需关心)
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:	调用函数之后，直接使用变量Encoder2_cnt, speedR
//-------------------------------------------------------------------------------------------------------------------
void Encoder2()
{
	ctimer_count_clean(CTIM4_P06);			// 清除计数
	pit_timer_ms(TIM_0, Timer_Period);
	//Encoder2_cnt = ctimer_count_read(CTIM4_P06);
	speedR = (Encoder2_cnt / CyclePulses * (2*PI*R_Wheel ) / Timer_Period);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器1对应的GPIO口初始化	(内部调用，用户无需关心)
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
	GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);				//初始化P04
	
	ctimer_count_init(CTIM3_P04);							//定时器3初始化作为外部计数
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器2对应的GPIO口初始化	(内部调用，用户无需关心)
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
	GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);				//初始化P06
	
	ctimer_count_init(CTIM4_P06);							//定时器4初始化作为外部计数
}
