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

#ifndef __WIRELESS_H__
#define __WIRELESS_H__

#include "config.h"
#include "uart.h"

#define WIRELESS_UART        		UART_4         //无线转串口模块 所使用到的串口     
#define WIRELESS_UART_TX_PIN    	UART4_TX_P03
#define WIRELESS_UART_RX_PIN    	UART4_RX_P02
#define WIRELESS_TIMER_N    		TIM_2
#define WIRELESS_UART_BAUD   		115200
#define WIRELESS_DATA_BUF           S4BUF

#define WIRELESS_RTS_PIN 	P01 			//定义流控位引脚  指示当前模块是否可以接受数据  0可以继续接收  1不可以继续接收
//#define WIRELESS_CMD_PIN 	P05 			//定义命令引脚

extern uint8 data_buffer[];
extern uint8 pid_changed;

void wireless_uart_init(void);

long get_data(void);
void PID_Adjust(void);
void wireless_PID(void);
#endif 
