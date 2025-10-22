#include "uart.h"

void UART_Init() 
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xFD;			//设置定时初始值
	TH1 = 0xFD;			//设置定时重载值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
}

void sendByte(unsigned char dat)
{
	SBUF=dat;
	while(!TI);
	TI=0;
}

//void sendString(unsigned char *dat)
//{
//	while(*dat!='\0')
//	{
//		sendByte(*dat++);
//	}
//}

char putchar(char c)
{
	sendByte(c);
	return c;
}