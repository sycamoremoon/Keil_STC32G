#ifndef 	__IIC_H__
#define 	__IIC_H__

#include "config.h"

//定义是否使用软件IIC
#define 	USE_SOFT_IIC		(1)
	
#ifdef USE_SOFT_IIC

	//软件 IIC 驱动引脚定义
	#define		SCL_GPIO_PIN	P15
	#define 	SDA_GPIO_PIN	P14
	#define 	IIC_GPIO					GPIO_P1
	#define 	IIC_SCL_PIN   				GPIO_Pin_5				// 软件 IIC SCL 引脚
	#define 	IIC_SDA_PIN  				GPIO_Pin_4				// 软件 IIC SDA 引脚

	#define 	SDA0()       SDA_GPIO_PIN=0	//IO口输出低电平
	#define 	SDA1()       SDA_GPIO_PIN=1	//IO口输出高电平  
	#define 	SCL0()       SCL_GPIO_PIN=0	//IO口输出低电平
	#define 	SCL1()       SCL_GPIO_PIN=1	//IO口输出高电平
	#define 	ack 			1      	//主应答
	#define 	no_ack 			0   	//从应答

#else	//use hardware IIC

#endif	//USE_SOFT_IIC


void iic_write_reg(uint8 dev_add, uint8 * reg_add, uint8 reg_num,uint8 dat);
void iic_write_regs(uint8 dev_add, uint8 * reg_add, uint8 reg_num, uint8 *dat_add, uint8 dat_num);
uint8 iic_read_reg(uint8 dev_add, uint8 * reg_add, uint8 reg_num);
void iic_read_regs(uint8 dev_add, uint8 * reg_add, uint8 reg_num, uint8 *dat_add, uint8 dat_num);
void iic_init(void);

#endif	//__IIC_H__