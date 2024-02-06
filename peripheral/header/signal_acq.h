#ifndef __SIGNAL_ACQ_H__
#define __SIGNAL_ACQ_H__

#include "config.h"
#define CHANNEL_NUM         7
#define CONVERT_TIMES       4           //DMA_ADC转化次数
#define MAX_VOLTAGE         3           //小车在赛道上能采集的最大电压
#define MIN_VOLTAGE         1           //小车在赛道上能采集的最小电压
#define MAX_BIAS          100           //小车偏移位置的映射范围最大值
#define MIN_BIAS          (-100)          //小车偏移位置的映射范围最小值
#define RATIO              MAX_BIAS/(MAX_VOLTAGE-MIN_VOLTAGE)*(MAX_VOLTAGE-MIN_VOLTAGE)*(MAX_VOLTAGE-MIN_VOLTAGE)
                                        //从线性电压数据到映射范围转化的比例
#define REF_VOLTAGE         3.3         //小车ADC参考电压
#define CAPTURE_LIMIT       4096

int8 Get_Regularized_Signal_Data(const float * Data_Array);
void Sample_All_Chanel();
uint16 Get_DMA_ADC_Result(uint8 channel);
void Signal_Acq_Init(unsigned char GPIO_PX, unsigned int GPIO_pin);

#endif