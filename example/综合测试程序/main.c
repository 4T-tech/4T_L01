
#include "main.h"

/*数码管显示相关*/
unsigned char SegCode[10]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};//共阴极段码0~9
unsigned char BitCode[4]={0x08,0x04,0x02,0x01};	//位码:P0口“1”有效

/*按键定义与蜂鸣器定义*/
sbit beep = P4^0;
sbit S1 = P3^0;
sbit S2 = P3^1;
sbit S3 = P3^2;
sbit S4 = P3^3;
static unsigned char key_flag = 0; //按键标志

/*ADC相关定义以及AD显示数目缓冲区*/
sbit CS=P4^5;
sbit CLK=P4^1;
sbit DOUT=P4^3;
unsigned char DispBuf_AD[4]={0,0,0,0};								//显示缓冲区  
float Volt;								//电压值
unsigned char digits[4];	//BCD码

/*eeprom*/
unsigned char boot_times;	//商店次数


void led_running();
void seg_display(unsigned char *tdata,unsigned char longth);
void AT_Buff();
void DisplayBootTimes();
void key_check();
void process_key_flags(void);
unsigned char at24c02_init();

void main()
{ 
	unsigned char i,temp;
	unsigned char j=0;
	unsigned char ROM[16]={8,8,8,8,8,8,8,8};
	CS = 1;
	SEG_CHOICE=0;
	LED_CHOICE=1;	
	led_running();
	LED_CHOICE=0;
	P4SW|=0xFF;
	UART_Init();
	CS = 0;
	DQ = 1;

	i = 100;
	while(i--) seg_display(ROM,8);
	
	boot_times = at24c02_init();
	AT_Buff();
	
	while (S1!=0&&S2!=0&&S3!=0&&S4!=0)
	{
    DisplayBootTimes();
	}
	CS = 1;
	DQ = 0;
	
///******1602液晶显示******/
//	init_1602();
//	dsp_string(0,"  Welcome...",12);
//	dsp_string(1," www.gxct.net",13);
///******1602液晶显示******/

	while(1)
	{
		key_check();
		process_key_flags();
	}
}
/********************************************************************/

/******流水灯******/
void led_running()
{
	/*快速过几轮,需要开关在LED上面*/
	unsigned char j=0;
	while(j<3)
	{
		unsigned char aa,i;
		aa=0x01;
		for(i=0;i<8;i++)
		{
			P0=~aa;
			delay(500);
			aa<<=1;
		}		
		j++;
	}
		P0=0x00;
		delay(1000);
		P0=0xff;
		
}

/******数码管显示******/
void seg_display(unsigned char *tdata,unsigned char longth)
{
	unsigned char seg_data[10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};/*字码表*/
	unsigned char i,w;																
	P24 = 1;	
	w=0xFE;							//位选码初值为0xfe
	for(i=0;i<longth;i++)
	{
		P0=0xff;          //关显示，共阳极数码管0xff熄灭
		P24 = 0;
		P0=~w;            //位选码送位选端P0口
		P24 = 1;
		w=_crol_(w,1);		//位选码左移一位，选中下一位LED
		P0=seg_data[ tdata[i] ] ;        //显示字型码并送P0口
		delay(10);		  	//延时
	}
}

/******eeprom******/
void write_AT24C02(unsigned char addr, unsigned char dat)
{
    // 发送起始信号
    I2C_Start();
    
    // 发送设备地址加上写操作位
    I2C_send_byte(AT24C02_ADDR | 0x00); // AT24C02的写地址是基础地址加上0
    if (!Test_ACK())
    {
        // 如果没有收到应答，则退出
        I2C_Stop();
        return;
    }
    
    // 发送内存地址
    I2C_send_byte(addr);
    if (!Test_ACK())
    {
        // 如果没有收到应答，则退出
        I2C_Stop();
        return;
    }
    
    // 发送要写入的数据
    I2C_send_byte(dat);
    if (!Test_ACK())
    {
        // 如果没有收到应答，则退出
        I2C_Stop();
        return;
    }
    
    // 发送停止信号
    I2C_Stop();
    
    // 等待EEPROM完成写入操作
    delay(10); // 根据具体需求调整延迟时间
}
unsigned char read_AT24C02(unsigned char addr)
{
	unsigned char dat;
	I2C_Start();
	I2C_send_byte(AT24C02_ADDR+0);
	if (!Test_ACK())
	{
		return(0);
	}
	I2C_send_byte(addr);
	Master_ACK(0);
	I2C_Start();
	I2C_send_byte(AT24C02_ADDR+1);
	if (!Test_ACK())
	{
		return(0);
	}
	dat = I2C_read_byte();
	Master_ACK(0);
	I2C_Stop();
	return(dat);
}
/*返回开机次数*/
unsigned char at24c02_init()
{
	/*用数码管显示开机次数，模拟i2c另外文件*/
	unsigned char times;
	times = read_AT24C02(254);
	//printf("times=%bu\n",times);
	if(times < 255 )
	{
			times=times+1;
			write_AT24C02(254,times);
			delay(130);
	//	printf("times2=%bu\n",times);
	}
	else
	{
		times = 1;
		write_AT24C02(254,times);
		delay(130);
		
	}
	//printf("times3=%bu\n",times);
	return times;
	
}

