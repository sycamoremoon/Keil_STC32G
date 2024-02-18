#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "pid.h"
#include "motor.h"
#include "encoder.h"
#include "signal_acq.h"
#include "gyroscope.h"

void Update_Motors(unsigned int Left_motor,unsigned int Right_motor);
unsigned int Speed_Ctrl(unsigned int Left_Speed,unsigned int Right_Speed);
unsigned int Turn_Ctrl(unsigned int Left_Speed,unsigned int Right_Speed,unsigned int Gyro);
unsigned int Regularization(unsigned int Pwm);

void Stop_Car(void);


#endif 