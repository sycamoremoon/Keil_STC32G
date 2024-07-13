/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		��ɿƼ�����ת����ģ��
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-03-27
 * @note		
					���߶��壺
					------------------------------------ 
					����ת����      ��Ƭ��                        
    				RX              �鿴SEEKFREE_WIRELESS.h�ļ��е�WIRELESS_UART_TX�궨��
    				TX              �鿴SEEKFREE_WIRELESS.h�ļ��е�WIRELESS_UART_RX�궨��
    				RTS             �鿴SEEKFREE_WIRELESS.h�ļ��е�RTS_PIN�궨��
    				CMD             �鿴SEEKFREE_WIRELESS.h�ļ��е�CMD_PIN�궨��
					------------------------------------ 
 ********************************************************************************************************************/

#ifndef __WIRELESS_H__
#define __WIRELESS_H__

#include "config.h"
#include "uart.h"

#define WIRELESS_UART        		UART_4         //����ת����ģ�� ��ʹ�õ��Ĵ���     
#define WIRELESS_UART_TX_PIN    	UART4_TX_P03
#define WIRELESS_UART_RX_PIN    	UART4_RX_P02
#define WIRELESS_TIMER_N    		TIM_2
#define WIRELESS_UART_BAUD   		115200
#define WIRELESS_DATA_BUF           S4BUF

#define WIRELESS_RTS_PIN 	P01 			//��������λ����  ָʾ��ǰģ���Ƿ���Խ�������  0���Լ�������  1�����Լ�������
//#define WIRELESS_CMD_PIN 	P05 			//������������

extern uint8 data_buffer[32];
extern uint8 pid_changed;

void wireless_uart_init(void);

long get_data(void);
void PID_Adjust(void);
void wireless_PID(void);
#endif 
