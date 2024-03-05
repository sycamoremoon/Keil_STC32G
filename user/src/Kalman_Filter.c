#include "Kalman_Filter.h"
#include "gyroscope.h"
#include <math.h> 

float Gyro_z=0;
float fil_Acc_x,fil_Acc_y,fil_Gyro_z;
float Angle_z=0;
float Angle_Z=90;
float coe_Gyro_z=0.2;
float ICM20602_FIFO[11];
int moto_flag=0;
int gyro_i=0;
#define dt 0.01

/**************************************************************************
函数功能：递推平均滤波算法 处理角速度
入口参数：无
返回  值：无
**************************************************************************/
void ICM20602_newValues()
{
	 float sum=0;
	 static float gyro[100],sum_gyro;
	 static int gyro_flag=0,Gyro_flag;
	 
	 get_icm20602_gyro();		
		if(gyro_flag==0)
	 {		 
		  gyro[gyro_i]=icm20602_gyro_z;
		  fil_Gyro_z=0.0;
		  gyro_i++;
		 if(gyro_i==99)
		 {
			 moto_flag=1;
			 for(gyro_i=0;gyro_i<100;gyro_i++)
			 {
				 sum_gyro+=gyro[gyro_i];
			 }
			 gyro_flag=1;
		 }
	 } 
	 if(gyro_flag==1)
	 {
    Gyro_z = (float)(icm20602_gyro_z-sum_gyro/100)/16.3835;
	  if(abs(Gyro_z)<3)//角速度小于3时  默认为小车静止  
	  {
		  Gyro_z=0;
	  }
	  for(Gyro_flag=1;Gyro_flag<10;Gyro_flag++)
		{	
		  ICM20602_FIFO[Gyro_flag-1]=ICM20602_FIFO[Gyro_flag];//FIFO 操作
		}
	  ICM20602_FIFO[9]=Gyro_z;
	  for(Gyro_flag=0;Gyro_flag<10;Gyro_flag++)
		{	            
			sum+=ICM20602_FIFO[Gyro_flag];//求当前数组的合，再取平均值
		}
	  fil_Gyro_z=sum/10;
	}
}		
/**************************************************************************
函数功能：对角速度积分 得到角度
入口参数：无
返回  值：无
**************************************************************************/
void Get_angle()
{
   ICM20602_newValues();
	 Angle_Z-=fil_Gyro_z*dt;
	 if(Angle_Z>=360) Angle_Z=Angle_Z-360;
	 if(Angle_Z<=-360) Angle_Z=Angle_Z+360;
}

