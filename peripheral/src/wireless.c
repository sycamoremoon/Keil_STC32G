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
#include "wireless.h"
#include "control.h"
uint8 start_car_signal = 0;
uint8 data_buffer[32] = {0};		// ���ݴ洢��data_buffer��
uint8 pid_changed = 0;
WIRELESS_TYPE_enum wireless_type = WIRELESS_SI24R1;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ת ����ģ���ʼ��
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
	uart_init(WIRELESS_UART, WIRELESS_UART_RX_PIN, WIRELESS_UART_TX_PIN, WIRELESS_UART_BAUD, WIRELESS_TIMER_N);	//��ʼ������    
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ͨ�����ߴ��ڽ��յ����ַ�������������
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	���յ� AP=13! �� ������������Ϊ13
//  @note       
//-------------------------------------------------------------------------------------------------------------------
long get_data(void)
{
    int		data_len 	= 0;	// ��¼����λ��
    uint8 	data_start	= 0;	// ��¼����λ��ʼ�ĵط�
	uint8 	data_end	= 0;	// ��¼����λ�����ĵط�
	uint8 	minus_Flag 	= 0; 	// �жϸ�����־λ
	uint8 	data_Num 	= 0; 	// ��¼����λ��
	long  	data_return	= 0;	// �����õ�������
	uint8 	i = 0;
	
	data_len = strlen(data_buffer);	// ��ȡdata_buffer�ĳ���

    if (data_len != 0) 
	{
		pid_changed = 1;
		for (i = 0; i < data_len; i++)	// ����'='��'!'��λ��
		{
			if (data_buffer[i] == '=') 
			{
				data_start = i + 1;	//��λ��������ʼλ
			}
			if (data_buffer[i] == '!')
			{
				data_end = i - 1;	// ��λ�����ݽ���λ
				break;
			}
		}
		
		if (data_buffer[data_start] == '-') 	// ����Ǹ���
		{
			data_start += 1; 					// ����һλ������λ
			minus_Flag = 1; 					// ����flag = 1
		}
		data_Num = data_end - data_start + 1;	// ���ݵ�λ��

		if (data_Num == 1) // ���ݹ�1λ
		{
			data_return = data_buffer[data_start]-48;	//��ȥ'0'��ASCII��48
		}
		else if (data_Num == 2) // ���ݹ�2λ
		{
			data_return = (data_buffer[data_start]-48)*10 + (data_buffer[data_end]-48);	
		}
		else if (data_Num == 3)	// ���ݹ�3λ
		{
			data_return = (data_buffer[data_start]-48)*100 + (data_buffer[data_start+1]-48)*10 + (data_buffer[data_end]-48);
		}
		else if (data_Num == 4)	// ���ݹ�4λ
		{
			data_return = (data_buffer[data_start]-48)*1000 + (data_buffer[data_start+1]-48)*100 + (data_buffer[data_start+2]-48)*10 + (data_buffer[data_end]-48);
		}
		
		if (minus_Flag == 1)	data_return = -data_return;	// �жϸ���
	}
	
	return data_return;
}

	
//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���ݽ�����������data_return��ʶ��Ҫ�����Ĳ�������data_return����
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	��λ�������ʽ������AP=58!����ADC_kp����Ϊ58
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
		
		
		// ��ʾ����PID����
//		printf("ADC:%ld,%ld,%ld\nSpeed:%ld\nTurn:%d\nVert:%d,%d\n\n",
//		PID_adc.kp, 		PID_adc.ki, 		PID_adc.kd,
//		TargetSpeed,		turn_ratio,			vertical_value, E_T);
		
		pid_changed = 0;
	}

	memset(data_buffer, 0, 32);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡbuffer������ݣ�������PID_Adjust()��������ѭ����ʵ��ʵʱ����
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       
//-------------------------------------------------------------------------------------------------------------------
void wireless_PID(void)
{
	uint8 i = 0;
	if(COM4.RX_TimeOut > 0)		//��ʱ����
		{
			if(--COM4.RX_TimeOut == 0)
			{
				if(COM4.RX_Cnt > 0)
				{
					for(i=0; i<COM4.RX_Cnt; i++)	
					{
						data_buffer[i] = RX4_Buffer[i];		//�յ������ݴ�ŵ�data_buffer������
						//TX4_write2buff(RX4_Buffer[i]);	//�յ�������ԭ������
					}
				}
				COM4.RX_Cnt = 0;
				PID_Adjust();
				
			}
		}
}