void AT_Buff()				//转换值送显示缓冲区(BCD码)
{
	digits[3]= 0;
	digits[2]= boot_times/100;	//百位
	digits[1]= (boot_times/10)%10;	//十位
	digits[0]= boot_times%10;			//个位
}

void DisplayBootTimes()
{
	unsigned char i;
	SEG_CHOICE=1;
	P0=0xff;
	LED_CHOICE=1;
	LED_CHOICE=0;
	for(i=0;i<4;i++)
	{
		SEG_CHOICE=1;
		P0=SegCode[digits[i]];
		SEG_CHOICE=0;
		P0=BitCode[i];
		delay_ADC(10);
	}
	SEG_CHOICE = 1;
	P0 = 0xff;
	SEG_CHOICE = 0;
}


/******模数转换******/
unsigned int TLC549_ADC(void)
{
	unsigned int i,temp=0;
	CS=1;
	CLK =0;
	CS=0;
	delay_us(20);
	for(i=0;i<8;i++)
	{
		temp=temp<<1;
		if (DOUT == 1) temp|=0x01;
		CLK=1;
		delay_us(10);
		CLK =0;
		delay_us(10);
	}
	
	CS =1;
	delay_us(100);
	//printf("%d adc \r\n",temp);
	return (temp);
}
void AD_Buff()				//AD转换值送显示缓冲区(BCD码)
{
	DispBuf_AD[3]= (unsigned int)Volt /1000;		//千位
	DispBuf_AD[2]= ((unsigned int)Volt/100)%10;	//百位
	DispBuf_AD[1]= ((unsigned int)Volt /10)%10;	//十位
	DispBuf_AD[0]= (unsigned int)Volt %10;			//个位
}

void adc_display()
{
	unsigned char i;
	SEG_CHOICE=1;
	P0=0xff;
	LED_CHOICE=1;
	LED_CHOICE=0;
	for(i=0;i<4;i++)
	{
		if(i==3)
		{
			SEG_CHOICE=1;
			P0=SegCode[DispBuf_AD[i]]&0x7f;
			SEG_CHOICE=0;
			P0=BitCode[i];
			delay(10);
		}
		else
		{
			SEG_CHOICE=1;
			P0=SegCode[DispBuf_AD[i]];
			SEG_CHOICE=0;
			P0=BitCode[i];
			delay(10);
		}
	}
	SEG_CHOICE = 1;
	P0 = 0xff;
	SEG_CHOICE = 0;
}



/******蜂鸣器******/
void Delay_B(unsigned int time)
{
    unsigned int i;
    for (i = 0; i < time; i++);
}

void buzz_running()
{
		beep=0;
		Delay_B(30);
		beep=1;
		Delay_B(30);
}

void key_check(void)
{
    // 检查S1按键
    if (S1 == 0) {
        delay(10); // 延时去抖动
        if (S1 == 0 && key_flag != 1) {
            key_flag = 1; // 设置S1标志位
        }
    }

    // 检查S2按键
    if (S2 == 0) {
        delay(10); // 延时去抖动
        if (S2 == 0 && key_flag != 2) {
            key_flag = 2; // 设置S2标志位
        }
    }

    // 检查S3按键
    if (S3 == 0) {
        delay(10); // 延时去抖动
        if (S3 == 0 && key_flag != 3) {
            key_flag = 3; // 设置S3标志位
        }
    }

    // 检查S4按键
    if (S4 == 0) {
        delay(10); // 延时去抖动
        if (S4 == 0 && key_flag != 4) {
            key_flag = 4; // 设置S4标志位
        }
    }

    // 如果按键释放，则清除标志位
    if (S1 && key_flag == 1) key_flag = 0;
    if (S2 && key_flag == 2) key_flag = 0;
    if (S3 && key_flag == 3) key_flag = 0;
    if (S4 && key_flag == 4) key_flag = 0;
}

void process_key_flags(void)
{
    switch(key_flag) {
			case 1: // S1按键按下
			{
					unsigned char i = 0;
					unsigned int ad;
					while((S2&S3&S4) == 1) {
							i++;
							if(i == 20) {
									ad = TLC549_ADC();
									i = 0;
							}
							Volt = 5.0 * ad / 255.0 * 1000.0;
							AD_Buff();
							adc_display();
					}
					break;
			}
			case 2: // S2按键按下
			{
					while((S1&S3&S4)  != 0) {
							Read_Temp();
							Temp_to_Buff();
							Display();
					}
					break;
			}
			case 3: // S3按键按下
			{
					buzz_running();
					break;
			}
			case 4: // S4按键按下
			{
					while((S1&S2&S3)  != 0) {
						DisplayBootTimes();
					}
					break;
			}
			default:
					break;
    }
}

