#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "pid.h"
#include "motor.h"
#include "encoder.h"
#include "signal_acq.h"
#include "gyroscope.h"

<<<<<<< HEAD
void Update_Motors(PID_State * left_state,PID_State * right_state,PID_State * gyro_state);
void Speed_Ctrl(unsigned int Left_Speed,unsigned int Right_Speed);
void Turn_Ctrl(unsigned int Gyro);
void Regularization(PID_State * left_state,PID_State * right_state);
=======
void Update_Motors(unsigned int Left_motor,unsigned int Right_motor);
unsigned int Speed_Ctrl(unsigned int Left_Speed,unsigned int Right_Speed);
unsigned int Turn_Ctrl(unsigned int Left_Speed,unsigned int Right_Speed,unsigned int Gyro);
unsigned int Regularization(unsigned int Pwm);

>>>>>>> bd21b71e188020094b6f438702624a306168fa69
void Stop_Car(void);


#endif 