//#include "stc12.h"
//#include "delay.h"

//#include <intrins.h>
//#define uchar unsigned char
//#define uint unsigned int
//	
//sbit DQ=P2^7;

//uchar code SegCode[]=
//{0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};//���������� 0~9
//uchar code BitCode[]={0x08,0x04,0x02,0x01};						//λ��:P0 �ڡ�1����Ч
//uchar DispBuf[4]={0,0,0,0};						//��ʾ������
//bit zf;
//int Temp;



//void Display()
//{
//	uchar i;
//	for(i=0;i<4;i++)
//	{
//		SEG_CHOICE=1;
//		P0=SegCode[DispBuf[i]];
//		SEG_CHOICE=0;
//		P0=BitCode[i];
//		delay(10);
//	}
//}

//void DS18B20_Init(void)
//{
//	DQ=0;
//	Delay_us(80);				//���������ϲ���Լ800��s�ĵ͵�ƽ
//	DQ =1;
//	Delay_us(6);					//����������,��ʱ60��s
//	for(;;)
//	{
//		if(!DQ)break;			//��ȡ���ߵ�ƽ,��DQ=0��λ�ɹ�,DO=1��λʧ��
//	}
//	DQ=1;
//	Delay_us(10);
//}

//uchar DS18B20_RByte(void)
//{
//	uchar i=0;
//	uchar dat=0;
//	for(i=8;i>0;i--)
//	{
//		DQ=0;
//		dat>>=1;
//		DQ =1;
//		Delay_us(10);				//��������,��ʱ 10��s
//		if(DQ)dat|=0x80;
//		Delay_us(50);				//��ʱ 50 us
//	}
//	return(dat);
//}

//void DS18B20_WByte(uchar dat)
//{
//	uchar i=0;
//	for(i=8;i>0;i--)
//	{
//		DQ=0;
//		DQ=dat&0x01;					//����ǰ���ݷŵ�������,��λ����,��λ�ں�
//		Delay_us(50);						//��ʱ 50 ws
//		DQ =1;								//��������,Ϊд����һλ��׼��
//		dat>>=1;
//	}
//	
//}

//void ReadRomCode(unsigned char *romcode)
//{
//	unsigned char i;
//	init;//��λ
//	DS18B20_WByte(0x33);//��ȡROMָ��
//	for(i=0;i<8;i++)
//	{
//		romcode[i]=DS18B20_RByte();//��64λrom��
//	}
//	
//}
//void Read_Temp(void)			//��ȡ DS18B20оƬ�¶�ֵ
//{
//	uchar lo,hi;
//	int t;
//	DS18B20_Init();					//DS18B20оƬ��ʼ��
//	DS18B20_WByte(0xCC);		//���������кŵ�����(�����Ͻ���һ��DS18B20оƬ��
//	DS18B20_WByte(0x44);		//�����¶�ת��
//	DS18B20_Init();
//	DS18B20_WByte(0xCC);
//	DS18B20_WByte(0xBE);		//���ݴ�������
//	lo=DS18B20_RByte();			//���¶�ֵ��λ
//	hi=DS18B20_RByte();			//���¶�ֵ��λ
//	t=hi;
//	t<<= 8;
//	t =t|lo;
//	if(t<0)
//	{
//		zf=1;									//�ø�����־
//		t=~t+1;								//ȡ��
//	}
//	Temp=t;
//}

//void Temp_to_Buff()				//����ʵ���¶�ֵ,�����¶�ֵ����ʾ������
//{	
//	uchar shi,ge,xshu;
//	float ft;
//	uint tt;
//	ft=Temp*0.0625;					//����ʵ���¶�ֵ
//	tt= ft*10+0.5;					//����С��(����1λС��)���¶�ֵ��չ������
//	shi=tt/100;							//ʵ���¶�ֵ��ʮλ(��չ��İ�λ)
//	tt =tt% 100;
//	ge= tt/10;							//ʵ���¶�ֵ�ĸ�λ(��չ���ʮλ)
//	xshu =tt%10;						//ʵ���¶�ֵ��С��λ(��չ��ĸ�λ)
//	if(zf==1)
//	DispBuf[3]=0x40;				//����λ��ʾ���š�_��
//	else
//	DispBuf[3]=0x00;							//����λ����ʾ(����)
//	DispBuf[2]=SegCode[shi];			//��ʾʮλ
//	DispBuf[1]=SegCode[ge]&0x80;	//��ʾ��λ,ͬʱ��ʾ dp
//	DispBuf[0]=SegCode[xshu];			//��ʾС��λ
//}

//void main(void)
//{
//	uchar i;
//	P4SW|=0X70;
//	P0=0xff;
//	LED_CHOICE=1;
//	LED_CHOICE=0;
//	while(1)
//	{
//		Read_Temp();
//		Temp_to_Buff();
//		for(i=0;i<100;i++)
//		Display();
//	}
//}
//	
//	
//	
//	