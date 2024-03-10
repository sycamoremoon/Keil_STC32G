#include "cevent.h"

#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "config.h"

#define MAXENCODER 		(3000.0)

//函数原型
void Encoder_init();
u16 get_EncoderL();
u16 get_EncoderR();

// 内部函数声明	用户无需关心
void EncoderL();
void EncoderR();


#endif