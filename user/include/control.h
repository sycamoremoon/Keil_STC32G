#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "pid.h"
#include "motor.h"
#include "encoder.h"
#include "signal_acq.h"
#include "gyroscope.h"

#define MAXTURN		1000

extern PID_State accy_state;
extern PID_State adc_state;
extern PID_State Left_Speed_State;
extern PID_State Right_Speed_State;

extern PID_Calibration PID_accy;
extern PID_Calibration PID_adc;
extern PID_Calibration PID_out_left;
extern PID_Calibration PID_out_right;

extern long TargetSpeed;	//目标速度，PID控制

void Update_Motors(PID_State * left_state,PID_State * right_state,PID_State * gyro_state);
void Set_Motors(long left, long right);
void Speed_Ctrl_in(unsigned int accy_target);
void Speed_Ctrl_mid(long adc_target);
void Speed_Ctrl_out(long Left_Speed,long Right_Speed);
void Turn_Ctrl(unsigned int Gyro);
void Regularization(PID_State * left_state,PID_State * right_state);
void Stop_Car(void);


#endif 
