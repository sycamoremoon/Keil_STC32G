#ifndef 	__IIC_H__
#define 	__IIC_H__

#include "config.h"


void iic_write_reg(uint8 dev_add, uint8 * reg_add, uint8 reg_num,uint8 dat);
uint8 iic_read_reg(uint8 dev_add, uint8 * reg_add, uint8 reg_num);
void iic_read_regs(uint8 dev_add, uint8 * reg_add, uint8 reg_num, uint8 *dat_add, uint8 dat_num);
void iic_init(void);

#endif	//__IIC_H__