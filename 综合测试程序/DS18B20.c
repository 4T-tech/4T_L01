//#include "stc12.h"
//#include "delay.h"

//#include <intrins.h>
//#define uchar unsigned char
//#define uint unsigned int
//	
//sbit DQ=P2^7;

//uchar code SegCode[]=
//{0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};//共阳极段码 0~9
//uchar code BitCode[]={0x08,0x04,0x02,0x01};						//位码:P0 口“1”有效
//uchar DispBuf[4]={0,0,0,0};						//显示缓冲区
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
//	Delay_us(80);				//在数据线上产生约800μs的低电平
//	DQ =1;
//	Delay_us(6);					//数据线拉高,延时60μs
//	for(;;)
//	{
//		if(!DQ)break;			//读取总线电平,若DQ=0复位成功,DO=1复位失败
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
//		Delay_us(10);				//总线拉高,延时 10μs
//		if(DQ)dat|=0x80;
//		Delay_us(50);				//延时 50 us
//	}
//	return(dat);
//}

//void DS18B20_WByte(uchar dat)
//{
//	uchar i=0;
//	for(i=8;i>0;i--)
//	{
//		DQ=0;
//		DQ=dat&0x01;					//将当前数据放到总线上,低位在先,高位在后
//		Delay_us(50);						//延时 50 ws
//		DQ =1;								//总线拉高,为写入下一位做准备
//		dat>>=1;
//	}
//	
//}

//void ReadRomCode(unsigned char *romcode)
//{
//	unsigned char i;
//	init;//复位
//	DS18B20_WByte(0x33);//读取ROM指令
//	for(i=0;i<8;i++)
//	{
//		romcode[i]=DS18B20_RByte();//读64位rom码
//	}
//	
//}
//void Read_Temp(void)			//读取 DS18B20芯片温度值
//{
//	uchar lo,hi;
//	int t;
//	DS18B20_Init();					//DS18B20芯片初始化
//	DS18B20_WByte(0xCC);		//跳过读序列号的命令(总线上仅有一个DS18B20芯片）
//	DS18B20_WByte(0x44);		//启动温度转换
//	DS18B20_Init();
//	DS18B20_WByte(0xCC);
//	DS18B20_WByte(0xBE);		//读暂存器命令
//	lo=DS18B20_RByte();			//读温度值低位
//	hi=DS18B20_RByte();			//读温度值高位
//	t=hi;
//	t<<= 8;
//	t =t|lo;
//	if(t<0)
//	{
//		zf=1;									//置负数标志
//		t=~t+1;								//取补
//	}
//	Temp=t;
//}

//void Temp_to_Buff()				//计算实际温度值,并将温度值送显示缓冲区
//{	
//	uchar shi,ge,xshu;
//	float ft;
//	uint tt;
//	ft=Temp*0.0625;					//计算实际温度值
//	tt= ft*10+0.5;					//将带小数(保留1位小数)的温度值扩展成整数
//	shi=tt/100;							//实际温度值的十位(扩展后的百位)
//	tt =tt% 100;
//	ge= tt/10;							//实际温度值的个位(扩展后的十位)
//	xshu =tt%10;						//实际温度值的小数位(扩展后的个位)
//	if(zf==1)
//	DispBuf[3]=0x40;				//符号位显示负号“_”
//	else
//	DispBuf[3]=0x00;							//符号位不显示(正数)
//	DispBuf[2]=SegCode[shi];			//显示十位
//	DispBuf[1]=SegCode[ge]&0x80;	//显示个位,同时显示 dp
//	DispBuf[0]=SegCode[xshu];			//显示小数位
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