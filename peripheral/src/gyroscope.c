#include "gyroscope.h"
#include "config.h"

#define dt 0.01

int16 gyro_gyro_x,gyro_gyro_y,gyro_gyro_z;
int16 gyro_acc_x,gyro_acc_y,gyro_acc_z;


float fil_Acc_x,fil_Acc_y;		//定义处理后的加速度值
float fil_Gyro_z;				//定义处理后的角速度值
float Gyro_z=0;					//定义的处理角速度的中间变量
float Angle_Z=90;				//设置初始角度为90度
float Gyroscope_FIFO[11];
int gyro_i=0;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取ICM20602加速度计数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void Get_gyro_accdata(void)
{
    uint8 dat[6];
    uint16 reg =  ICM20602_ACCEL_XOUT_H;
    iic_read_regs(ICM20602_DEV_ADDR, reg, 2 , dat, 6);  
    gyro_acc_x = (int16)(((uint16)dat[0]<<8 | dat[1]));
    gyro_acc_y = (int16)(((uint16)dat[2]<<8 | dat[3]));
    gyro_acc_z = (int16)(((uint16)dat[4]<<8 | dat[5]));
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取ICM20602陀螺仪数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void Get_gyro_gyro(void)
{
    uint8 dat[6];
    uint16 reg =  ICM20602_GYRO_XOUT_H;

    iic_read_regs(ICM20602_DEV_ADDR, reg, 2 , dat, 6);  
    gyro_gyro_x = (int16)(((uint16)dat[0]<<8 | dat[1]));
    gyro_gyro_y = (int16)(((uint16)dat[2]<<8 | dat[3]));
    gyro_gyro_z = (int16)(((uint16)dat[4]<<8 | dat[5]));
}

/**************************************************************************
函数功能：递推平均滤波算法 处理角速度
入口参数：无
返回  值：无
**************************************************************************/
void Gyroscope_newValues()
{
	float sum=0;
	static float gyro[100],sum_gyro;
	static int gyro_flag=0,Gyro_flag;

	Get_gyro_gyro();		// 获取角速度数据
	if(gyro_flag==0)
	{		 
		gyro[gyro_i]=gyro_gyro_z;
		fil_Gyro_z=0.0;
		gyro_i++;
		if(gyro_i==99)
		{
			for(gyro_i=0;gyro_i<100;gyro_i++)
			{
				sum_gyro+=gyro[gyro_i];
			}
			gyro_flag=1;
		}
	} 
	if(gyro_flag==1)
	{
		Gyro_z = (float)(gyro_gyro_z-sum_gyro/100)/16.3835;
		if(abs(Gyro_z)<3)//角速度小于3时  默认为小车静止  
		{
			Gyro_z=0;
		}
		for(Gyro_flag=1;Gyro_flag<10;Gyro_flag++)
		{	
			Gyroscope_FIFO[Gyro_flag-1]=Gyroscope_FIFO[Gyro_flag];//FIFO 操作
		}
		Gyroscope_FIFO[9]=Gyro_z;
		for(Gyro_flag=0;Gyro_flag<10;Gyro_flag++)
		{	            
			sum+=Gyroscope_FIFO[Gyro_flag];//求当前数组的合，再取平均值
		}
		fil_Gyro_z=sum/10;
	}
}		

/**************************************************************************
函数功能：对角速度积分 得到角度
入口参数：无
返回  值：无
**************************************************************************/
float Get_angle()
{
	Gyroscope_newValues();			//使用陀螺仪获取角速度
	Angle_Z-=fil_Gyro_z*dt;
	if(Angle_Z>=360) Angle_Z=Angle_Z-360;
	if(Angle_Z<=-360) Angle_Z=Angle_Z+360;
	
	return Angle_Z;
}



