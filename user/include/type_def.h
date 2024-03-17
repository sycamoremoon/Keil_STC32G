/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCAI.com ---------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- BBS: www.STCAIMCU.com  -----------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/

#ifndef		__TYPE_DEF_H
#define		__TYPE_DEF_H

//========================================================================
//                               类型定义
//========================================================================

typedef unsigned char   u8;     //  8 bits 
typedef unsigned int    u16;    // 16 bits 
typedef unsigned long   u32;    // 32 bits 

typedef signed char     int8;   //  8 bits 
typedef signed int      int16;  // 16 bits 
typedef signed long     int32;  // 32 bits 

typedef unsigned char   uint8;  //  8 bits 
typedef unsigned int    uint16; // 16 bits 
typedef unsigned long   uint32; // 32 bits 

//========================================================================

#define	TRUE	1
#define	FALSE	0

//========================================================================
#ifndef NULL
#define NULL ((void *) 0)
#endif
//========================================================================

#define	Priority_0			0	//中断优先级为 0 级（最低级）
#define	Priority_1			1	//中断优先级为 1 级（较低级）
#define	Priority_2			2	//中断优先级为 2 级（较高级）
#define	Priority_3			3	//中断优先级为 3 级（最高级）

#define ENABLE		1
#define DISABLE		0

#define SUCCESS		0
#define FAIL		-1

//========================================================================
/////////////////////////////////////////////////

#define T22M_ADDR CHIPID11 //22.1184MHz
#define T24M_ADDR CHIPID12 //24MHz
#define T27M_ADDR CHIPID13 //27MHz
#define T30M_ADDR CHIPID14 //30MHz
#define T33M_ADDR CHIPID15 //33.1776MHz
#define T35M_ADDR CHIPID16 //35MHz
#define T36M_ADDR CHIPID17 //36.864MHz
#define T40M_ADDR CHIPID18 //40MHz
#define T44M_ADDR CHIPID19 //44.2368MHz
#define T48M_ADDR CHIPID20 //48MHz
#define VRT6M_ADDR CHIPID21 //VRTRIM_6M
#define VRT10M_ADDR CHIPID22 //VRTRIM_10M
#define VRT27M_ADDR CHIPID23 //VRTRIM_27M
#define VRT44M_ADDR CHIPID24 //VRTRIM_44M


#endif


