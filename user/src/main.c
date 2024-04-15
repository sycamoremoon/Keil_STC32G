#include "config.h"

#pragma userclass (near=CEVENT)	
	CEVENT_EXPORT(0,NULL,NULL);
#pragma userclass (near=default)

int main(void)
{	
	uint16 speed = 0;
	/******************************************************************/
	ceventInit();
	ceventPost(0);

	//初始化结束
	/******************************************************************/
	EA =1;
	/******************************************************************/

	//主循环
	
	while(1)
	{	

	}

}
