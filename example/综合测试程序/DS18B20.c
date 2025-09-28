#include "DS18B20.h"


unsigned char romcode[8];
int Temp;

unsigned char DispBuf[4]={1,1,1,1};						//显示缓冲区

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
	DS18B20_WByte(0x33);//读取ROM指令
	for(i=0;i<8;i++)
	{	
		romcode[i]=DS18B20_RByte();//读64位rom码
	}
	
}

void Read_Temp(void)			//读取 DS18B20芯片温度值
{
	unsigned char lo,hi;
	unsigned int t;
	DS18B20_Init();					//DS18B20芯片初始化
	DS18B20_WByte(0xCC);		//跳过读序列号的命令(总线上仅有一个DS18B20芯片）
	DS18B20_WByte(0x44);		//启动温度转换
	DS18B20_Init();
	DS18B20_WByte(0xCC);
	DS18B20_WByte(0xBE);		//读暂存器命令
	lo=DS18B20_RByte();			//读温度值低位
	hi=DS18B20_RByte();			//读温度值高位
//	printf("lo=%bu\n",lo);
//	printf("hi=%bu\n",hi);
	t=hi;
	t<<= 8;
	t =t|lo;
//	t=((hi*256)+lo)>>4;
//	printf("t=%d\n",t);
	if(t<0)
	{
		zf=1;									//置负数标志
		t=~t+1;								//取补
	}
	Temp=t;
//	printf("Temp=%d\n",Temp);
}

void Temp_to_Buff()				//计算实际温度值,并将温度值送显示缓冲区
{	
	unsigned char shi,ge,xshu;
	float ft;
	unsigned int tt;
	ft=Temp*0.0625;					//计算实际温度值
	tt= ft*10+0.5;					//将带小数(保留1位小数)的温度值扩展成整数
	shi=tt/100;	          	//实际温度值的十位(扩展后的百位)
//	tt =tt% 100;
	ge= (tt%100)/10;							//实际温度值的个位(扩展后的十位)
	xshu =(tt%100)%10;						//实际温度值的小数位(扩展后的个位)
//	if(zf==1)
//	DispBuf[3]=0xBF;				//符号位显示负号“_”
//	else
	DispBuf[3]=0xFF;							//符号位不显示(正数)
	DispBuf[2]=SegCode[shi];			//显示十位
	DispBuf[1]=SegCode[ge]&0x7f;	//显示个位,同时显示 dp
	DispBuf[0]=SegCode[xshu];			//显示小数位
}
