/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/

#ifndef		__CONFIG_H
#define		__CONFIG_H

//========================================================================
//                               主时钟定义
//========================================================================

//#define MAIN_Fosc		22118400L	//定义主时钟
//#define MAIN_Fosc		12000000L	//定义主时钟
//#define MAIN_Fosc		11059200L	//定义主时钟
//#define MAIN_Fosc		 5529600L	//定义主时钟
#define MAIN_Fosc		24000000L	//定义主时钟

//========================================================================
//                                头文件
//========================================================================
/******************************库头文件*************************************/
#include 	"stc32g.h"
#include 	"type_def.h"
#include	"STC32G_GPIO.h"
#include	"STC32G_Delay.h"
#include  	"stc32_stc8_usb.h"
#include 	"STC32G_PWM.h"
#include 	"mpu6050.h"

/******************************系统头文件*************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

/******************************自定义头文件*************************************/
#include "cpost.h"
#include "cevent.h"
#include "pid.h"


//========================================================================
//                             外部函数和变量声明
//========================================================================



/******************************基本参数*************************************/



#endif
