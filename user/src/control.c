#include "control.h"

PID_Calibration		PID_AngleZ 		= {-20,0,-450};		// 内环陀螺仪的PID参数，用PD
PID_Calibration 	PID_adc 		= {-660,-10,-35};	// 中环adc的PID参数 20
PID_Calibration 	PID_out_left 	= {450,1300,30};	// 外环左速度的PID参数hong{290,195,57};
PID_Calibration		PID_out_right 	= {450,1300,35};	// 外环右速度的PID参数lv  {290,195,57};

PID_State	AngleZ_state		= {0};				//accy状态参数
PID_State 	adc_state 			= {0};				//adc状态参数
PID_State 	Left_Speed_State 	= {0};				//左电机速度状态参数
PID_State 	Right_Speed_State 	= {0};				//右电机速度状态参数

long TargetSpeed = 0;
long targetspeed_backup = 0;

long pid1_output = 0;
long pid2_output = 0;
long pid3_output_left = 0;
long pid3_output_right = 0;
long output_left = 0;
long output_right = 0;
long AngleZ_output = 0;

extern uint8 start_car_signal;	//发车信号

/// @brief 通过PID算法调整电机速度达到目标速度
/// @param Left_Speed 参数给出左电机的目标速度
/// @param Right_Speed 参数给出右电机的目标速度
// 内环 error_in = y_acc - 0
void Speed_Ctrl_in(long AngleZ_target)
{
	//获取真实accy
	AngleZ_state.actual = (long)Angle_Z * 100;
	//获取目标accy
	AngleZ_state.target = AngleZ_target * 100;
	
	pid_location(&PID_AngleZ,&AngleZ_state);
//	Update_Motors(&accy_state,&accy_state,&Turn_State);
}

// 中环
void Speed_Ctrl_mid(long adc_target)
{
	//获取真实adc
	adc_state.actual = (long)Get_Regularized_Signal_Data(All_Signal_Data);
//	printf("adc_state:%ld,%ld\n",adc_state.integral,adc_state.actual);
	//adc_state.actual = 0;
	//获取目标adc
	adc_state.target = adc_target;

	pid_location(&PID_adc,&adc_state);

}

// 外环
void Speed_Ctrl_out(long Left_Speed,long Right_Speed)
{
	//获取真实速度*MAXSPEED/MAXENCODER
	Left_Speed_State.actual = get_EncoderL();
	Right_Speed_State.actual = get_EncoderR();
//	
	
	//获取目标速度
	Left_Speed_State.target = Left_Speed * MAXENCODER_L / MAXSPEED;
	Right_Speed_State.target = Right_Speed * MAXENCODER_R / MAXSPEED;
	
	pid_increase(&PID_out_left,&Left_Speed_State);
	pid_increase(&PID_out_right,&Right_Speed_State);
//	Update_Motors(&Left_Speed_State,&Right_Speed_State,&Turn_State);
}



/// @brief 通过PID算法调整电机速度达到目标速度和目标角度
/// @param Gyro 参数给出小车的目标角度
//void Turn_Ctrl(unsigned int Gyro)
//{
//	
//}

/// @brief 标准化最后的输出结果，视具体情况而定。如：缩放，限幅，归一化等操作
/// @param left_state 
/// @param right_state 
//void Regularization(PID_State * left_state,PID_State * right_state)
//{
//	
//}

/// @brief 更新电机PWM转速
/// @param left_state 
/// @param right_state 
/// @param gyro_state 
void Update_Motors(PID_State * left_state,PID_State * right_state,PID_State * gyro_state)
{
	Set_Rmotor_Speed(right_state->output + gyro_state->output);
	Set_Lmotor_Speed(left_state->output + gyro_state->output);
}

void Set_Motors(long left, long right)
{
	Set_Lmotor_Speed(left * MAXSPEED / MAXENCODER_L);
	Set_Rmotor_Speed(right * MAXSPEED / MAXENCODER_R);
}

void Stop_Car(void)		// 小车停止
{
	if(TargetSpeed >= 50){
		if(TargetSpeed > 0) TargetSpeed = 0;
		else TargetSpeed = TargetSpeed - 100;
		memset((void*)&Left_Speed_State,0,sizeof(PID_State));
		memset((void*)&Right_Speed_State,0,sizeof(PID_State));
		adc_state.integral = 0;
	}else{
		start_car_signal = 0;
	}
}


