
#include "stc12.h"
#include "intrins.h"
#include "delay.h"
#define uchar unsigned char
#define uint unsigned  int
/******��ʱ����******/
static void Delaym1us()
{
	_nop_();
}

//����STC12��Ƭ��us��ʱ����
//����˵�����ⲿ����
void delay_us(uint time)
{
	int i;
	for(i=0; i<time; i++)
	{
		Delaym1us();
	}
}

/*5us��ʱ*/
void delay_5us()  
{
	delay_us(5);
}

/*1Ms��ʱ*/
void delay(uint z)
{
	uint x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--);
}

void delay_ADC (unsigned int ms)
{
    unsigned int i, j;
    for (i = ms; i > 0; i--)
    for (j = 113; j > 0; j--);
}