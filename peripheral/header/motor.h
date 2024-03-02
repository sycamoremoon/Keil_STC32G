/*==========电机驱动头文件============*/

#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "config.h"

// 定义电机结构体
typedef struct {
    u8 L_pwm_channel; 		// PWM输出通道
	u8 R_pwm_channel; 		// PWM输出通道
} Motor;

// 内部函数声明	用户无需关心
void GPIO_PU_Init();


//函数原型
void motor_init(void);					//初始化电机
void Set_Lmotor_Speed(long speed);		//设置左电机速度
void Set_Rmotor_Speed(long speed);		//设置右电机速度	


#endif
