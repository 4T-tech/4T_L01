
#include "stc12.h"
#include "intrins.h"
#define uchar unsigned char
#define uint unsigned  int
/******��ʱ����******/
static void Delaym1us();


//����STC12��Ƭ��us��ʱ����
//����˵�����ⲿ����
void delay_us(uint time);

/*5us��ʱ*/
void delay_5us();
/*1Ms��ʱ*/
void delay(uint z);
void delay_ADC (unsigned int ms);