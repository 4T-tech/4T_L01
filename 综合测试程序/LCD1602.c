#include <stc12.h>
#include "intrins.h"


//控制线定义
sbit RS = P2^0;
sbit RW = P2^1; 
sbit EN = P2^2;
sbit D0 = P1^0;
sbit D1 = P1^1;
sbit D2 = P1^2;
sbit D3 = P1^3;
sbit D4 = P1^4;
sbit D5 = P1^5;
sbit D6 = P1^6;
sbit D7 = P1^7;

//必要的延时
void delay_ms(unsigned char t)
{
	unsigned char i;
	while(t--)
	{
		for(i=0;i<112;i++);	
	}
}

//写命令函数
void write_command(unsigned char cmd)
{
    delay_ms(5); // check busy flag
    EN = 0;
    RS = 0;
    RW = 0;
    _nop_();
    EN = 1;
    
    
    D7 = (cmd>>7)&0x01;
    D6 = (cmd>>6)&0x01;
    D5 = (cmd>>5)&0x01;
    D4 = (cmd>>4)&0x01;
    D3 = (cmd>>3)&0x01;
    D2 = (cmd>>2)&0x01;
    D1 = (cmd>>1)&0x01;
    D0 = cmd&0x01;

    EN = 0;
}

void write_data(unsigned char dat)
{
    delay_ms(5); //busy flag check 
    EN = 0;
    RS = 1;
    RW = 0; 
    _nop_();
    EN = 1;

    
    D7 = (dat>>7)&0x01;
    D6 = (dat>>6)&0x01;
    D5 = (dat>>5)&0x01;
    D4 = (dat>>4)&0x01;
    D3 = (dat>>3)&0x01;
    D2 = (dat>>2)&0x01;
    D1 = (dat>>1)&0x01;
    D0 = dat&0x01;

    EN = 0;
    RS = 0;
}

//显示一个字符
void dsp_onechar(unsigned char pos,unsigned char c)
{
 	unsigned char p;
 	if (pos>=0x10)
    p=pos+0xb0; 
 	else 
    p=pos+0x80; 
 	write_command(p);
 	write_data(c); 
}

//显示字符串
void dsp_string(unsigned char line,unsigned char *p,unsigned char length)
{
 	unsigned char l,i;
 	l=line<<4;
 	for (i=0;i<length;i++)
 	dsp_onechar(l++,*(p+i));
}

//LCD1602初始化操作
void init_1602(void)
{
 	delay_ms(15);
 	write_command(0x38); 
 	write_command(0x38); 
 	write_command(0x38); 
 	write_command(0x06); 
 	write_command(0x0c); 
 	write_command(0x01); 
 	delay_ms(15);
 	delay_ms(15);
}
