#include <stc12.h>
#include "DS1302.h"
#include <INTRINS.H>
//引脚定义；
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
 
//定义数组，用于存放设定的时间，年月日时分秒星期
unsigned char DS1302_Time[7];//顺序：年月日时分秒星期

// DS1302初始化
void  DS1302_Init()
{
	DS1302_CE=0;//将使能位置0，低电平；
	DS1302_SCLK=0;//将时钟位置0，低电平；
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

//DS1302单字节写入函数
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
 
//DS1302单字节读出函数
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
 
 
//向DS1302内设定时间
void DS1302_SetTime(unsigned char Date[])
{
    DS1302_WriteBety(DS1302_WP, 0x00); // 关闭写保护

    DS1302_WriteBety(DS1302_YEAR, Date[0] / 10 * 16 + Date[0] % 10); // 写入年
		DS1302_WriteBety(DS1302_MONTH, Date[1] / 10 * 16 + Date[1] % 10); // 写入月
    DS1302_WriteBety(DS1302_DATE, Date[2] / 10 * 16 + Date[2] % 10); // 写入日
    DS1302_WriteBety(DS1302_HOUR, Date[3] / 10 * 16 + Date[3] % 10); // 写入时
    DS1302_WriteBety(DS1302_MINUTE, Date[4] / 10 * 16 + Date[4] % 10); // 写入分
    DS1302_WriteBety(DS1302_SECOND, Date[5] / 10 * 16 + Date[5] % 10); // 写入秒
    DS1302_WriteBety(DS1302_DAY, Date[6] / 10 * 16 + Date[6] % 10); // 写入星期
    DS1302_WriteBety(DS1302_WP, 0x80); // 开启写保护
}
 
//读取DS1302内时间
void DS1302_ReadTime(unsigned char Date[])	
{
	unsigned char Temp;//定义变量，用于暂时存储BCD码
    Temp=DS1302_ReadBety(0x8d);//读取年BCD码；
		Date[0]=Temp/16*10+Temp%16;//BCD码转十进制；
	 Temp=DS1302_ReadBety(0x89);//读取月BCD码；
		Date[1]=Temp/16*10+Temp%16;//BCD码转十进制；
	 Temp=DS1302_ReadBety(0x87);//读取日BCD码；
		Date[2]=Temp/16*10+Temp%16;//BCD码转十进制；
	 Temp=DS1302_ReadBety(0x85);//读取小时BCD码；
		Date[3]=Temp/16*10+Temp%16;//BCD码转十进制；
	 Temp=DS1302_ReadBety(0x83);//读取分钟BCD码；
		Date[4]=Temp/16*10+Temp%16;//BCD码转十进制；
	 Temp=DS1302_ReadBety(0x81);//读取秒BCD码；
		Date[5]=Temp/16*10+Temp%16;//BCD码转十进制；
	 Temp=DS1302_ReadBety(0x8b);//读取星期BCD码；
		Date[6]=Temp/16*10+Temp%16;//BCD码转十进制；
}