/*==========电机驱动头文件============*/

#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "config.h"

// 定义电机结构体
typedef struct {
    u8 pwm_pin; 		// PWM输出引脚
    u8 direction_pin1;  // 电机方向控制引脚1
    u8 direction_pin2;  // 电机方向控制引脚2
} Motor;

// 内部函数声明	用户无需关心
void GPIO_PU_Init();



//函数原型
void motor_init(u8 PWMx);				//初始化电机
void Set_Lmotor_Speed(float speed);		//设置左电机速度
void Set_Rmotor_Speed(float speed);		//设置右电机速度	



#endif
