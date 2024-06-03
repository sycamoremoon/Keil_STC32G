#include "config.h"

#pragma userclass (near=CEVENT)	
	CEVENT_EXPORT(0,NULL,NULL);
#pragma userclass (near=default)
extern uint16 All_Signal_Data[4];

int main(void)
{	
	/******************************************************************/
	ceventInit();
	ceventPost(0);
	Screen_Init();
	Signal_Init();
	//初始化结束
	/******************************************************************/
	EA =1;
	/******************************************************************/

	//主循环
	
	while(1)
	{	
		
	}

}
