#include "cevent.h"

#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "config.h"



void PWM_Config();				// PWM初始化设置函数
void Encoder_Check();			// 编码器检测函数(读取CNTR值)
void Encoder_Speed();			// 根据编码器计数速度计算

#endif