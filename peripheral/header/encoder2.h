#include "cevent.h"

#ifndef __ENCODER2_H__
#define __ENCODER2_H__

#include "config.h"


// 内部函数声明	用户无需关心
void Encoder1_init();
void Encoder2_init();


//函数原型
//void Timer_config(u8 TimerX);
void Encoder1();
void Encoder2();
void Encoder_init();

#endif