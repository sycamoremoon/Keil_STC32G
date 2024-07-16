/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序        */
/*---------------------------------------------------------------------*/

#include	"STC32G_Timer.h"
#include 	"gyroscope.h"
#include 	"telemeter.h"
#include	"control.h"
int cnt = 0;
extern uint8 start_car_signal;
extern int start_get_distance;
uint16 distance = 0;
int turn_out_start_flag = 0, turn_in_start_flag = 0, turn_finish_flag = 0, turn_in_end_flag = 0;
long keep_going = 0;
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
	output_left = pid3_output_left;
	output_right = pid3_output_right;
	cnt++;
	if(TargetSpeed > 0 && start_car_signal){
		if(turn_out_start_flag == 0 && turn_in_start_flag == 0 && turn_in_end_flag == 0){
			output_left = output_left - pid2_output;
			output_right = output_right + pid2_output; 
		}else if(turn_out_start_flag == 1 || turn_in_start_flag == 1){
			output_left = output_left - AngleZ_output;
			output_right = output_right + AngleZ_output;
		}
	}
	Set_Motors(output_left,output_right);		// 更新电机PWM

	/* 陀螺仪控制环 */
	if(turn_finish_flag == 0){
		if(turn_out_start_flag == 0 && turn_in_start_flag == 0 && turn_in_end_flag == 0){
			start_get_distance++;
			if(distance > 600 && distance < dis_near ){		// 600-900
				turn_out_start_flag = 1;
				Angle_Z = 90;
				P34 = 1;
			}
		}
		
		if(turn_out_start_flag == 1 || turn_in_start_flag == 1){
			Get_angle();
	//		printf("sa:%.2f\n",Angle_Z);
		}
		
		if(turn_out_start_flag == 1 && turn_in_start_flag == 0)		// 偏航
		{
			pid2_output = 0;							// adc环控制输出置零
	//		TargetSpeed = targetspeed_backup - 300; 	// 降速
			Speed_Ctrl_in(150);
			AngleZ_output = AngleZ_state.output;
			if(Angle_Z > 125 && Angle_Z < 140){		// 偏移角度条件判断,初始值为90°
				turn_in_start_flag = 1;
				turn_out_start_flag = 0;			//turn_out finished
			}
		}

		if(turn_in_start_flag == 1)		// 返航
		{
			if(keep_going < 6000)
			{
				AngleZ_output = 0;
				keep_going += Left_Speed_State.actual / 5;
			}	
			else{
	//			TargetSpeed = targetspeed_backup;	// 恢复原速度
				Speed_Ctrl_in(60);
				AngleZ_output = AngleZ_state.output;
				if(Angle_Z > 50 && Angle_Z < 67){	// 偏移角度条件判断,初始值为90°
					turn_in_start_flag = 0;			//turn in finished
					turn_in_end_flag = 1;			//finish all
					keep_going = 0;
				}
			}
		}

		if(turn_in_end_flag == 1)
		{
			if(keep_going < 6000)
			{
				AngleZ_output = 0;
				keep_going += Left_Speed_State.actual / 5;
			}	
			else
			{
				turn_finish_flag = 1;
				turn_in_end_flag = 0;
				ctimer_count_clean(CTIM3_P04);
				ctimer_count_clean(CTIM4_P06);
			}
		}
	}
	
	if(turn_finish_flag == 1)
	{
		keep_going = 0;
		AngleZ_output = 0;
		TargetSpeed = targetspeed_backup;
		P34 = 0;
	}

//	/* ADC控制环 */
	if(turn_out_start_flag == 0 && turn_in_start_flag == 0 && turn_in_end_flag == 0)	// 中环pid adc处理，每一小段的偏移，PID 
	{
		// adc获取
		// error_mid = pid(error_in) - adc获取值   （注意可能减反）
		// pid(error_mid)
		// PID输出的是外环的输入(下一环的期望)
//		P34 = 1;
		// 更新电机PWM = pid(error_mid)
		Sample_All_Chanel();
		Speed_Ctrl_mid(0);
		pid2_output = adc_state.output;
	}
//	else
//		P34 = 0;
//	
	
	/* 电机速度控制环 */
	if(turn_out_start_flag == 0 && turn_in_start_flag == 0 && turn_in_end_flag == 0 && cnt % 5 == 0)	// 外环pid 速度环，用PI
	{
		cnt = 0;
		// PID输入：中环的两个输出数据
		// error = 中环的输出 - 实际速度
		// 更新电机PWM = pid(error)
		// 爬坡下坡和适应电压的
		Speed_Ctrl_out(TargetSpeed,TargetSpeed);
//		printf("F: %d,%d\n",get_EncoderL(), get_EncoderR());
//		printf("F: %ld,%ld\n",Left_Speed_State.actual ,Right_Speed_State.actual);
		pid3_output_left  = Left_Speed_State.output;
		pid3_output_right = Right_Speed_State.output;
		
	}

	if(reset_1 == 666)
	{
		reset_1 = 0;
		
		turn_out_start_flag = 0;
		turn_in_start_flag = 0;
		turn_finish_flag = 0;
		turn_in_end_flag = 0;
		distance = 5555;

		P34 = 0;
	}
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
