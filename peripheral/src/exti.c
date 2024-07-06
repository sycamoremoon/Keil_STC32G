#include "exti.h"

#pragma userclass (near=CEVENT)	
//	CEVENT_EXPORT(0,Exti_config,NULL);
#pragma userclass (near=default)
	
void Exti_config(void)
{
	EXTI_InitTypeDef	Exti_InitStructure;							//结构定义

	Exti_InitStructure.EXTI_Mode      = EXT_MODE_Fall;//中断模式,   EXT_MODE_RiseFall,EXT_MODE_Fall
	Ext_Inilize(EXT_INT0,&Exti_InitStructure);				//初始化
	NVIC_INT0_Init(ENABLE,Priority_3);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3

	Exti_InitStructure.EXTI_Mode      = EXT_MODE_Fall;//中断模式,   EXT_MODE_RiseFall,EXT_MODE_Fall
	Ext_Inilize(EXT_INT1,&Exti_InitStructure);				//初始化
	NVIC_INT1_Init(ENABLE,Priority_0);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3

	NVIC_INT2_Init(ENABLE,Priority_0);		//中断使能, ENABLE/DISABLE; 无优先级,只能为最低优先级
	NVIC_INT3_Init(ENABLE,Priority_0);		//中断使能, ENABLE/DISABLE; 无优先级，只能为最低优先级
//	NVIC_INT4_Init(ENABLE,NULL);		//中断使能, ENABLE/DISABLE; 无优先级
}