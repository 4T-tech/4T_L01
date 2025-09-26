
#include "stc12.h"
#include "intrins.h"
#define uchar unsigned char
#define uint unsigned  int
/******延时函数******/
static void Delaym1us();


//基于STC12单片机us延时函数
//函数说明：外部调用
void delay_us(uint time);

/*5us延时*/
void delay_5us();
/*1Ms延时*/
void delay(uint z);
void delay_ADC (unsigned int ms);