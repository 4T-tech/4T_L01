//#include "stc12.h"
//#include <intrins.h>

//	
//sbit LED_CHOICE = P2^3;
//sbit SEG_CHOICE = P2^4;

//sbit CS=P4^5;
//sbit CLK=P4^1;
//sbit DOUT=P4^3;
//unsigned char code SegCode[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};//����������0~9
//unsigned char code BitCode[]={0x08,0x04,0x02,0x01};	//λ��:P0�ڡ�1��
//																						//��Ч
//unsigned char DispBuf[4];								//��ʾ������
//unsigned int Volt;
//void delay_ADC (unsigned int ms)
//{
//    unsigned int i, j;
//    for (i = ms; i > 0; i--)
//    for (j = 113; j > 0; j--);
//}


//unsigned char TLC549_ADC(void)
//{
//	unsigned char i,temp=0;
//	CS=1;
//	CLK =0;
//	CS=0;
//	_nop_();
//	_nop_();
//	for(i=0;i<8;i++)
//	{
//		temp=temp<<1;
//		if (DOUT == 1) temp|=0x01;
//		CLK=1;
//		_nop_();
//		_nop_();
//		CLK =0;
//		_nop_();
//	}
//	_nop_();
//	_nop_();
//	CS =1;
//	return(temp);
//	
//}



//void AD_Buff()				//ADת��ֵ����ʾ������(BCD��)
//{
//	DispBuf[3]= Volt /1000;		//ǧλ
//	DispBuf[2]= (Volt/100)%10;	//��λ
//	DispBuf[1]=	(Volt /10)%10;	//ʮλ
//	DispBuf[0]= Volt %10;			//��λ
//}

//void Display()
//{

//	unsigned char i;
//	SEG_CHOICE=1;
//	P0=0xff;
//	for(i=0;i<4;i++)
//	{
//		if(i==3)
//		{
//			SEG_CHOICE=1;
//			P0=SegCode[DispBuf[i]]&0x7f;
//			SEG_CHOICE=0;
//			P0=BitCode[i];
//			delay_ADC(10);
//		}
//		else
//		{
//			SEG_CHOICE=1;
//			P0=SegCode[DispBuf[i]];
//			SEG_CHOICE=0;
//			P0=BitCode[i];
//			delay_ADC(10);
//		}
//	}
//	
//}

////void main(void)
////{	
////	unsigned char ad;
////	P4SW|=0Xff;
////	P0=0xff;
////	LED_CHOICE=1;
////	LED_CHOICE=0;
////	while(1)
////	{
////		ad=TLC549_ADC();
////		Volt=5*ad/256.0*1000.0;
////		AD_Buff();
////		Display();
////	}
////		
////}
