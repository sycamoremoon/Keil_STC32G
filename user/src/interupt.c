#include "interupt.h"


void turn_on(char * num)
{
	P2 = (1<<(*num));
}

	
void turn_off(char * num)
{
	P2 = ~(1<<(*num));
}
