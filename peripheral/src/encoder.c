#include "encoder.h"
#include "intrins.h"
#include "STC32G_Timer.h"
#include "timer.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      2个编码器对应的定时器及GPIO初始化
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:	可以修改初始化的GPIO口
//-------------------------------------------------------------------------------------------------------------------
void Encoder_init()
{
	P0_MODE_IO_PU(GPIO_Pin_4);			//P04初始化为双向口
	ctimer_count_init(CTIM3_P04);		//定时器3初始化作为外部计数

	P0_MODE_IO_PU(GPIO_Pin_6);			//P06初始化为双向口
	ctimer_count_init(CTIM4_P06);		//定时器4初始化作为外部计数
	
	P2_MODE_IO_PU(GPIO_Pin_0|GPIO_Pin_2);			//P20,P22初始化为双向口记录方向
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取左编码器的计数值
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:	调用函数之后，直接使用变量Encoder1_cnt, speedL
//-------------------------------------------------------------------------------------------------------------------
uint16 get_EncoderL()
{
	int16 EncoderL_cnt;
	
	EncoderL_cnt = (int16)ctimer_count_read(CTIM3_P04);
	ctimer_count_clean(CTIM3_P04);					// 清除计数

	return EncoderL_cnt;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取右编码器的计数值
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:	调用函数之后，直接使用变量Encoder2_cnt, speedR
//-------------------------------------------------------------------------------------------------------------------
uint16 get_EncoderR()
{
	uint16 EncoderR_cnt;

	EncoderR_cnt = (int16)ctimer_count_read(CTIM4_P06);
	ctimer_count_clean(CTIM4_P06);			// 清除计数
	
	return EncoderR_cnt;
}
