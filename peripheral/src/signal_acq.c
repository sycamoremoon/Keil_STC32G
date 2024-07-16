#include "signal_acq.h"

extern long TargetSpeed,targetspeed_backup;	//目标速度，备份目标速度
extern uint8 start_car_signal;		//发车信号
#define BUFFERLENGTH 10
#define WEIGHTSUM 	70
#define MAXI(a,b) (a > b ? a : b)
#define MINI(a,b) (a > b ? b : a)

uint8 xdata DmaAdBuffer[CHANNEL_NUM][2*CONVERT_TIMES+4];
uint16 ADC_DataBuffer[CHANNEL_NUM][BUFFERLENGTH] = {0};
static uint16 ADC_counter[CHANNEL_NUM] = {0};
uint16 All_Signal_Data[CHANNEL_NUM] = {0};
uint16 Weight[BUFFERLENGTH] = {1,1,2,2,3,3,4,4,10,40};
int turn_ratio= 222;
int vertical_value=0;
int E_T = 168;

void Stop_Car();

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
	static int start_up_enter = 100;
	uint32 ADC_Value_Sum = 0;
	uint32 Average_value[CHANNEL_NUM] = {0};
	uint16 * ADC_Data;
	uint16 adc;
	uint8 j,cnt;
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
	return adc;		// select if use filter
	cnt = ADC_counter[channel];
	ADC_DataBuffer[channel][cnt] = adc;
	ADC_counter[channel] = ADC_counter[channel]+1;
	if(ADC_counter[channel] == BUFFERLENGTH) ADC_counter[channel] = 0;
	// zero them
	Average_value[channel] = 0;
	ADC_Value_Sum = 0;
	for(j = 0; j < BUFFERLENGTH; j++,cnt++)
    {
		if(abs((int)(ADC_DataBuffer[channel][cnt%BUFFERLENGTH] - (Average_value[channel] / (j+1)))) < 350 || start_up_enter--){
			ADC_Value_Sum += ADC_DataBuffer[channel][cnt%BUFFERLENGTH] * Weight[j];
			Average_value[channel] += ADC_DataBuffer[channel][cnt%BUFFERLENGTH];
		}else{	//out of range
			ADC_Value_Sum += Average_value[channel] / (j+1) * Weight[j];
		}
    }
	
	ADC_Value_Sum = ADC_Value_Sum / (WEIGHTSUM);
	
	if(abs((int)ADC_Value_Sum) < 200) ADC_Value_Sum = 0;
	
	return ADC_Value_Sum;
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
	static int32 previous = 0;
	static int32 strai=453000, turn =968411;
	static int count = 0;
	static int enter_time = 0;
	static uint8 enter_island_begin = 0, enter_island_finish = 0, leave_island_finish = 0, leave_island_begin = 0, out_island;
	static uint8 enter_angle90_begin = 0, enter_angle90_finish = 0;
	uint8 angle90_flag, cross_flag, soft_turn_flag;
	int32 answer = 0, mid_answer = 0;
	int32 diff1,diff2 ,sum1,sum2;

	angle90_flag = 0;
	cross_flag = 0;
	soft_turn_flag = 0;
	
	if(abs((int)(Data_Array[1] - Data_Array[2])) > 850 && abs((int)(Data_Array[0] - Data_Array[3])) < 1800) angle90_flag = 1;
	if(MAXI(Data_Array[0],Data_Array[3]) < 1300 && MAXI(Data_Array[0],Data_Array[3]) > 650 && Data_Array[0] > 420){
		if(abs((int)(Data_Array[1] - Data_Array[2])) > 550) angle90_flag = 1;
	}
	if(MINI(Data_Array[1],Data_Array[2]) > 1500 && Data_Array[0] > 420){
		if(abs((int)(Data_Array[1] - Data_Array[2])) > 650) angle90_flag = 1;
	}
	if((MAXI(Data_Array[1], Data_Array[2])/MINI(Data_Array[1], Data_Array[2]) > 4) && (MINI(Data_Array[1], Data_Array[2]) > 200)) angle90_flag = 1;
	//弯道不使用中间电感
	if(angle90_flag){
		if(Data_Array[1] > Data_Array[2]){
			if(Data_Array[1] < 1700){
				if(abs((int)(Data_Array[0] - Data_Array[1])) < 340) soft_turn_flag = 1;
				if(abs((int)(Data_Array[2] - Data_Array[3])) < 230 && abs((int)(Data_Array[2] - Data_Array[3])) > 80) soft_turn_flag = 1;
			}
		}
		else{
			if(Data_Array[2] < 1600){
				if(abs((int)(Data_Array[2] - Data_Array[3])) < 340) soft_turn_flag = 1;
				if(abs((int)(Data_Array[0] - Data_Array[1])) < 230 && abs((int)(Data_Array[0] - Data_Array[1])) > 80) soft_turn_flag = 1;
			}
		}
	}
	if(Data_Array[0]+Data_Array[1]+Data_Array[2]+Data_Array[3] > 4800) cross_flag = 1;
	
	//enter_angle90_begin置1之后只有当确认离开直角才会清零
	if(angle90_flag == 1){
		if(enter_angle90_begin == 0) enter_angle90_begin = 1;
//		if(targetspeed_backup >= 400) TargetSpeed = targetspeed_backup - 400;
		enter_time ++;
	}else{		//电感检测不到直角信号
		if(enter_angle90_begin == 1) enter_angle90_finish = 1;
	}
	
	//如果enter_angle90_finish为1但是没有达到指定的次数，还是认为没有完成一次直角转弯
	if(enter_angle90_finish == 1 && enter_time < E_T){
		enter_angle90_finish = 0;
		enter_time ++;
		angle90_flag = 1;
	}
	
	//如果到这里enter_angle90_finish还是为1，则说明确实执行了E_T次确实离开了直角
	if(enter_angle90_finish == 1){
		enter_angle90_finish = enter_angle90_begin = 0;
//		TargetSpeed = targetspeed_backup;
		enter_time = 0;
	}
	
	//纵向电感
	if(angle90_flag && !cross_flag && !soft_turn_flag){
//		P34 = 1;
		if(Data_Array[1] > Data_Array[2]){
			diff2 = Data_Array[1]-Data_Array[2];
			sum2 = Data_Array[1]+Data_Array[2];
			mid_answer = (diff2*turn)/(sum2*sum2);
			mid_answer = mid_answer * turn_ratio / 100;
		}else{
			diff2 = Data_Array[2]-Data_Array[1];
			sum2 = Data_Array[1]+Data_Array[2];
			mid_answer = (diff2*turn)/(sum2*sum2);
			mid_answer = mid_answer * turn_ratio / 50;
			mid_answer = - mid_answer;
		}
		answer += mid_answer;
	}else{
//		P34 = 0;
	}
	
	//横向电感
	if(abs((int)(Data_Array[0] - Data_Array[3])) > 100){
		if(angle90_flag && !cross_flag && !soft_turn_flag && MINI(Data_Array[0],Data_Array[3]) < 300){
			// Do nothing
		}else{
			if(Data_Array[0] > Data_Array[3]){
				diff1 = Data_Array[0]-Data_Array[3];
				sum1 = *Data_Array+*(Data_Array+3);
				mid_answer = (diff1*strai)/(sum1*sum1);
			}else{
				diff1 = Data_Array[3]-Data_Array[0];
				sum1 = *Data_Array+*(Data_Array+3);
				mid_answer = (diff1*strai)/(sum1*sum1);
				mid_answer = -mid_answer;
			}
			answer += mid_answer;	
		}		
	}
	
	// 入环检测
	if(Data_Array[0] > 3500 && enter_island_finish == 0){
		enter_island_begin = 1;
	}
	if(Data_Array[3] > 3500 && leave_island_finish == 0 && enter_island_finish == 1){
		leave_island_begin = 1;
	}
	if(enter_island_begin){
		if(Data_Array[1] > Data_Array[2]) answer += 400;
		else answer += -400;
		if(Data_Array[0] < 1500 && Data_Array[3] < 1500 && enter_island_finish == 0){
			enter_island_finish = 1;
			enter_island_begin = 0;
		}
	}
	if(leave_island_begin){
		if(Data_Array[1] > Data_Array[2]) answer += 400;
		else answer += -400;
		if(Data_Array[0] < 1500 && Data_Array[3] < 1500 && leave_island_finish == 0 && enter_island_finish == 1){
			leave_island_finish = 1;
			leave_island_begin = 0;
		}
	}
	
//		if(leave_island_finish == 1 && enter_island_finish == 1) enter_island_finish = leave_island_finish = 0; //重置环岛标志
		//冲出赛道停车
	if(Data_Array[0]+Data_Array[1]+Data_Array[2]+Data_Array[3] < 200){
		if(enter_angle90_begin == 1){if(targetspeed_backup >= 400) TargetSpeed = targetspeed_backup - 400;}	//仍处于直角转弯状态
		else Stop_Car();
	}else{
		if(start_car_signal == 1) TargetSpeed = targetspeed_backup;
	}

	if(labs(answer) > 1200) answer > 0 ? answer = 1200 : answer = -1200;
	if(labs(answer) < 5) answer = 0;
	
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
