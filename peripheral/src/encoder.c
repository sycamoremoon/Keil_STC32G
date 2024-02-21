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


//==================================================================================================================

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


//#include "encoder.h"
//#include "intrins.h"
//#include "STC32G_PWM.h"

//#define R_Wheel 	0.03		// 轮子半径
//#define CyclePulses 1500		// 转一圈的脉冲数 

//u8  cnt_H, cnt_L;						//计数值高8位、低8位
//u16 count, newcount;					//计数值、新计数值
//u8  leftmove, rightmove;				//左转标志、右转标志
//float speed;							//通过编码器计算出的速度
//bit numberchange;						//数值改变标志

////-------------------------------------------------------------------------------------------------------------------
////  @brief      编码器初始化函数
////  @param      NULL
////  @return     void
////  @since      v1.0
////  Sample usage:	
////-------------------------------------------------------------------------------------------------------------------
//void Encoder_Config(void)
//{
//	// 可选
//	P0M1 = 0x00;P0M0 = 0x00;		//设置P0口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
//	P1M1 = 0x00;P1M0 = 0x00;		//设置P1口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
//	P2M1 = 0x00;P2M0 = 0x00;		//设置P2口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
//	P3M1 = 0x00;P3M0 = 0x00;		//设置P3口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
//	P4M1 = 0x00;P4M0 = 0x00;		//设置P4口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
//	P5M1 = 0x00;P5M0 = 0x00;		//设置P5口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
//	P6M1 = 0x00;P6M0 = 0x00;		//设置P6口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
//	P7M1 = 0x00;P7M0 = 0x00;		//设置P7口为准双向口模式 //00：准双向口 01：推挽输出 10：高阻输入 11：开漏输出
//	
//	PWMB_ENO = 0x00;					// 配置成 TRGI 的 pin 需关掉 ENO 对应 bit 并配成 input
//	
//	PWMB_PSCRH = 0x00;					//预分频寄存器
//	PWMB_PSCRL = 0x00;
//	
//	PWMB_CCER1 = 0x00;					// 清零CCER，关闭通道
//	PWMB_CCMR1 = 0x21; 					// 通道模式配置为输入，接编码器 , 滤波器 4 时钟
//	PWMB_CCMR2 = 0x21; 					// 通道模式配置为输入，接编码器 , 滤波器 4 时钟
//	PWMB_CCER1 = 0x55; 					// 配置通道使能和极性
//	PWMB_CCER2 = 0x55; 					// 配置通道使能和极性
//	
////	PWMA_SMCR = 0x01; 					// 编码器模式 1
////	PWMA_SMCR = 0x02; 					// 编码器模式 2
//	PWMB_SMCR = 0x03; 					// 编码器模式 3
//	
//	PWMB_IER = 0x02; 					// 使能中断
//	PWMB_CR1 |= 0x01; 					// 使能PWM计数器
//	
//}


////-------------------------------------------------------------------------------------------------------------------
////  @brief      编码器检测函数(读取CNTR值)
////  @param      NULL
////  @return     void
////  @since      v1.0
////  Sample usage:	
////-------------------------------------------------------------------------------------------------------------------
//void Encoder_Check()					//检测编码器计数值
//{
//	newcount = cnt_H * 256 + cnt_L;		//获取新的计数器值
//	if(newcount < count) 				//如果新计数值 小于 计数值
//	{
//		leftmove = 1;					//左转标志置1
//		count = newcount;				//更新计数值
//		numberchange = 1;

//	}
//	else if(newcount > count)			//如果新计数值 大于 计数值
//	{
//		rightmove = 1;					//右转标志置1
//		count = newcount;				//更新计数值
//		numberchange = 1;

//	}
//}

////-------------------------------------------------------------------------------------------------------------------
////  @brief      根据编码器计数计算速度
////  @param      NULL
////  @return     void
////  @since      v1.0
////  Sample usage:	
////-------------------------------------------------------------------------------------------------------------------
//void Encoder_Speed()
//{
//	speed = (count / 5) * (2*PI*R_Wheel / CyclePulses);
//}

////-------------------------------------------------------------------------------------------------------------------
////  @brief      中断服务函数
////  @param      NULL
////  @return     void
////  @since      v1.0
////  Sample usage:	
////-------------------------------------------------------------------------------------------------------------------
//void PWMB_ISR() interrupt 26
//{
//	if (PWMB_SR1 & 0x02)				//捕获1中断标志置1时
//	{
//		cnt_H = PWMB_CCR5H;				//读取PWM计数器高8位		
//		cnt_L = PWMB_CCR5L;				//读取PWM计数器低8位
//		PWMB_SR1 &= ~0X02;				//清零捕获1中断标志
//		Encoder_Check();				//检测编码器计数值
//		Encoder_Speed();				//根据计数值计算速度
//	}
//}
