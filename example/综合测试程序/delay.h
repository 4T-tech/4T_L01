#ifndef _DELAY_H_
#define _DELAY_H_

#include "main.h"
/******��ʱ����******/
static void Delaym1us();


//����STC12��Ƭ��us��ʱ����
//����˵�����ⲿ����
void delay_us(unsigned int time);

/*5us��ʱ*/
void delay_5us();
/*1Ms��ʱ*/
void delay(unsigned int z);
void delay_ADC (unsigned int ms);

#endif