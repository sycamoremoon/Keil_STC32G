#include "control.h"
PID_Calibration Left_Speed_PID = {0,0,0};		//左电机速度PID参数
PID_Calibration Right_Speed_PID = {0,0,0};		//右电机速度PID参数
PID_Calibration Turn_PID = {0,0,0};			    //转向PID参数

PID_State Left_Speed_State;				//左电机速度状态参数
PID_State Right_Speed_State;			//右电机速度状态参数
PID_State Turn_State;				    //转向状态参数


/// @brief 通过PID算法调整电机速度达到目标速度
/// @param Left_Speed 参数给出左电机的目标速度
/// @param Right_Speed 参数给出右电机的目标速度
void Speed_Ctrl(unsigned int Left_Speed,unsigned int Right_Speed)
{
	
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

void Stop_Car(void)
{
	Set_Rmotor_Speed(0);
	Set_Lmotor_Speed(0);
}
