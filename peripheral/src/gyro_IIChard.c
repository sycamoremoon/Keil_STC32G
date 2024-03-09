/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
	接线定义：
	------------------------------------ 
	ICM20602模块(IIC通信)   单片机                        
	SCL              		查看SEEKFREE_ICM20602.h文件中的ICM20602_SCL_PIN宏定义
	SDA              		查看SEEKFREE_ICM20602.h文件中的ICM20602_SDA_PIN宏定义
	------------------------------------ 
 ********************************************************************************************************************/

#include "gyro_IIChard.h"
#include "gyro_IICsoft.h"
#include "STC32G_I2C.h"

//========================================================================
// 函数: void I2C_Init(I2C_InitTypeDef *I2Cx)
// 描述: I2C初始化程序.
// 参数: I2Cx: 结构参数,请参考I2C.h里的定义.
// 返回: none.
// 版本: V1.0, 2012-11-22
//========================================================================
void I2C_Init_hard(I2C_InitTypeDef *I2Cx)
{
	if(I2Cx->I2C_Mode == I2C_Mode_Master)
	{
		I2C_Master();						//设为主机	
		I2CMSST = 0x00;						//清除I2C主机状态寄存器
		I2C_SetSpeed(I2Cx->I2C_Speed);
		if(I2Cx->I2C_MS_WDTA == ENABLE)		I2C_WDTA_EN();				//使能自动发送
		else									I2C_WDTA_DIS();			//禁止自动发送
	}
	else
	{
		I2C_Slave();						//设为从机
		I2CSLST = 0x00;						//清除I2C从机状态寄存器
		I2C_Address(I2Cx->I2C_SL_ADR);
		if(I2Cx->I2C_SL_MA == ENABLE)		I2C_MATCH_EN();			//从机地址比较功能，只接受相匹配地址
		else									I2C_MATCH_DIS();	//禁止从机地址比较功能，接受所有设备地址
	}
	
	I2C_Function(I2Cx->I2C_Enable);
}



//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 ICM20602 加速度计数据
// 参数说明     void
// 返回参数     void
// 使用示例     icm20602_get_acc();                                             // 执行该函数后，直接查看对应的变量即可
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void icm20602_get_acc_hard (void)
{
    uint8 dat[6];

	I2C_ReadNbyte(ICM20602_DEV_ADDR, ICM20602_ACCEL_XOUT_H, dat, 6);   /*  DeviceAddress,WordAddress,First Data Address,Byte lenth   */
    icm20602_acc_x = (int16)(((uint16)dat[0] << 8 | dat[1]));
    icm20602_acc_y = (int16)(((uint16)dat[2] << 8 | dat[3]));
    icm20602_acc_z = (int16)(((uint16)dat[4] << 8 | dat[5]));
}



//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取ICM20602陀螺仪数据
// 参数说明     void
// 返回参数     void
// 使用示例     icm20602_get_gyro();                                            // 执行该函数后，直接查看对应的变量即可
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void icm20602_get_gyro_hard (void)
{
    uint8 dat[6];

	I2C_ReadNbyte(ICM20602_DEV_ADDR, ICM20602_GYRO_XOUT_H, dat, 6);   /*  DeviceAddress,WordAddress,First Data Address,Byte lenth   */
    icm20602_gyro_x = (int16)(((uint16)dat[0] << 8 | dat[1]));
    icm20602_gyro_y = (int16)(((uint16)dat[2] << 8 | dat[3]));
    icm20602_gyro_z = (int16)(((uint16)dat[4] << 8 | dat[5]));
}
