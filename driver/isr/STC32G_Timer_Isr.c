/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����        */
/*---------------------------------------------------------------------*/

#include	"STC32G_Timer.h"
#include 	"gyroscope.h"
#include 	"telemeter.h"
#include	"control.h"
int cnt = 0;
extern uint8 start_car_signal;
extern int start_get_distance;
uint16 distance = 0;
extern uint8 enter_island_begin, enter_island_finish, leave_island_finish, leave_island_begin;
int turn_out_start_flag = 0, turn_in_start_flag = 0, turn_finish_flag = 0, turn_in_end_flag = 0;
long keep_going = 0;
//========================================================================
// ����: Timer0_ISR_Handler
// ����: Timer0�жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2020-09-23
//========================================================================
// 
void Timer0_ISR_Handler (void) interrupt TMR0_VECTOR		//���ж�ʱ�Ѿ������־ 
{
	output_left = pid3_output_left;
	output_right = pid3_output_right;
	cnt++;
	if(TargetSpeed > 0 && start_car_signal){
		if(turn_out_start_flag == 0 && turn_in_start_flag == 0 && turn_in_end_flag == 0){
			output_left = output_left - pid2_output;
			output_right = output_right + pid2_output; 
		}else if(turn_out_start_flag == 1 || turn_in_start_flag == 1){
			output_left = output_left - AngleZ_output;
			output_right = output_right + AngleZ_output;
		}
	}
	Set_Motors(output_left,output_right);		// ���µ��PWM

	/* �����ǿ��ƻ� */
	if(turn_finish_flag == 0){
		if(turn_out_start_flag == 0 && turn_in_start_flag == 0 && turn_in_end_flag == 0){
			start_get_distance++;
			if(distance > 600 && distance < dis_near ){		// 600-900
				turn_out_start_flag = 1;
				Angle_Z = 90;
//				P34 = 1;
			}
		}
		
		if(turn_out_start_flag == 1 || turn_in_start_flag == 1){
			Get_angle();
	//		printf("sa:%.2f\n",Angle_Z);
		}
		
		if(turn_out_start_flag == 1 && turn_in_start_flag == 0)		// ƫ��
		{
			pid2_output = 0;							// adc�������������
	//		TargetSpeed = targetspeed_backup - 300; 	// ����
			Speed_Ctrl_in(150);
			AngleZ_output = AngleZ_state.output;
			if(Angle_Z > 125 && Angle_Z < 140){		// ƫ�ƽǶ������ж�,��ʼֵΪ90��
				turn_in_start_flag = 1;
				turn_out_start_flag = 0;			//turn_out finished
			}
		}

		if(turn_in_start_flag == 1)		// ����
		{
			if(keep_going < 6000)
			{
				AngleZ_output = 0;
				keep_going += Left_Speed_State.actual / 5;
			}	
			else{
	//			TargetSpeed = targetspeed_backup;	// �ָ�ԭ�ٶ�
				Speed_Ctrl_in(60);
				AngleZ_output = AngleZ_state.output;
				if(Angle_Z > 50 && Angle_Z < 67){	// ƫ�ƽǶ������ж�,��ʼֵΪ90��
					turn_in_start_flag = 0;			//turn in finished
					turn_in_end_flag = 1;			//finish all
					keep_going = 0;
				}
			}
		}

		if(turn_in_end_flag == 1)
		{
			if(keep_going < 6000)
			{
				AngleZ_output = 0;
				keep_going += Left_Speed_State.actual / 5;
			}	
			else
			{
				turn_finish_flag = 1;
				turn_in_end_flag = 0;
				ctimer_count_clean(CTIM3_P04);
				ctimer_count_clean(CTIM4_P06);
			}
		}
	}
	
	if(turn_finish_flag == 1)
	{
		keep_going = 0;
		AngleZ_output = 0;
		TargetSpeed = targetspeed_backup;
		P34 = 0;
	}

//	/* ADC���ƻ� */
	if(turn_out_start_flag == 0 && turn_in_start_flag == 0 && turn_in_end_flag == 0)	// �л�pid adc����ÿһС�ε�ƫ�ƣ�PID 
	{
		// adc��ȡ
		// error_mid = pid(error_in) - adc��ȡֵ   ��ע����ܼ�����
		// pid(error_mid)
		// PID��������⻷������(��һ��������)
//		P34 = 1;
		// ���µ��PWM = pid(error_mid)
		Sample_All_Chanel();
		Speed_Ctrl_mid(0);
		pid2_output = adc_state.output;
	}
//	else
//		P34 = 0;
//	
	
	/* ����ٶȿ��ƻ� */
	if(turn_out_start_flag == 0 && turn_in_start_flag == 0 && turn_in_end_flag == 0 && cnt % 5 == 0)	// �⻷pid �ٶȻ�����PI
	{
		cnt = 0;
		// PID���룺�л��������������
		// error = �л������ - ʵ���ٶ�
		// ���µ��PWM = pid(error)
		// �������º���Ӧ��ѹ��
		Speed_Ctrl_out(TargetSpeed,TargetSpeed);
//		printf("F: %d,%d\n",get_EncoderL(), get_EncoderR());
//		printf("F: %ld,%ld\n",Left_Speed_State.actual ,Right_Speed_State.actual);
		pid3_output_left  = Left_Speed_State.output;
		pid3_output_right = Right_Speed_State.output;
		
	}

	if(reset_1 == 666)
	{
		reset_1 = 0;
		
		enter_island_begin = enter_island_finish = leave_island_finish = leave_island_begin = 0;
//		turn_out_start_flag = 0;
//		turn_in_start_flag = 0;
//		turn_finish_flag = 0;
//		turn_in_end_flag = 0;
//		distance = 5555;

		P34 = 0;
	}
}

//========================================================================
// ����: Timer1_ISR_Handler
// ����: Timer1�жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2020-09-23
//========================================================================
void Timer1_ISR_Handler (void) interrupt TMR1_VECTOR		//���ж�ʱ�Ѿ������־
{
	// TODO: �ڴ˴�����û�����

}

//========================================================================
// ����: Timer2_ISR_Handler
// ����: Timer2�жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2020-09-23
//========================================================================
void Timer2_ISR_Handler (void) interrupt TMR2_VECTOR		//���ж�ʱ�Ѿ������־
{
	AUXINTIF &= ~0x01;	// ���жϱ�־
	// TODO: �ڴ˴�����û�����
	
}

//========================================================================
// ����: Timer3_ISR_Handler
// ����: Timer3�жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2020-09-23
//========================================================================
void Timer3_ISR_Handler (void) interrupt TMR3_VECTOR		//���ж�ʱ�Ѿ������־
{
	AUXINTIF &= ~0x02;	// ���жϱ�־
	// TODO: �ڴ˴�����û�����

}

//========================================================================
// ����: Timer4_ISR_Handler
// ����: Timer4�жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2020-09-23
//========================================================================
void Timer4_ISR_Handler (void) interrupt TMR4_VECTOR		//���ж�ʱ�Ѿ������־
{
	AUXINTIF &= ~0x04;	// ���жϱ�־
	// TODO: �ڴ˴�����û�����
	
}
