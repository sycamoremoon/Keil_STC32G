#ifndef __SIGNAL_ACQ_H__
#define __SIGNAL_ACQ_H__

#include "config.h"
#define CHANNEL_NUM         1           //一共需要采集的通道数目
#define CONVERT_TIMES       4           //DMA_ADC采集的次数
#define MAX_VOLTAGE         2.5           //小车能采集的最大电压
#define MIN_VOLTAGE         1.6           //小车能采集的最小电压
#define MAX_BIAS          100           //标准化后表示电压大小的参数最大值
#define MIN_BIAS          (-100)        //标准化后表示电压大小的最小值
#define RATIO              MAX_BIAS/(MAX_VOLTAGE-MIN_VOLTAGE)*(MAX_VOLTAGE-MIN_VOLTAGE)*(MAX_VOLTAGE-MIN_VOLTAGE)
                                        //将ADC采集值转化成标准范围的比例
#define REF_VOLTAGE         5         //参考电压
#define CAPTURE_LIMIT       4096

int8 Get_Regularized_Signal_Data(const float * Data_Array);
void Sample_All_Chanel();
uint16 Get_DMA_ADC_Result(uint8 channel);
void Signal_Acq_Init(unsigned char GPIO_PX, unsigned int GPIO_pin);
float Get_Voltage();
#endif