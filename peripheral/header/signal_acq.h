#ifndef __SIGNAL_ACQ_H__
#define __SIGNAL_ACQ_H__

#include "config.h"
#define CHANNEL_NUM         4           //一共需要采集的通道数目
#define CONVERT_TIMES       4           //DMA_ADC采集的次数
#define MAX_MESU         3000           //小车能采集的最大电压
#define MIN_MESU         300          //小车能采集的最小电压
#define MAX_BIAS          100           //标准化后表示电压大小的参数最大值
#define MIN_BIAS          (-100)        //标准化后表示电压大小的最小值
#define STANDERD            1500    //中间电感采集的触发转弯电压
                                  
#define REF_VOLTAGE         (3.3)         //参考电压
#define CAPTURE_LIMIT       (4095.0)


int32 Get_Regularized_Signal_Data(const uint16 * Data_Array);
void Sample_All_Chanel();
uint16 Get_DMA_ADC_Result(uint8 channel);
void Signal_Init(void);
float Get_Voltage();
#endif
