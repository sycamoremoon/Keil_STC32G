#include "Kalman_Filter.h"
#include <math.h> 

// 卡尔曼滤波器参数
#define Q_angle		0.001		// 角度的测量噪声方差
#define Q_bias		0.003		// 角速度偏差的测量噪声方差
#define R_measure	0.03		// 测量的噪声方差
// 定义全局变量，同时初始化
float angle = 0; 					// 卡尔曼滤波器计算的角度
float bias = 0;  					// 卡尔曼滤波器计算的陀螺仪偏置
float P_Filt[2][2] = {{0, 0}, {0, 0}}; 	// 误差协方差矩阵

//-------------------------------------------------------------------------------------------------------------------
//  @brief      更新卡尔曼滤波器
//  @param      filter		指向卡尔曼滤波器结构体的指针
//  @param      newAngle	陀螺仪测量得到的新角度值
//  @param      newRate		陀螺仪测量得到的新角速度值
//  @param      dt			时间间隔，表示两次测量之间的时间差，通常以秒为单位。
//  @return     		
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
void KalmanFilter(float newAngle, float newRate, float dt)
{
	float S, y_Filt;
	float K_Filt[2] = {0, 0};	// 卡尔曼增益
	
    // 预测更新
    angle += dt * (newRate - bias);
    P_Filt[0][0] += dt * (dt*P_Filt[1][1] - P_Filt[0][1] - P_Filt[1][0] + Q_angle);
    P_Filt[0][1] -= dt * P_Filt[1][1];
    P_Filt[1][0] -= dt * P_Filt[1][1];
    P_Filt[1][1] += Q_bias * dt;

    // 测量更新
    S = P_Filt[0][0] + R_measure;
    K_Filt[0] = P_Filt[0][0] / S;
    K_Filt[1] = P_Filt[1][0] / S;

    y_Filt = newAngle - angle;
    angle += K_Filt[0] * y_Filt;
    bias += K_Filt[1] * y_Filt;

    P_Filt[0][0] -= K_Filt[0] * P_Filt[0][0];
    P_Filt[0][1] -= K_Filt[0] * P_Filt[0][1];
    P_Filt[1][0] -= K_Filt[1] * P_Filt[0][0];
    P_Filt[1][1] -= K_Filt[1] * P_Filt[0][1];
}

