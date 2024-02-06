#include "pwm.h"


#define PWM_PSCR 0
#define PWM_ARR  2211
#define PWM_CCR  553

//========================================================================
// 函数名称: PWMA_Init
// 函数功能: p60端口输出一个PWM信号
// 入口参数: 无
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023
// 当前作者: 
// 其他备注: 
//输出频率   = sysclk（22.1184mhz）/（pscr+1）/(arr+1) = 22118400 / 1 / （ 999+1） = 22118.4hz = 22.1184khz 
//输出占空比 = ccr/( arr+1 )*100% = 500/（ 999+1） = 50%
//========================================================================
void PWMA_Init(void)			//PWMA的初始化
{
	//1.时钟源选择  这里不改，默认22.1184mhz
	
	//2.时基单元选择
	PWMA_PSCRH = (u8)(PWM_PSCR>>8);				//1分频
	PWMA_PSCRL = (u8)(PWM_PSCR);
	
	PWMA_ARRH  = (u8)(PWM_ARR>>8);
	PWMA_ARRL  = (u8)(PWM_ARR);
	
	PWMA_CCR1H  = (u8)(PWM_CCR>>8);
	PWMA_CCR1L  = (u8)(PWM_CCR);	
	
	
	//3.输入输出的引脚配置
	PWMA_ENO = 0X00;			
	PWMA_ENO |= 0X01;			//使能PWM1p通道输出
	PWMA_ENO |= 0X02;			//使能PWM1N通道输出
	
	PWMA_PS =0x00;				//选择PWM1从P60引脚输出
	PWMA_PS |= (2<<0);	
	
	//4.输入输出模式设置
	PWMA_CCER1 = 0X00;
	PWMA_CCMR1 = 0X68;			//pwm模式1 配置置位输出
	PWMA_CCER1 = 0x05;			//开启输出比较通道
//	PWMA_CCER1 |= 0x08;			//
	
	PWMA_DTR = 175;				//10us死区时间
	
	PWMA_BKR = 0X80;			//使能主输出
	
	PWMA_CR1 |= 0X01;			//启动计数器
}

//========================================================================
// 函数名称: PWMA1_Set
// 函数功能: 单路pwm输出
// 入口参数: @pwm ： 0:停止   >0: pwmxp输出pwm  <0: pwmxn输出pwm  
// 函数返回: 无
// 当前版本: VER1.0
// 修改日期: 2023
// 当前作者:
// 其他备注: PWMA1_Set(0);  停止    PWMA1_Set(100); 正转    PWMA1_Set(-100); 反转 
//========================================================================

void PWMA1_Set(int pwm)
{
	if( pwm==0 )		//电机停止
	{
		//1p 1n均为低电平
		PWMA_CCR1H = 0;
		PWMA_CCR1L = 0;
		PWMA_ENO &= 0XFC;		//关闭1p 1n通道
		PWM1P = 0;
		PWM1N = 0;
	}
	else if( pwm>0 )		//电机正转
	{
		if( pwm >(PWM_ARR+1) )
			pwm = (PWM_ARR+1);
		
		PWMA_CCR1H = (u8)(pwm>>8);
		PWMA_CCR1L = (u8)(pwm);	
		PWMA_ENO &= 0XFC;		//关闭1p 1n通道
		PWMA_ENO |= 0X01;			//使能PWM1p通道输出
		PWM1N = 0;
	}
	else if( pwm<0 )		//电机反转
	{
		pwm = -pwm;
		if( pwm >(PWM_ARR+1) )
			pwm = (PWM_ARR+1);
		
		PWMA_CCR1H = (u8)(pwm>>8);
		PWMA_CCR1L = (u8)(pwm);	
		PWMA_ENO &= 0XFC;		//关闭1p 1n通道
		PWMA_ENO |= 0X02;			//使能PWM1n通道输出
		PWM1P = 0;	
	}	
}

void PWMA_Init(void)			//PWMA的初始化
{
	//时钟源选择  这里不改，默认22.1184mhz
	
	//时基单元选择
	PWMA_PSCRH = 0;				//1分频
	PWMA_PSCRL = 0;
	
	PWMA_ARRH  = (u8)(PWM_ARR>>8);
	PWMA_ARRL  = (u8)(PWM_ARR);
	
	PWMA_CCR1H  = (u8)(PWM_CCR>>8);
	PWMA_CCR1L  = (u8)(PWM_CCR);	
	
	
	//输入输出的引脚配置
	PWMA_ENO = 0X00;			//使能PWM1n通道输出
	PWMA_ENO |= 0X02;
	
	PWMA_PS =0x00;				//选择PWM1从P60引脚输出
	PWMA_PS |= (2<<0);	
	
	//输入输出模式设置
	PWMA_CCER1 = 0X00;
	PWMA_CCMR1 = 0X68;			//pwm模式1 配置置位输出
	PWMA_CCER1 = 0x04;			//开启输出比较通道
	
	PWMA_BKR = 0X80;			//使能主输出
	
	PWMA_CR1 |= 0X01;			//启动计数器
}

void PWMA_Init(void)			//PWMA的初始化
{
	//1.时钟源选择  这里不改，默认22.1184mhz
	
	//2.时基单元选择
	PWMA_PSCRH = 0;				//1分频
	PWMA_PSCRL = 0;
	
	PWMA_ARRH  = (u8)(PWM_ARR>>8);
	PWMA_ARRL  = (u8)(PWM_ARR);
	
	PWMA_CCR1H  = (u8)(PWM_CCR>>8);
	PWMA_CCR1L  = (u8)(PWM_CCR);	
	
	
	//3.输入输出的引脚配置
	PWMA_ENO = 0X00;			//使能PWM2p通道输出
	PWMA_ENO |= 0X04;
	
	PWMA_PS =0x00;				//选择PWM2从P62引脚输出
	PWMA_PS |= (2<<2);	
	
	//4.输入输出模式设置
	PWMA_CCER1 = 0X00;
	PWMA_CCMR2 = 0X68;			//pwm模式1 配置置位输出
	PWMA_CCER1 = 0x10;			//开启输出比较通道
	
	PWMA_BKR = 0X80;			//使能主输出
	
	PWMA_CR1 |= 0X01;			//启动计数器
}


