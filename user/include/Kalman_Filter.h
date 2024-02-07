#ifndef __KALMAN_FILTER_H__
#define __KALMAN_FILTER_H__

#include "config.h"

extern float angle;
extern float bias;
extern float P_Filt[2][2];

// º¯ÊýÔ­ÐÍ
void KalmanFilter(float newAngle, float newRate, float dt);

#endif