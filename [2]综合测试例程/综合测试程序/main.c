
#include "main.h"

/*�������ʾ���*/
unsigned char SegCode[10]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};//����������0~9
unsigned char BitCode[4]={0x08,0x04,0x02,0x01};	//λ��:P0�ڡ�1����Ч

/*�������������������*/
sbit beep = P4^0;
sbit S1 = P3^0;
sbit S2 = P3^1;
sbit S3 = P3^2;
sbit S4 = P3^3;
static unsigned char key_flag = 0; //������־

/*ADC��ض����Լ�AD��ʾ��Ŀ������*/
sbit CS=P4^5;
sbit CLK=P4^1;
sbit DOUT=P4^3;
unsigned char DispBuf_AD[4]={0,0,0,0};								//��ʾ������  
float Volt;								//��ѹֵ
unsigned char digits[4];	//BCD��

/*eeprom*/
unsigned char boot_times;	//�̵����


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
	
///******1602Һ����ʾ******/
//	init_1602();
//	dsp_string(0,"  Welcome...",12);
//	dsp_string(1," www.gxct.net",13);
///******1602Һ����ʾ******/

	while(1)
	{
		key_check();
		process_key_flags();
	}
}
/********************************************************************/

/******��ˮ��******/
void led_running()
{
	/*���ٹ�����,��Ҫ������LED����*/
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

/******�������ʾ******/
void seg_display(unsigned char *tdata,unsigned char longth)
{
	unsigned char seg_data[10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};/*�����*/
	unsigned char i,w;																
	P24 = 1;	
	w=0xFE;							//λѡ���ֵΪ0xfe
	for(i=0;i<longth;i++)
	{
		P0=0xff;          //����ʾ�������������0xffϨ��
		P24 = 0;
		P0=~w;            //λѡ����λѡ��P0��
		P24 = 1;
		w=_crol_(w,1);		//λѡ������һλ��ѡ����һλLED
		P0=seg_data[ tdata[i] ] ;        //��ʾ�����벢��P0��
		delay(10);		  	//��ʱ
	}
}

/******eeprom******/
void write_AT24C02(unsigned char addr, unsigned char dat)
{
    // ������ʼ�ź�
    I2C_Start();
    
    // �����豸��ַ����д����λ
    I2C_send_byte(AT24C02_ADDR | 0x00); // AT24C02��д��ַ�ǻ�����ַ����0
    if (!Test_ACK())
    {
        // ���û���յ�Ӧ�����˳�
        I2C_Stop();
        return;
    }
    
    // �����ڴ��ַ
    I2C_send_byte(addr);
    if (!Test_ACK())
    {
        // ���û���յ�Ӧ�����˳�
        I2C_Stop();
        return;
    }
    
    // ����Ҫд�������
    I2C_send_byte(dat);
    if (!Test_ACK())
    {
        // ���û���յ�Ӧ�����˳�
        I2C_Stop();
        return;
    }
    
    // ����ֹͣ�ź�
    I2C_Stop();
    
    // �ȴ�EEPROM���д�����
    delay(10); // ���ݾ�����������ӳ�ʱ��
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
/*���ؿ�������*/
unsigned char at24c02_init()
{
	/*���������ʾ����������ģ��i2c�����ļ�*/
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

void AT_Buff()				//ת��ֵ����ʾ������(BCD��)
{
	digits[3]= 0;
	digits[2]= boot_times/100;	//��λ
	digits[1]= (boot_times/10)%10;	//ʮλ
	digits[0]= boot_times%10;			//��λ
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


/******ģ��ת��******/
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
void AD_Buff()				//ADת��ֵ����ʾ������(BCD��)
{
	DispBuf_AD[3]= (unsigned int)Volt /1000;		//ǧλ
	DispBuf_AD[2]= ((unsigned int)Volt/100)%10;	//��λ
	DispBuf_AD[1]= ((unsigned int)Volt /10)%10;	//ʮλ
	DispBuf_AD[0]= (unsigned int)Volt %10;			//��λ
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



/******������******/
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
    // ���S1����
    if (S1 == 0) {
        delay(10); // ��ʱȥ����
        if (S1 == 0 && key_flag != 1) {
            key_flag = 1; // ����S1��־λ
        }
    }

    // ���S2����
    if (S2 == 0) {
        delay(10); // ��ʱȥ����
        if (S2 == 0 && key_flag != 2) {
            key_flag = 2; // ����S2��־λ
        }
    }

    // ���S3����
    if (S3 == 0) {
        delay(10); // ��ʱȥ����
        if (S3 == 0 && key_flag != 3) {
            key_flag = 3; // ����S3��־λ
        }
    }

    // ���S4����
    if (S4 == 0) {
        delay(10); // ��ʱȥ����
        if (S4 == 0 && key_flag != 4) {
            key_flag = 4; // ����S4��־λ
        }
    }

    // ��������ͷţ��������־λ
    if (S1 && key_flag == 1) key_flag = 0;
    if (S2 && key_flag == 2) key_flag = 0;
    if (S3 && key_flag == 3) key_flag = 0;
    if (S4 && key_flag == 4) key_flag = 0;
}

void process_key_flags(void)
{
    switch(key_flag) {
			case 1: // S1��������
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
			case 2: // S2��������
			{
					while((S1&S3&S4)  != 0) {
							Read_Temp();
							Temp_to_Buff();
							Display();
					}
					break;
			}
			case 3: // S3��������
			{
					buzz_running();
					break;
			}
			case 4: // S4��������
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

