#include "DS18B20.h"


unsigned char romcode[8];
int Temp;

unsigned char DispBuf[4]={1,1,1,1};						//��ʾ������

bit zf;


void Delay600us(void)	//@11.0592MHz
{
	unsigned char  i, j;

	_nop_();
	_nop_();
	i = 7;
	j = 112;
	do
	{
		while (--j);
	} while (--i);
}

void Delay60us(void)	//@11.0592MHz
{
	unsigned char  i;

	i = 163;
	while (--i);
}

void Delay45us(void)	//@11.0592MHz
{
	unsigned char  i;

	_nop_();
	_nop_();
	i = 121;
	while (--i);
}
void Delay100us(void)	//@11.0592MHz
{
	unsigned char  i, j;

	i = 2;
	j = 15;
	do
	{
		while (--j);
	} while (--i);
}



void Delay1ms(void)	//@11.0592MHz
{
	unsigned char  i, j;

	_nop_();
	i = 11;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
}


void Display()
{
	unsigned char i;
	for(i=0;i<4;i++)
	{
		SEG_CHOICE=1;
		P0=DispBuf[i];
		SEG_CHOICE=0;
		P0=BitCode[i];
		Delay1ms();
	}
}

unsigned char DS18B20_Init(void)
{
	unsigned char x=0;
	DQ=1;
	_nop_();
	_nop_();
	DQ=0;
	delay_us(600);
	DQ=1;
	delay_us(60);
	x=DQ;
	delay_us(100);
//	printf("x=%bu\n",x);
	return x;
}

unsigned char DS18B20_RByte(void)
{
	unsigned char i=0;
	unsigned char dat=0;
	for(i=8;i>0;i--)
	{
		DQ=1;
		_nop_();
		_nop_();
		DQ=0;
		dat>>=1;
		DQ=1;
		
		if(DQ)
			dat|=0x80;
		delay_us(60);
	}
		return(dat);		
}

void DS18B20_WByte(unsigned char dat)
{
	unsigned char i=0;
	for(i=8;i>0;i--)
	{
		DQ=0;
		
		DQ=dat&0x01;
		delay_us(60);
		DQ=1;
		dat>>=1;
	}
	delay_us(45);
}

void ReadRomCode(unsigned char *romcode)
{
	unsigned char i;
	DS18B20_Init();	
	DS18B20_WByte(0x33);//��ȡROMָ��
	for(i=0;i<8;i++)
	{	
		romcode[i]=DS18B20_RByte();//��64λrom��
	}
	
}

void Read_Temp(void)			//��ȡ DS18B20оƬ�¶�ֵ
{
	unsigned char lo,hi;
	unsigned int t;
	DS18B20_Init();					//DS18B20оƬ��ʼ��
	DS18B20_WByte(0xCC);		//���������кŵ�����(�����Ͻ���һ��DS18B20оƬ��
	DS18B20_WByte(0x44);		//�����¶�ת��
	DS18B20_Init();
	DS18B20_WByte(0xCC);
	DS18B20_WByte(0xBE);		//���ݴ�������
	lo=DS18B20_RByte();			//���¶�ֵ��λ
	hi=DS18B20_RByte();			//���¶�ֵ��λ
//	printf("lo=%bu\n",lo);
//	printf("hi=%bu\n",hi);
	t=hi;
	t<<= 8;
	t =t|lo;
//	t=((hi*256)+lo)>>4;
//	printf("t=%d\n",t);
	if(t<0)
	{
		zf=1;									//�ø�����־
		t=~t+1;								//ȡ��
	}
	Temp=t;
//	printf("Temp=%d\n",Temp);
}

void Temp_to_Buff()				//����ʵ���¶�ֵ,�����¶�ֵ����ʾ������
{	
	unsigned char shi,ge,xshu;
	float ft;
	unsigned int tt;
	ft=Temp*0.0625;					//����ʵ���¶�ֵ
	tt= ft*10+0.5;					//����С��(����1λС��)���¶�ֵ��չ������
	shi=tt/100;	          	//ʵ���¶�ֵ��ʮλ(��չ��İ�λ)
//	tt =tt% 100;
	ge= (tt%100)/10;							//ʵ���¶�ֵ�ĸ�λ(��չ���ʮλ)
	xshu =(tt%100)%10;						//ʵ���¶�ֵ��С��λ(��չ��ĸ�λ)
//	if(zf==1)
//	DispBuf[3]=0xBF;				//����λ��ʾ���š�_��
//	else
	DispBuf[3]=0xFF;							//����λ����ʾ(����)
	DispBuf[2]=SegCode[shi];			//��ʾʮλ
	DispBuf[1]=SegCode[ge]&0x7f;	//��ʾ��λ,ͬʱ��ʾ dp
	DispBuf[0]=SegCode[xshu];			//��ʾС��λ
}
