/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		逐飞科技无线转串口模块
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-03-27
 * @note		
					接线定义：
					------------------------------------ 
					无线转串口      单片机                        
    				RX              查看SEEKFREE_WIRELESS.h文件中的WIRELESS_UART_TX宏定义
    				TX              查看SEEKFREE_WIRELESS.h文件中的WIRELESS_UART_RX宏定义
    				RTS             查看SEEKFREE_WIRELESS.h文件中的RTS_PIN宏定义
    				CMD             查看SEEKFREE_WIRELESS.h文件中的CMD_PIN宏定义
					------------------------------------ 
 ********************************************************************************************************************/
#include "wireless.h"
#include "control.h"
uint8 start_car_signal = 0;
uint8 data_buffer[128] = {0};		// 数据存储在data_buffer中
uint8 pid_changed = 0;
WIRELESS_TYPE_enum wireless_type = WIRELESS_SI24R1;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      无线转 串口模块初始化
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
void wireless_uart_init(void)
{
    WIRELESS_RTS_PIN = 0;
    wireless_type = WIRELESS_SI24R1;
	uart_init(WIRELESS_UART, WIRELESS_UART_RX_PIN, WIRELESS_UART_TX_PIN, WIRELESS_UART_BAUD, WIRELESS_TIMER_N);	//初始化串口    
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      通过无线串口接收到的字符串解析出数据
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	接收到 AP=13! 则 解析到的数据为13
//  @note       
//-------------------------------------------------------------------------------------------------------------------
long get_data(void)
{
    int		data_len 	= 0;	// 记录数据位数
    uint8 	data_start	= 0;	// 记录数据位开始的地方
	uint8 	data_end	= 0;	// 记录数据位结束的地方
	uint8 	minus_Flag 	= 0; 	// 判断负数标志位
	uint8 	data_Num 	= 0; 	// 记录数据位数
	long  	data_return	= 0;	// 解析得到的数据
	uint8 	i = 0;
	
	data_len = strlen(data_buffer);	// 获取data_buffer的长度

    if (data_len != 0) 
	{
		pid_changed = 1;
		for (i = 0; i < data_len; i++)	// 查找'='和'!'的位置
		{
			if (data_buffer[i] == '=') 
			{
				data_start = i + 1;	//定位到数据起始位
			}
			if (data_buffer[i] == '!')
			{
				data_end = i - 1;	// 定位到数据结束位
				break;
			}
		}
		
		if (data_buffer[data_start] == '-') 	// 如果是负数
		{
			data_start += 1; 					// 后移一位到数据位
			minus_Flag = 1; 					// 负数flag = 1
		}
		data_Num = data_end - data_start + 1;	// 数据的位数

		if (data_Num == 1) // 数据共1位
		{
			data_return = data_buffer[data_start]-48;	//减去'0'的ASCII码48
		}
		else if (data_Num == 2) // 数据共2位
		{
			data_return = (data_buffer[data_start]-48)*10 + (data_buffer[data_end]-48);	
		}
		else if (data_Num == 3)	// 数据共3位
		{
			data_return = (data_buffer[data_start]-48)*100 + (data_buffer[data_start+1]-48)*10 + (data_buffer[data_end]-48);
		}
		else if (data_Num == 4)	// 数据共4位
		{
			data_return = (data_buffer[data_start]-48)*1000 + (data_buffer[data_start+1]-48)*100 + (data_buffer[data_start+2]-48)*10 + (data_buffer[data_end]-48);
		}
		
		if (minus_Flag == 1)	data_return = -data_return;	// 判断负数
	}
	
	return data_return;
}

	
//-------------------------------------------------------------------------------------------------------------------
//  @brief      根据解析到的数据data_return，识别要调整的参数并用data_return覆盖
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	上位机传输格式，例：AP=58!，则将ADC_kp更改为58
//  @note       
//-------------------------------------------------------------------------------------------------------------------

void PID_Adjust(void)
{
	long data_return = 0;
	data_return = get_data();
	
	if(pid_changed)
	{
		// PID_adc
		if 		(data_buffer[0] == 'A' && data_buffer[1] == 'P') 
		{
			PID_adc.kp = data_return;
		}
		else if (data_buffer[0] == 'A' && data_buffer[1] == 'I') 
		{
			PID_adc.ki = data_return;
		}
		else if (data_buffer[0] == 'A' && data_buffer[1] == 'D') 
		{
			PID_adc.kd = data_return;
		}
		
		// PID_out_left
		else if (data_buffer[0] == 'L' && data_buffer[1] == 'P') 
		{
			PID_out_left.kp = data_return;
		}
		else if (data_buffer[0] == 'L' && data_buffer[1] == 'I') 
		{
			PID_out_left.ki = data_return;
		}
		else if (data_buffer[0] == 'L' && data_buffer[1] == 'D') 
		{
			PID_out_left.kd = data_return;
		}
		
		// PID_out_right
		else if (data_buffer[0] == 'R' && data_buffer[1] == 'P') 
		{
			PID_out_right.kp = data_return;
		}
		else if (data_buffer[0] == 'R' && data_buffer[1] == 'I') 
		{
			PID_out_right.ki = data_return;
		}
		else if (data_buffer[0] == 'R' && data_buffer[1] == 'D') 
		{
			PID_out_right.kd = data_return;
		}
		
		else if (data_buffer[0] == 'V' && data_buffer[1] == 'E') 
		{
			start_car_signal = 1;
			adc_state.integral = 0;
			if(data_return == 0) Stop_Car();
			targetspeed_backup = TargetSpeed = data_return;
		}else if (data_buffer[0] == 'T' && data_buffer[1] == 'R') 
		{
			turn_ratio = data_return;
		}
		else if (data_buffer[0] == 'V' && data_buffer[1] == 'T') 
		{
			vertical_value = data_return;
		}
		else if (data_buffer[0] == 'E' && data_buffer[1] == 'T') 
		{
			E_T = data_return;
		}
		
		// PID_AngleZ
		else if (data_buffer[0] == 'Z' && data_buffer[1] == 'P') 
		{
			PID_AngleZ.kp = data_return;
		}
		else if (data_buffer[0] == 'Z' && data_buffer[1] == 'I') 
		{
			PID_AngleZ.ki = data_return;
		}
		else if (data_buffer[0] == 'Z' && data_buffer[1] == 'D') 
		{
			PID_AngleZ.kd = data_return;
		}
		
		
		// 显示三组PID参数
//		printf("ADC:%ld,%ld,%ld\nSpeed:%ld\nTurn:%d\nVert:%d,%d\n\n",\
//		PID_adc.kp, 		PID_adc.ki, 		PID_adc.kd,\
//		TargetSpeed,		turn_ratio,			vertical_value, E_T);
		
		pid_changed = 0;
	}

	memset(data_buffer, 0, 128);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取buffer里的数据，并调用PID_Adjust()。放入主循环中实现实时调参
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
void wireless_PID(void)
{
	uint8 i = 0;
	if(COM4.RX_TimeOut > 0)		//超时计数
		{
			if(--COM4.RX_TimeOut == 0)
			{
				if(COM4.RX_Cnt > 0)
				{
					for(i=0; i<COM4.RX_Cnt; i++)	
					{
						data_buffer[i] = RX4_Buffer[i];		//收到的数据存放到data_buffer数组中
						//TX4_write2buff(RX4_Buffer[i]);	//收到的数据原样返回
					}
				}
				COM4.RX_Cnt = 0;
				PID_Adjust();
				
			}
		}
}