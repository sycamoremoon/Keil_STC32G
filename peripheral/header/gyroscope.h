#include "cevent.h"

/*********************************************************************************************************************
 * Copyright (c) 2018,逐飞科技
 * 部分代码改自SEEKFREE
			接线定义：
			------------------------------------ 
			软件IIC
			SCL	查看本头文件内的MPU6050_SCL_PIN宏定义
			SDA 查看本头文件内的MPU6050_SDA_PIN宏定义  
			------------------------------------ 
 ********************************************************************************************************************/

#ifndef __GYROSCOPE_H__
#define __GYROSCOPE_H__

#include "config.h"

//=====================================================软件 IIC 驱动====================================================
#define MPU6050_SCL_PIN 	P40						// 软件 IIC SCL 引脚
#define MPU6050_SDA_PIN 	P41 					// 软件 IIC SDA 引脚
#define MPU6050_IIC_DELAY 	(0)						// 模拟IIC延时函数中J的值
//=====================================================软件 IIC 驱动====================================================

// 定义MPU6050内部地址
#define	MPU6050_DEV_ADDR	    0xD0>>1	//IIC写入时的地址字节数据，+1为读取

#define	SMPLRT_DIV				0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	MPU6050_CONFIG          0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG				0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	        0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define INT_PIN_CFG	  			0x37    //设置6050辅助I2C为直通模式寄存器
#define	ACCEL_XOUT_H	        0x3B
#define	ACCEL_XOUT_L	        0x3C
#define	ACCEL_YOUT_H	        0x3D
#define	ACCEL_YOUT_L	        0x3E
#define	ACCEL_ZOUT_H	        0x3F
#define	ACCEL_ZOUT_L	        0x40
#define	GYRO_XOUT_H				0x43
#define	GYRO_XOUT_L				0x44	
#define	GYRO_YOUT_H				0x45
#define	GYRO_YOUT_L				0x46
#define	GYRO_ZOUT_H				0x47
#define	GYRO_ZOUT_L				0x48
#define User_Control 			0x6A    //关闭6050对辅助I2C设备的控制
#define	PWR_MGMT_1				0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I				0x75	//IIC地址寄存器(默认数值0x68，只读)

// 内部声明
extern int16 mpu6050_acc_x,mpu6050_acc_y,mpu6050_acc_z;
extern int16 mpu6050_gyro_x, mpu6050_gyro_y, mpu6050_gyro_z;
extern float accelX_real, accelY_real, accelZ_real;

// 函数原型
uint8 mpu6050_init(void);				// 初始化MPU6050
void mpu6050_get_accdata(void);			// 获取加速度计数据
void mpu6050_get_gyro(void);			// 获取陀螺仪数据

#endif
