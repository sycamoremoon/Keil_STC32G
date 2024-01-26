#include "config.h"
//char *USER_DEVICEDESC = NULL;
//char *USER_PRODUCTDESC = NULL;
//char *USER_STCISPCMD = "@STCISP#";                      //设置自动复位到ISP区的用户接口命令
int main(void)
{
	WTST = 0;		//设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
	EAXSFR();		//扩展SFR(XFR)访问使能 
	CKCON = 0;      //提高访问XRAM速度
	
	/******************************************************************/
	//初始化
//	EAXFR = 1;	
//	usb_init();
//	EA =1;
	
	
	//初始化结束
	/******************************************************************/

	
	P2_MODE_IO_PU(GPIO_Pin_All);		//P6 设置为准双向口
	/******************************************************************/
	//主循环
	while(1)
	{
		delay_ms(250);

		P2=0;
		P31=0;
		P32=0;
		
	}
}




