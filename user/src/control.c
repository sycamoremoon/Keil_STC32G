#include "control.h"



PID_Calibration PID_accy 		= {50,0,50};	// 内环accy的PID参数，用PD
PID_Calibration PID_adc 		= {50,50,100};	// 中环adc的PID参数
PID_Calibration PID_out_left 	= {80,0,0};	// 外环左速度的PID参数
PID_Calibration PID_out_right 	= {80,0,0};	// 外环右速度的PID参数

PID_State accy_state		= {0};				        //accy状态参数
PID_State adc_state 		= {0};				        //adc状态参数
PID_State Left_Speed_State 	= {0};				//左电机速度状态参数
PID_State Right_Speed_State = {0};				//右电机速度状态参数

/// @brief 通过PID算法调整电机速度达到目标速度
/// @param Left_Speed 参数给出左电机的目标速度
/// @param Right_Speed 参数给出右电机的目标速度
// 内环 error_in = y_acc - 0
void Speed_Ctrl_in(unsigned int accy_target)
{
	extern float fil_acc_y;
	//获取真实accy
	accy_state.actual = fil_acc_y;
	//获取目标accy
	accy_state.target = accy_target;
	
	pid_increase(&PID_accy,&accy_state);
//	Update_Motors(&accy_state,&accy_state,&Turn_State);
}

// 中环
void Speed_Ctrl_mid(unsigned int adc_target)
{
	//获取真实adc
	adc_state.actual = (int16) Get_Regularized_Signal_Data(All_Signal_Data);
	//adc_state.actual = 0;
	//获取目标adc
	adc_state.target = adc_target;

	pid_increase(&PID_adc,&adc_state);
//	Update_Motors(&adc_state,&adc_state,&Turn_State);
}

// 外环
void Speed_Ctrl_out(unsigned int Left_Speed,unsigned int Right_Speed)
{
	//获取真实速度
	Left_Speed_State.actual = (get_EncoderL()/MAXENCODER)*MAXSPEED;
	Right_Speed_State.actual = (get_EncoderR()/MAXENCODER)*MAXSPEED;
	
	//获取目标速度
	Left_Speed_State.target = Left_Speed;
	Right_Speed_State.target = Right_Speed;
	
	pid_increase(&PID_out_left,&Left_Speed_State);
	pid_increase(&PID_out_right,&Right_Speed_State);
//	Update_Motors(&Left_Speed_State,&Right_Speed_State,&Turn_State);
}



/// @brief 通过PID算法调整电机速度达到目标速度和目标角度
/// @param Gyro 参数给出小车的目标角度
void Turn_Ctrl(unsigned int Gyro)
{
	
}

/// @brief 标准化最后的输出结果，视具体情况而定。如：缩放，限幅，归一化等操作
/// @param left_state 
/// @param right_state 
void Regularization(PID_State * left_state,PID_State * right_state)
{
	
}

/// @brief 更新电机PWM转速
/// @param left_state 
/// @param right_state 
/// @param gyro_state 
void Update_Motors(PID_State * left_state,PID_State * right_state,PID_State * gyro_state)
{
	Set_Rmotor_Speed(right_state->output + gyro_state->output);
	Set_Lmotor_Speed(left_state->output + gyro_state->output);
}

void Set_Motors(unsigned int left, unsigned int right)
{
	if(left > SAFESPEED || right > SAFESPEED)
	{
		left = SAFESPEED;
		right = SAFESPEED;
	}
	Set_Lmotor_Speed(right);
	Set_Rmotor_Speed(left);
}

void Stop_Car(void)		// 小车停止
{
	Set_Rmotor_Speed(0);
	Set_Lmotor_Speed(0);
}


