#include "config.h"
#include "motor.h"

#define	PWM_Peri	65535	// PWM周期设定

Motor *motor;				// 定义电机结构体
PWMx_Duty *pwmA_duty;		// 定义PWMA结构体

//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化电机
//  @param      PWM					PWM通道，取值 PWM1~PWM8,PWMA,PWMB
//  @return     void					
//  @since      v1.0
//  Sample usage:	motor_init(PWM1);
//-------------------------------------------------------------------------------------------------------------------
void motor_init(u8 PWMx)
{

	// 初始化 PWMx
	PWMx_InitDefine PWMx_InitStructure;		// 定义PWM初始化结构体
	
	pwmA_duty -> PWM1_Duty = 128; 			// 初始化PWMA的4个通道的占空比
	pwmA_duty -> PWM2_Duty = 256;
	pwmA_duty -> PWM3_Duty = 512;
	pwmA_duty -> PWM4_Duty = 1024;
	
	motor->pwm_pin = PWMx;
	
	if(PWMx == PWM1)
	{
		PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1; 			//设置 PWM1 模式
		PWMx_InitStructure.PWM_Duty = pwmA_duty -> PWM1_Duty; 				//PWM 占空比时间, 0~Period
		PWMx_InitStructure.PWM_EnoSelect = ENO1P | ENO1N; 		//输出通道选择
		PWM_Configuration(PWM1, &PWMx_InitStructure); 			//初始化 PWM1

		PWM1_USE_P60P61(); 										// PWM1 对应选择通道 P60, P61
	}

	else if(PWMx == PWM2)
	{
		PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1; 			//设置 PWM2 模式
		PWMx_InitStructure.PWM_Duty = pwmA_duty -> PWM2_Duty; 				//PWM 占空比时间, 0~Period
		PWMx_InitStructure.PWM_EnoSelect = ENO2P | ENO2N; 		//输出通道选择
		PWM_Configuration(PWM2, &PWMx_InitStructure); 			//初始化 PWM2
		
		PWM2_USE_P62P63(); 										// PWM2 选择通道 P62, P63
	}
	else if(PWMx == PWM3)
	{
		PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1; 			//设置 PWM3 模式
		PWMx_InitStructure.PWM_Duty = pwmA_duty -> PWM3_Duty; 				//PWM 占空比时间, 0~Period
		PWMx_InitStructure.PWM_EnoSelect = ENO3P | ENO3N; 		//输出通道选择
		PWM_Configuration(PWM3, &PWMx_InitStructure); 			//初始化 PWM3
		
		PWM3_USE_P64P65(); 										//PWM3 选择通道 P64, P65
	}
	else if(PWMx == PWM4)
	{
		PWMx_InitStructure.PWM_Mode = CCMRn_PWM_MODE1; 			//设置 PWM4 模式
		PWMx_InitStructure.PWM_Duty = pwmA_duty -> PWM4_Duty; 				//PWM 占空比时间, 0~Period
		PWMx_InitStructure.PWM_EnoSelect = ENO4P | ENO4N; 		//输出通道选择
		PWM_Configuration(PWM4, &PWMx_InitStructure); 			//初始化 PWM2
		
		PWM4_USE_P66P67(); 										//PWM4 选择通道 P66, P67
	}

	PWMx_InitStructure.PWM_Period = 2047; 						//周期时间, 0~65535
	PWMx_InitStructure.PWM_DeadTime = 0; 						//死区发生器设置, 0~255
	PWMx_InitStructure.PWM_MainOutEnable= ENABLE;				//主输出使能, ENABLE,DISABLE
	PWMx_InitStructure.PWM_CEN_Enable = ENABLE; 				//使能计数器, ENABLE,DISABLE
	
	PWM_Configuration(PWMA, &PWMx_InitStructure); 				//初始化 PWMA 通用寄存器
	//NVIC_PWM_Init(PWMA,DISABLE,Priority_0); 					// PWM 中断关闭, 优先级 0 级
	
	// 初始化 GPIO，全部设置为准双向口
	GPIO_PU_All_Init();
	
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      设置左电机速度
//  @param      speed
//  @return     void				
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void Set_Lmotor_Speed(float speed)
{
	float duty_cycle = 0;
	
	// 限制速度在0到100之间
    if (speed < 0)
        speed = 0;
	else if (speed > 100)
        speed = 100;	    

	// 计算占空比，假设PWM的范围是0到255
    duty_cycle = (speed * PWM_Peri) / 100; 

	// 判断计算出的占空比输出到哪个PWM通道
	if(motor->pwm_pin == PWM1)
	{
		pwmA_duty->PWM1_Duty = duty_cycle;
	}
	else if(motor->pwm_pin == PWM2)
	{
		pwmA_duty->PWM2_Duty = duty_cycle;
	}
		

	UpdatePwm(motor->pwm_pin, pwmA_duty) ;	   // 设置PWM输出引脚的占空比
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      设置右电机速度
//  @param      speed
//  @return     void				
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void Set_Rmotor_Speed(float speed)
{
	float duty_cycle = 0;
	
	// 限制速度在0到100之间
    if (speed < 0)
        speed = 0;
	else if (speed > 100)
        speed = 100;	    

	// 计算占空比，假设PWM的范围是0到255
    duty_cycle = (speed * PWM_Peri) / 100; 

	// 判断占空比duty_cycle输出到哪个PWM通道
	if(motor->pwm_pin == PWM3)
	{
		pwmA_duty->PWM3_Duty = duty_cycle;
	}
	else if(motor->pwm_pin == PWM4)
	{
		pwmA_duty->PWM4_Duty = duty_cycle;
	}

	UpdatePwm(motor->pwm_pin, pwmA_duty) ;	   // 对pwm输出通道更新占空比
	
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化所有IO口为准双向口	内部调用，用户无需关心
//  @param      void
//  @return     void					
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void GPIO_PU_All_Init(void)
{
	P0_MODE_IO_PU(GPIO_Pin_All);
	P1_MODE_IO_PU(GPIO_Pin_All);
	P2_MODE_IO_PU(GPIO_Pin_All);
	P3_MODE_IO_PU(GPIO_Pin_All);
	P4_MODE_IO_PU(GPIO_Pin_All);
	P5_MODE_IO_PU(GPIO_Pin_All);
	P6_MODE_IO_PU(GPIO_Pin_All);
	P7_MODE_IO_PU(GPIO_Pin_All);
}
