#include "config.h"
#include "uart.h"
#include "timer.h"

#ifndef __BOARD_H__
#define __BOARD_H__



#define EXTERNAL_CRYSTA_ENABLE 	0			// 使用外部晶振，0为不使用，1为使用（建议使用内部晶振）
#define PRINTF_ENABLE			1			// printf使能，0为失能，1为使能
#define ENABLE_IAP 				1			// 使能软件一键下载功能，0为失能，1为使能

#define DEBUG_UART 			  	UART_1
#define DEBUG_UART_BAUD 	  	115200
#define DEBUG_UART_RX_PIN  		UART1_RX_P30
#define DEBUG_UART_TX_PIN  		UART1_TX_P31
#define DEBUG_UART_TIM			TIM_2

#define SET_P54_RESRT 	  (RSTCFG |= 1<<4)	//设置P54为复位引脚

extern int32 sys_clk;

void board_init(void);
void DisableGlobalIRQ(void);
void EnableGlobalIRQ(void);


#endif 