/*==========电机驱动头文件============*/

#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "config.h"

#define MAXSPEED 	10000

#define	PWM_Peri			65535	// PWM周期设定
#define Lmotor_direction	P35		// 左电机方向控制引脚
#define Rmotor_direction	P70		// 右电机方向控制引脚
#define Lmotor_PWM			P14		// 左电机方向控制引脚
#define Rmotor_PWM			P16		// 右电机方向控制引脚
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
