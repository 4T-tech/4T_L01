#ifndef _DELAY_H_
#define _DELAY_H_

#include "main.h"
/******延时函数******/
static void Delaym1us();


//基于STC12单片机us延时函数
//函数说明：外部调用
void delay_us(unsigned int time);

/*5us延时*/
void delay_5us();
/*1Ms延时*/
void delay(unsigned int z);
void delay_ADC (unsigned int ms);

#endif