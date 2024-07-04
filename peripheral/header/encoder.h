#include "cevent.h"

#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "config.h"

#define MAXENCODER 		(1650)	//10ms的1024线编码器最大获取脉冲数
#define Encode_L		P04
#define Encode_R		P06
#define Dir_Encode_L	P20
#define Dir_Encode_R	P22

//函数原型
void Encoder_init();
uint16 get_EncoderL();
uint16 get_EncoderR();

// 内部函数声明	用户无需关心
void EncoderL();
void EncoderR();


#endif