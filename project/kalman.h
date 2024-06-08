#ifndef __KALMAN_H__
#define __KALMAN_H__

typedef struct
{
    /*不用动*/
    float LastP;//上次估算协方差
    float Now_P;//当前估算协方差
    float out;//卡尔曼滤波器输出
    float Kg;//卡尔曼增益
	float Q;
	float R;
}Kalman_Typedef;

extern Kalman_Typedef Speed_Kalman_L,Speed_Kalman_R;

void Kalman_Init(Kalman_Typedef *klm, const float klm_Q, const float klm_R);
double KalmanFilter(Kalman_Typedef *klm, float input);

#endif

