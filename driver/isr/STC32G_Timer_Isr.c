/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/

#include	"STC32G_Timer.h"
#include 	"gyroscope.h"
#include	"control.h"

u8 cnt = 0;
unsigned int pid1_output = 0;
unsigned int pid2_output = 0;
unsigned int pid3_output_left = 0;
unsigned int pid3_output_right = 0;
unsigned int output_left = 0;
unsigned int output_right = 0;
//========================================================================
// 函数: Timer0_ISR_Handler
// 描述: Timer0中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2020-09-23
//========================================================================
// 
void Timer0_ISR_Handler (void) interrupt TMR0_VECTOR		//进中断时已经清除标志 
{
	output_left = pid1_output + pid2_output + pid3_output_left;
	output_right = pid1_output + pid2_output + pid3_output_right;
	Update_Motors_2(100+output_left,100+output_right);		// 更新电机PWM = 基准速度 + pid_in(error) + pid_mid(error) +pid_out(error) 
	cnt++;
	P60 = 0;
	if(cnt % 1 == 0)
	{
	//内环pid 处理gyro数据，PID, I可以小，D的作用稍微大一点。或者使用PD
	//Get_gyro_gyro();		// 获取角速度
	//基准电机PWM的作用，保证车直走，小车不抖。用陀螺仪
	//PID输入：y_acc(滤波后的值)
	//error_in = y_acc - 0
	//pid(error_in)
	//输出的是中环的输入(下一环的期望)
	//
	//Get_gyro_accdata();			// 获取加速度
	
		accy_filter();			// 获取滤波后的accy
		Speed_Ctrl_in(0);		// PID的target为0
		pid1_output = accy_state.output;
		
	}


	if(cnt % 1 == 0)	// 中环pid adc处理，每一小段的偏移，PID
	{
		// adc获取
		// error_mid = pid(error_in) - adc获取值   （注意可能减反）
		// pid(error_mid)
		// PID输出的是外环的输入(下一环的期望)

		// 更新电机PWM = pid(error_mid)
		Speed_Ctrl_mid(pid1_output);
		pid2_output = adc_state.output;
		
	}
	
	
	if(cnt % 1 == 0)	// 外环pid 速度环，用PI
	{
		// PID输入：中环的两个输出数据
		// error = 中环的输出 - 实际速度
		// 更新电机PWM = pid(error)
		Speed_Ctrl_out(pid2_output,pid2_output);
		
		pid3_output_left = Left_Speed_State.output;
		pid3_output_right = Right_Speed_State.output;
	}
	

	if(output_left > 1500 | output_right > 1500)	// 保险,防止PWM过大
		{
			Stop_Car();
		}
		else 
		{
			
		}
		
		P60 = 1;
}

//========================================================================
// 函数: Timer1_ISR_Handler
// 描述: Timer1中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2020-09-23
//========================================================================
void Timer1_ISR_Handler (void) interrupt TMR1_VECTOR		//进中断时已经清除标志
{
	// TODO: 在此处添加用户代码
	P60 = ~P60;
}

//========================================================================
// 函数: Timer2_ISR_Handler
// 描述: Timer2中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2020-09-23
//========================================================================
void Timer2_ISR_Handler (void) interrupt TMR2_VECTOR		//进中断时已经清除标志
{
	AUXINTIF &= ~0x01;	// 清中断标志
	// TODO: 在此处添加用户代码
	
}

//========================================================================
// 函数: Timer3_ISR_Handler
// 描述: Timer3中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2020-09-23
//========================================================================
void Timer3_ISR_Handler (void) interrupt TMR3_VECTOR		//进中断时已经清除标志
{
	AUXINTIF &= ~0x02;	// 清中断标志
	// TODO: 在此处添加用户代码

}

//========================================================================
// 函数: Timer4_ISR_Handler
// 描述: Timer4中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2020-09-23
//========================================================================
void Timer4_ISR_Handler (void) interrupt TMR4_VECTOR		//进中断时已经清除标志
{
	AUXINTIF &= ~0x04;	// 清中断标志
	// TODO: 在此处添加用户代码
	
}
