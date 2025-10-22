#include "uart.h"

void UART_Init() 
{
	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//8λ����,�ɱ䲨����
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TMOD |= 0x20;		//���ö�ʱ��ģʽ
	TL1 = 0xFD;			//���ö�ʱ��ʼֵ
	TH1 = 0xFD;			//���ö�ʱ����ֵ
	ET1 = 0;			//��ֹ��ʱ���ж�
	TR1 = 1;			//��ʱ��1��ʼ��ʱ
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