#include "delay.h"

void delay_Nus(unsigned char us)
{
	unsigned char d;

	while(us--)
	{
		d=6;
		while(d--);
	}
}


void delay_Nms(unsigned char ms)
{	
	while(ms--)
	{
		delay_Nus(250);
		delay_Nus(250);
		delay_Nus(250);
		delay_Nus(250);
	}
}


void delay_Ns(unsigned char s)
{	
	while(s--)
	{
		delay_Nms(250);
		delay_Nms(250);
		delay_Nms(250);
		delay_Nms(250);
	}
}

