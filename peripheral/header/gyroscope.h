#ifndef	__GYROSCOPE_H__
#define __GYROSCOPE_H__

#include "config.h"

extern float Acc_x,Acc_y,Gyro_z,Angle_z;
extern float fil_Acc_x,fil_Acc_y,fil_Gyro_z;
extern float Angle_z,Angle_Z;
extern float Last_tar_angle;
extern double A_acc[2],V_acc[2],X_acc[2];
extern int gyro_i;
extern int moto_flag;



void Get_accdata(void);
void Get_gyro(void);
//void Icm20602_odometer(float Acc_x,float Acc_y);
void Get_angle(void);
void ICM20602_newValues(void);
float Turn_optimize(float Tar_angle,float Last_tar_angle);



#endif