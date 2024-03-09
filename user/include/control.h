#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "pid.h"
#include "motor.h"
#include "encoder.h"
#include "signal_acq.h"
#include "gyro_IIChard.h"

void Update_Motors(PID_State * left_state,PID_State * right_state,PID_State * gyro_state);
void Speed_Ctrl(unsigned int Left_Speed,unsigned int Right_Speed);
void Turn_Ctrl(unsigned int Gyro);
void Regularization(PID_State * left_state,PID_State * right_state);
void Stop_Car(void);


#endif 