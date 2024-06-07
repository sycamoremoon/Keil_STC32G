#include "signal_acq.h"

uint8 xdata DmaAdBuffer[CHANNEL_NUM][2*CONVERT_TIMES+4];
uint16 All_Signal_Data[CHANNEL_NUM] = {0};		//私有变量，不提供对外接口

//#pragma userclass (near=CEVENT)	
//	CEVENT_EXPORT(0,Signal_Init,NULL);
//#pragma userclass (near=default)

/// @brief 初始化ADC信号采集模块,以及使用DMA数据传输
/// @param GPIO_PX 	选择IO为P1或者P0
/// @param GPIO_pin 选择需要初始化的引脚号
void Signal_Acq_Config(unsigned char GPIO_PX, unsigned int GPIO_pin)
{
    ADC_InitTypeDef		ADC_InitStructure;		//结构定义
	DMA_ADC_InitTypeDef		DMA_ADC_InitStructure;		//结构定义

    if (GPIO_PX == GPIO_P0)
        P0_MODE_IN_HIZ(GPIO_pin)
	else if(GPIO_PX == GPIO_P1)
        P1_MODE_IN_HIZ(GPIO_pin)
	else return;

	ADC_InitStructure.ADC_SMPduty   = 31;		//ADC 模拟信号采样时间控制, 0~31（注意： SMPDUTY 一定不能设置小于 10）
	ADC_InitStructure.ADC_CsSetup   = 0;		//ADC 通道选择时间控制 0(默认),1
	ADC_InitStructure.ADC_CsHold    = 1;		//ADC 通道选择保持时间控制 0,1(默认),2,3
	ADC_InitStructure.ADC_Speed     = ADC_SPEED_2X8T;		//设置 ADC 工作时钟频率	ADC_SPEED_2X1T~ADC_SPEED_2X16T
	ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;	//ADC结果调整,	ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
	ADC_Inilize(&ADC_InitStructure);		//初始化
	ADC_PowerControl(ENABLE);						//ADC电源开关, ENABLE或DISABLE
	NVIC_ADC_Init(DISABLE,Priority_0);		//中断使能


	DMA_ADC_InitStructure.DMA_Enable = ENABLE;			//DMA使能  	ENABLE,DISABLE
	DMA_ADC_InitStructure.DMA_Channel = (GPIO_PX==GPIO_P1)?GPIO_pin:(GPIO_pin<<8);			//ADC通道使能寄存器, 1:使能, bit15~bit0 对应 ADC15~ADC0
	DMA_ADC_InitStructure.DMA_Buffer = (u16)DmaAdBuffer;	//ADC转换数据存储地址
	DMA_ADC_InitStructure.DMA_Times = ADC_4_Times;	//每个通道转换次数, ADC_1_Times,ADC_2_Times,ADC_4_Times,ADC_8_Times,ADC_16_Times,ADC_32_Times,ADC_64_Times,ADC_128_Times,ADC_256_Times
	DMA_ADC_Inilize(&DMA_ADC_InitStructure);		//初始化
	NVIC_DMA_ADC_Init(DISABLE,Priority_0,Priority_0);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0~Priority_3; 总线优先级(低到高) Priority_0~Priority_3
	DMA_ADC_TRIG();		//触发启动转换
}

/// @brief 读取由DMA直接存入的ADC采集数据
/// @param channel 通道号
/// @return ADC转化数据，右对齐
uint16 Get_DMA_ADC_Result(uint8 channel)
{
	uint16 * ADC_Data;
	uint16 adc;
	ADC_Data = (uint16 *) &DmaAdBuffer[channel][2*CONVERT_TIMES+2];		//指向了ADC采集数据的平均值
	if(RESFMT)		//转换结果右对齐。 
	{
		adc = *(uint16 *)ADC_Data;						//由于ADC_data是16位的格式，所以直接取地址
	}
	else		//转换结果左对齐。 
	{
		adc = *(uint16 *)ADC_Data;
		adc = adc>>4;
	}
	if(channel == CHANNEL_NUM-1)
	{
		DMA_ADC_STA &= ~0x01;	//清标志位
		DMA_ADC_TRIG();		//触发启动转换
	}
	return adc;
}

/// @brief 采集所有通道的ADC数值，放置在一个文件作用域的私有数组中
/// @note 可以放入中断函数执行，也可以导出事件执行
void Sample_All_Chanel()
{
	uint8 channel;
	for(channel = 0 ; channel < CHANNEL_NUM ; channel++)
	{
		All_Signal_Data[channel] = Get_DMA_ADC_Result(channel);
	}
}


/// @brief 将电压数据经过运算成关于偏移量的线性函数，并且转化成车身偏差
/// @param Data_Array 参数为存入电压值的数组
/// @return 返回一个在指定范围内的，表示偏移量的有符号整形数据

int32 Get_Regularized_Signal_Data(const uint16 * Data_Array)
{
	int32 answer = 0;
	int32 diff1,diff2 ,sum1,sum2;
	int32 strai=453000;
	int32 turn =968411L;

	if(*(Data_Array+1)>STANDERD||*(Data_Array+2)>STANDERD)
	{
		if(*(Data_Array+1)>*(Data_Array+2))
		{
			diff2 = *(Data_Array+1)-*(Data_Array+2);
			sum2 = *(Data_Array+1)+*(Data_Array+2);
			answer = (diff2*turn)/(sum2*sum2);
		}
		else
		{
			diff2 = *(Data_Array+2)-*(Data_Array+1);
			sum2 = *(Data_Array+2)+*(Data_Array+1);
			answer = -(diff2*turn)/(sum2*sum2);
		}
		printf("%d\n",diff2*turn);
		printf("%d\n",sum2*sum2);
	}
	else
	{
		if(*(Data_Array)>*(Data_Array+3))
		{
			diff1 = *Data_Array-*(Data_Array+3);
			sum1 = *Data_Array+*(Data_Array+3);
			answer = (diff1*strai)/(sum1*sum1);
		}
		else
		{
			diff1 = *(Data_Array+3)-*(Data_Array);
			sum1 = *Data_Array+*(Data_Array+3);
			answer = -(diff1*strai)/(sum1*sum1);
		}
	}

	return answer;
}

/// @brief 测试ADC采集准确性使用
/// @return 采集的一个通道电压
float Get_Voltage()
{
	float vol;
	vol = (Get_DMA_ADC_Result(0)/CAPTURE_LIMIT)*REF_VOLTAGE;
	return vol;
}

/// @brief 信号采集初始化
/// @return void
void Signal_Init(void)
{
	Signal_Acq_Config(GPIO_P1,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_5|GPIO_Pin_7);
}