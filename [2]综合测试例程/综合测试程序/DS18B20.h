#ifndef _DS18B20_H_
#define _DS18B20_H_


#include "main.h"

sbit LED_CHOICE = P2^3;
sbit SEG_CHOICE = P2^4;

sbit DQ=P2^7;

void Read_Temp();
void Temp_to_Buff();
void Display();
								
								
#endif 
