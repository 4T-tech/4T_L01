#include "delay.h"

/******延时函数******/
static void Delaym1us()
{
	_nop_();
}

//基于STC12单片机us延时函数
//函数说明：外部调用
void delay_us(unsigned int time)
{
	int i;
	for(i=0; i<time; i++)
	{
		Delaym1us();
	}
}

/*5us延时*/
void delay_5us()  
{
	delay_us(5);
}

/*1Ms延时*/
void delay(unsigned int z)
{
	unsigned int x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--);
}

void delay_ADC (unsigned int ms)
{
    unsigned int i, j;
    for (i = ms; i > 0; i--)
    for (j = 113; j > 0; j--);
}