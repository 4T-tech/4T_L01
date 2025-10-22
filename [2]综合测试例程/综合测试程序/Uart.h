#ifndef __UART_H__
#define __UART_H__

#include <main.h> 

void UART_Init();
void sendByte(unsigned char dat);
//void sendString(unsigned char *dat);
char putchar(char c);
#endif