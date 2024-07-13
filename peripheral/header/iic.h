#ifndef 	__IIC_H__
#define 	__IIC_H__

#include "config.h"



//定义是否使用软件IIC
#define 	USE_SOFT_IIC		(0)

//IIC引脚定义
#define		SCL_GPIO_PIN	(P25)
#define 	SDA_GPIO_PIN	(P24)
#define 	IIC_GPIO					GPIO_P2
#define 	IIC_SCL_PIN   				GPIO_Pin_5				// IIC SCL 引脚
#define 	IIC_SDA_PIN  				GPIO_Pin_4				// IIC SDA 引脚

#if	(USE_SOFT_IIC == 1)
	//软件 IIC 驱动
	#define 	SDA0()       SDA_GPIO_PIN=0	//IO口输出低电平
	#define 	SDA1()       SDA_GPIO_PIN=1	//IO口输出高电平  
	#define 	SCL0()       SCL_GPIO_PIN=0	//IO口输出低电平
	#define 	SCL1()       SCL_GPIO_PIN=1	//IO口输出高电平
	#define 	ack 			(1)      	//主应答
	#define 	no_ack 			(0)   	//从应答
	
#else	//use hardware IIC
	#define IIC_SEND_OK 	0
	#define IIC_SEND_FAIL	1
	#define WAIT_TIME		63	//设置等待时长
	
#endif	//USE_SOFT_IIC

void iic_write_reg(uint8 dev_add, uint8 reg,uint8 dat);
uint8 iic_read_reg(uint8 dev_add, uint8 reg);
void iic_read_regs(uint8 dev_add, uint8 reg, uint8 *dat_add, uint8 dat_num);
void iic_transfer(const uint8 dev_add, const uint8 *write_add, uint8 write_num, uint8 *read_add, uint8 read_num);
void iic_init(void);

#endif	//__IIC_H__