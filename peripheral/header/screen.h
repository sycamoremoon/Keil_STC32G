#include "config.h"

#ifndef __SCREEN_H__
#define __SCREEN_H__

//引脚定义
#define SCL         P43
#define SDA         P40
#define RST         P13
#define DC          P07
#define CS          P21
#define BLK         P05

//屏幕数据定义
#define X_MAX_PIXEL	        128
#define Y_MAX_PIXEL	        128

//常用颜色值定义
#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//灰色0 3165 00110 001011 00101
#define GRAY1   0x8410      	//灰色1      00000 000000 00000
#define GRAY2   0x4208      	//灰色2  1111111111011111

//用户调用函数
void Screen_Init(void);
void Fill_With_Color(int color);
void Screen_Show_String(u16 x, u16 y, u16 fc, u16 bc, u8 *string);
void Screen_DrawPoint(u16 x,u16 y,u16 Data);
void Screen_ShowChar(uint16 x,uint16 y,u16 font_color, u16 background_color,const char dat);
void Screen_ShowInt(uint16 x,uint16 y,u16 font_color, u16 background_color,const int16 num);
void Screen_ShowFloat(uint16 x,uint16 y,u16 font_color, u16 background_color,const float flot);
//ASCII字库
extern unsigned char xdata TFT_ASCII[];

#endif 

