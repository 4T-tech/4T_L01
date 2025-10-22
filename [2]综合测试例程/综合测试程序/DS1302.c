#include <stc12.h>
#include "DS1302.h"
#include <INTRINS.H>
//���Ŷ��壻
sbit  DS1302_SCLK=P2^5;
sbit  DS1302_IO=P2^4;
sbit  DS1302_CE=P2^6;
 
#define  DS1302_SECOND		0X80
#define  DS1302_MINUTE  	0X82
#define  DS1302_HOUR			0X84
#define  DS1302_DATE      0X86
#define  DS1302_MONTH			0X88
#define  DS1302_DAY       0X8A
#define  DS1302_YEAR      0X8C
#define  DS1302_WP        0X8E 
 
//�������飬���ڴ���趨��ʱ�䣬������ʱ��������
unsigned char DS1302_Time[7];//˳��������ʱ��������

// DS1302��ʼ��
void  DS1302_Init()
{
	DS1302_CE=0;//��ʹ��λ��0���͵�ƽ��
	DS1302_SCLK=0;//��ʱ��λ��0���͵�ƽ��
}
 
void DS1302_Write(unsigned char temp)
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		DS1302_SCLK=0;
		DS1302_IO=temp&0x01;
		temp>>=1; 
		DS1302_SCLK=1;
	}
		
}

//DS1302���ֽ�д�뺯��
void  DS1302_WriteBety(unsigned char address,Data)
{
 	DS1302_CE=0;	
	_nop_();

 	DS1302_SCLK=0;	
	_nop_();

 	DS1302_CE=1; 	
	_nop_();
	
 	DS1302_Write(address);	
 	DS1302_Write(Data);
 	DS1302_CE=0; 
}
 
//DS1302���ֽڶ�������
unsigned char   DS1302_ReadBety(unsigned char address)
{
unsigned char i,temp=0x00;
 	DS1302_CE=0;	
	_nop_();
 	DS1302_SCLK=0;	
	_nop_();
 	DS1302_CE=1;	
	_nop_();
 	DS1302_Write(address);
 	for (i=0;i<8;i++) 	
 	{	_nop_();	
		DS1302_SCLK=0;
		temp>>=1;	
 		if(DS1302_IO)
 		temp|=0x80;	
 		DS1302_SCLK=1;
	} 
 	DS1302_CE=0;	_nop_();
 	DS1302_SCLK=0;	_nop_();
	DS1302_SCLK=1;	_nop_();
	DS1302_IO=0;	_nop_();
	DS1302_IO=1;	_nop_();
	return (temp);
}
 
 
//��DS1302���趨ʱ��
void DS1302_SetTime(unsigned char Date[])
{
    DS1302_WriteBety(DS1302_WP, 0x00); // �ر�д����

    DS1302_WriteBety(DS1302_YEAR, Date[0] / 10 * 16 + Date[0] % 10); // д����
		DS1302_WriteBety(DS1302_MONTH, Date[1] / 10 * 16 + Date[1] % 10); // д����
    DS1302_WriteBety(DS1302_DATE, Date[2] / 10 * 16 + Date[2] % 10); // д����
    DS1302_WriteBety(DS1302_HOUR, Date[3] / 10 * 16 + Date[3] % 10); // д��ʱ
    DS1302_WriteBety(DS1302_MINUTE, Date[4] / 10 * 16 + Date[4] % 10); // д���
    DS1302_WriteBety(DS1302_SECOND, Date[5] / 10 * 16 + Date[5] % 10); // д����
    DS1302_WriteBety(DS1302_DAY, Date[6] / 10 * 16 + Date[6] % 10); // д������
    DS1302_WriteBety(DS1302_WP, 0x80); // ����д����
}
 
//��ȡDS1302��ʱ��
void DS1302_ReadTime(unsigned char Date[])	
{
	unsigned char Temp;//���������������ʱ�洢BCD��
    Temp=DS1302_ReadBety(0x8d);//��ȡ��BCD�룻
		Date[0]=Temp/16*10+Temp%16;//BCD��תʮ���ƣ�
	 Temp=DS1302_ReadBety(0x89);//��ȡ��BCD�룻
		Date[1]=Temp/16*10+Temp%16;//BCD��תʮ���ƣ�
	 Temp=DS1302_ReadBety(0x87);//��ȡ��BCD�룻
		Date[2]=Temp/16*10+Temp%16;//BCD��תʮ���ƣ�
	 Temp=DS1302_ReadBety(0x85);//��ȡСʱBCD�룻
		Date[3]=Temp/16*10+Temp%16;//BCD��תʮ���ƣ�
	 Temp=DS1302_ReadBety(0x83);//��ȡ����BCD�룻
		Date[4]=Temp/16*10+Temp%16;//BCD��תʮ���ƣ�
	 Temp=DS1302_ReadBety(0x81);//��ȡ��BCD�룻
		Date[5]=Temp/16*10+Temp%16;//BCD��תʮ���ƣ�
	 Temp=DS1302_ReadBety(0x8b);//��ȡ����BCD�룻
		Date[6]=Temp/16*10+Temp%16;//BCD��תʮ���ƣ�
}