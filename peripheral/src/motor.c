#include "config.h"
#include "motor.h"

#define	PWM_Peri			65535	// PWM周期设定
#define Lmotor_direction	P35		// 左电机方向控制引脚
#define Rmotor_direction	P70		// 右电机方向控制引脚

Motor motor;				// 定义电机结构体
PWMx_Duty pwmA_duty;		// 定义PWMA结构体

//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化电机
//  @param      PWM	
//  @return     void					
//  @since      v1.0
//  Sample usage:	motor_init()
//-------------------------------------------------------------------------------------------------------------------
void motor_init(void)
{

	// 初始化 PWMx
	PWMx_InitDefine PWMx_InitStructure;							// 定义PWM初始化结构体
	
	pwmA_duty.PWM3_Duty = 0;									// 初始化PWM3 PWM4个通道的占空比
	pwmA_duty.PWM4_Duty = 0;
	
	PWMx_InitStructure.PWM_Mode    =	CCMRn_PWM_MODE1;		//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty    = pwmA_duty.PWM3_Duty;		//PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect   = ENO3P | ENO3N;			//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM3, &PWMx_InitStructure);				//初始化PWM3

	PWMx_InitStructure.PWM_Mode    =	CCMRn_PWM_MODE1;		//模式,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty    = pwmA_duty.PWM4_Duty;		//PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect   = ENO4P | ENO4N;			//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM4, &PWMx_InitStructure);				//初始化PWM4

	PWMx_InitStructure.PWM_Period   = PWM_Peri;					//周期时间,   0~65535
	PWMx_InitStructure.PWM_DeadTime = 0;						//死区发生器设置, 0~255
	PWMx_InitStructure.PWM_MainOutEnable= ENABLE;				//主输出使能, ENABLE,DISABLE
	PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;				//使能计数器, ENABLE,DISABLE
	PWM_Configuration(PWMA, &PWMx_InitStructure);				//初始化PWM通用寄存器,  PWMA,PWMB
	
	motor.L_pwm_channel = PWM3;
	motor.R_pwm_channel = PWM4;
	
	PWM3_USE_P14P15();					//PWM3选择P14P15
	PWM4_USE_P16P17();					//PWM4选择P16P17
	
	GPIO_PU_Init();						//IO口初始化
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      设置左电机速度
//  @param      speed	参数选择区间为-10000~10000	正数为正转，负数为反转
//  @return     void				
//  @since      v1.0
//  Sample usage:	Set_Lmotor_Speed(5000)
//-------------------------------------------------------------------------------------------------------------------
void Set_Lmotor_Speed(long speed)
{
	u16 duty_cycle = 0;
	
    if (speed < -10000)				// 限制速度在-10000到10000之间
        speed = -10000;
	else if (speed > 10000)
        speed = 10000;

	if(speed >= 0)
    {
		Lmotor_direction = 1;							// 电机正转
		duty_cycle = (PWM_Peri / 10000) * speed; 		// 计算占空比
		pwmA_duty.PWM3_Duty = duty_cycle;
	}
	else if(speed < 0)
	{
		Lmotor_direction = 0;							// 电机反转
		duty_cycle = (PWM_Peri / 10000) * -speed; 		// 计算占空比
		pwmA_duty.PWM3_Duty = duty_cycle;
	}
	
	UpdatePwm(PWM3, &pwmA_duty) ;	   // 设置左电机PWM输出引脚的占空比

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      设置右电机速度
//  @param      speed		参数选择区间为-10000~10000	正数为正转，负数为反转
//  @return     void				
//  @since      v1.0
//  Sample usage:	Set_Rmotor_Speed(5000)
//-------------------------------------------------------------------------------------------------------------------
void Set_Rmotor_Speed(long speed)
{
	u16 duty_cycle = 0;
	
    if (speed < -10000)					// 限制速度在-10000到10000之间
        speed = -10000;
	else if (speed > 10000)
        speed = 10000;

	if(speed >= 0)
    {
		Rmotor_direction = 1;							// 电机正转
		duty_cycle = (PWM_Peri / 10000) * speed; 		// 计算占空比
		pwmA_duty.PWM4_Duty = duty_cycle;
	}
	else if(speed < 0)
	{
		Rmotor_direction = 0;							// 电机反转
		duty_cycle = (PWM_Peri / 10000) * -speed; 		// 计算占空比
		pwmA_duty.PWM4_Duty = duty_cycle;
	}
	
	UpdatePwm(PWM4, &pwmA_duty) ;	   // 设置右电机PWM输出引脚的占空比
	
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化IO口为准双向口	内部调用，用户无需关心
//  @param      void
//  @return     void					
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void GPIO_PU_Init(void)
{
	P1_MODE_IO_PU(GPIO_Pin_4|GPIO_Pin_6);	//P1.4和P1.6输出PWM的波形
	P3_MODE_IO_PU(GPIO_Pin_5);				//P3.5控制电机方向
	P7_MODE_IO_PU(GPIO_Pin_0);				//P7.0控制电机方向
}
