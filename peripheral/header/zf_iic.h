/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		iic
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-4-14
 ********************************************************************************************************************/

#ifndef __ZF_IIC_H
#define __ZF_IIC_H


#include "config.h"

typedef enum
{
    IIC_1 = 0,
    IIC_2 = 1,
    IIC_3 = 2,	//STC8H 48脚核心板没有该组引脚。
    IIC_4 = 3,
	
} IICN_enum;

typedef enum
{
    IIC1_SCL_P15 = 0x00, IIC1_SDA_P14,
    IIC2_SCL_P25 = 0x10, IIC2_SDA_P24,
    IIC3_SCL_P77 = 0x20, IIC3_SDA_P76,	//STC8H 48脚核心板没有该组引脚。
    IIC4_SCL_P32 = 0x30, IIC4_SDA_P33,
} IIC_PIN_enum;






#define IIC_SEND_OK 	0
#define IIC_SEND_FAIL	1




void  Zf_IIC_init(IICN_enum iic_n, IIC_PIN_enum scl_pin, IIC_PIN_enum sda_pin, uint32 wait_time);
uint8 Zf_IIC_write_reg(uint8 dev_add, uint8 reg, uint8 dat);
uint8 Zf_IIC_read_reg(uint8 dev_add, uint8 reg, uint8 *dat);
uint8 Zf_IIC_read_reg_bytes(uint8 dev_add, uint8 reg, uint8 *dat, uint8 num);
void Zf_IIC_change_pin(IICN_enum iic_n,IIC_PIN_enum scl_pin,IIC_PIN_enum sda_pin);
uint8 Zf_IIC_transfer(const uint8 dev_add, const uint8 *write_add, uint8 write_num, uint8 *read_add, uint8 read_num);
#endif