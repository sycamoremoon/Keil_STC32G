#include "control.h"

PID_Calibration		PID_AngleZ 		= {-20,0,-450};		// �ڻ������ǵ�PID��������PD
PID_Calibration 	PID_adc 		= {-660,-10,-35};	// �л�adc��PID���� 20
PID_Calibration 	PID_out_left 	= {450,1300,30};	// �⻷���ٶȵ�PID����hong{290,195,57};
PID_Calibration		PID_out_right 	= {450,1300,35};	// �⻷���ٶȵ�PID����lv  {290,195,57};

PID_State	AngleZ_state		= {0};				//accy״̬����
PID_State 	adc_state 			= {0};				//adc״̬����
PID_State 	Left_Speed_State 	= {0};				//�����ٶ�״̬����
PID_State 	Right_Speed_State 	= {0};				//�ҵ���ٶ�״̬����

long TargetSpeed = 0;
long targetspeed_backup = 0;

long pid1_output = 0;
long pid2_output = 0;
long pid3_output_left = 0;
long pid3_output_right = 0;
long output_left = 0;
long output_right = 0;
long AngleZ_output = 0;

extern uint8 start_car_signal;	//�����ź�

/// @brief ͨ��PID�㷨��������ٶȴﵽĿ���ٶ�
/// @param Left_Speed ��������������Ŀ���ٶ�
/// @param Right_Speed ���������ҵ����Ŀ���ٶ�
// �ڻ� error_in = y_acc - 0
void Speed_Ctrl_in(long AngleZ_target)
{
	//��ȡ��ʵaccy
	AngleZ_state.actual = (long)Angle_Z * 100;
	//��ȡĿ��accy
	AngleZ_state.target = AngleZ_target * 100;
	
	pid_location(&PID_AngleZ,&AngleZ_state);
//	Update_Motors(&accy_state,&accy_state,&Turn_State);
}

// �л�
void Speed_Ctrl_mid(long adc_target)
{
	//��ȡ��ʵadc
	adc_state.actual = (long)Get_Regularized_Signal_Data(All_Signal_Data);
//	printf("adc_state:%ld,%ld\n",adc_state.integral,adc_state.actual);
	//adc_state.actual = 0;
	//��ȡĿ��adc
	adc_state.target = adc_target;

	pid_location(&PID_adc,&adc_state);

}

// �⻷
void Speed_Ctrl_out(long Left_Speed,long Right_Speed)
{
	//��ȡ��ʵ�ٶ�*MAXSPEED/MAXENCODER
	Left_Speed_State.actual = get_EncoderL();
	Right_Speed_State.actual = get_EncoderR();
//	
	
	//��ȡĿ���ٶ�
	Left_Speed_State.target = Left_Speed * MAXENCODER_L / MAXSPEED;
	Right_Speed_State.target = Right_Speed * MAXENCODER_R / MAXSPEED;
	
	pid_increase(&PID_out_left,&Left_Speed_State);
	pid_increase(&PID_out_right,&Right_Speed_State);
//	Update_Motors(&Left_Speed_State,&Right_Speed_State,&Turn_State);
}



/// @brief ͨ��PID�㷨��������ٶȴﵽĿ���ٶȺ�Ŀ��Ƕ�
/// @param Gyro ��������С����Ŀ��Ƕ�
//void Turn_Ctrl(unsigned int Gyro)
//{
//	
//}

/// @brief ��׼���������������Ӿ�������������磺���ţ��޷�����һ���Ȳ���
/// @param left_state 
/// @param right_state 
//void Regularization(PID_State * left_state,PID_State * right_state)
//{
//	
//}

/// @brief ���µ��PWMת��
/// @param left_state 
/// @param right_state 
/// @param gyro_state 
void Update_Motors(PID_State * left_state,PID_State * right_state,PID_State * gyro_state)
{
	Set_Rmotor_Speed(right_state->output + gyro_state->output);
	Set_Lmotor_Speed(left_state->output + gyro_state->output);
}

void Set_Motors(long left, long right)
{
	Set_Lmotor_Speed(left * MAXSPEED / MAXENCODER_L);
	Set_Rmotor_Speed(right * MAXSPEED / MAXENCODER_R);
}

void Stop_Car(void)		// С��ֹͣ
{
	if(TargetSpeed >= 50){
		if(TargetSpeed > 0) TargetSpeed = 0;
		else TargetSpeed = TargetSpeed - 100;
		memset((void*)&Left_Speed_State,0,sizeof(PID_State));
		memset((void*)&Right_Speed_State,0,sizeof(PID_State));
		adc_state.integral = 0;
	}else{
		start_car_signal = 0;
	}
}


