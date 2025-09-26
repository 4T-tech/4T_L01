//#include "iic.h"

//#define delay_time	5

//static void iic_delay(unsigned char n)
//{
//	do
//	{
//		_nop_();_nop_();_nop_();_nop_();_nop_();
//		_nop_();_nop_();_nop_();_nop_();_nop_();
//		_nop_();_nop_();_nop_();_nop_();_nop_();		
//	}
//	while(n--);      	
//}

////
//void iic_start(void)
//{
//	sda = 1;
//	scl = 1;
//	iic_delay(delay_time);
//	sda = 0;
//	iic_delay(delay_time);
//	scl = 0;    
//}

////
//void iic_stop(void)
//{
//	sda = 0;
//	scl = 1;
//	iic_delay(delay_time);
//	sda = 1;
//	iic_delay(delay_time);
//}

////
//void iic_send_byte(unsigned char byt)
//{
//	unsigned char i;
//	
//	for(i=0; i<8; i++)
//	{
//		scl = 0;
//		iic_delay(delay_time);
//		if(byt & 0x80)
//		{
//			sda = 1;
//		}
//		else{
//			sda = 0;
//		}
//		iic_delay(delay_time);
//		scl = 1;
//		byt <<= 1;
//		iic_delay(delay_time);
//	}
//	scl = 0;  
//}

////
//unsigned char iic_wait_ack(void)
//{
//	unsigned char ack_bit;
//	
//	scl = 1;
//	iic_delay(delay_time);
//	ack_bit = sda; 
//	scl = 0;
//	iic_delay(delay_time);
//	
//	return ack_bit;
//}

////
//unsigned char iic_receive_byte(void)
//{
//	unsigned char da;
//	unsigned char i;
//	for(i=0;i<8;i++){   
//		scl = 1;
//		iic_delay(delay_time);
//		da <<= 1;
//		if(sda) 
//			da |= 0x01;
//		scl = 0;
//		iic_delay(delay_time);
//	}
//	return da;    
//}

////
//void iic_send_ack(unsigned char ack_bit)
//{
//	scl = 0;
//	sda = ack_bit; 
//	iic_delay(delay_time);
//	scl = 1;
//	iic_delay(delay_time);
//	scl = 0; 
//	sda = 1;
//	iic_delay(delay_time);
//}




#include "iic.h"


#define uchar unsigned char
#define uint unsigned  int
#define AT24C02_ADDR  0xa0	//AT24C02地址


////基于STC12单片机1us延时函数
////函数说明：内部调用
//static void Delaym1us()
//{
//	_nop_();
//}

////基于STC12单片机us延时函数
////函数说明：外部调用
//void Delay_us(uint time)
//{
//	int i;
//	for(i=0; i<time; i++)
//	{
//		Delaym1us();
//	}
//}

///*5us延时*/
//void delay_5us()  
//{
//	Delay_us(5);
//}

///*1Ms延时*/
//void delay(uint z)
//{
//	uint x,y;
//	for(x = z; x > 0; x--)
//		for(y = 114; y > 0 ; y--);
//}

/*I2C初始化*/
void I2C_init()	
{
	sda = 1;
	_nop_();
	scl = 1;
	_nop_();
}

/*I2C起始信号*/
void I2C_Start()  
{
	scl = 1;
	_nop_();
	sda = 1;
	delay_5us();
	sda = 0;
	delay_5us();
}

/*I2C终止信号*/
void I2C_Stop()
{
	sda = 0;
	_nop_();
	scl = 1;
	delay_5us();
	sda = 1;
	delay_5us();
}

/*主机发送应答*/
void Master_ACK(bit i)		
{
	scl = 0; // 拉低时钟总线允许sda数据总线上的数据变化
	_nop_(); // 让总线稳定
	if (i)	 //如果i = 1 那么拉低数据总线 表示主机应答
	{
		sda = 0;
	}
	else	 
	{
		sda = 1;	 //发送非应答
	}
	_nop_();//让总线稳定
	scl = 1;//拉高时钟总线 让从机从sda线上读走 主机的应答信号
	delay_5us();
	scl = 0;//拉低时钟总线， 占用总线继续通信
	_nop_();
	sda = 1;//释放sda数据总线。
	_nop_();
}

/*检测从机应答*/
bit Test_ACK()
{
	scl = 1;
	delay_5us();
	if (sda)
	{
		scl = 0;
		_nop_();
		I2C_Stop();
		return(0);
	}
	else
	{
		scl = 0;
		_nop_();
		return(1);
	}
}

/*发送一个字节*/
void I2C_send_byte(uchar byte)
{
	uchar i;
	for(i = 0 ; i < 8 ; i++)
	{
		scl = 0;
		_nop_();
		if (byte & 0x80)
		{				
			sda = 1;	
			_nop_();				   
		}				
		else
		{
			sda = 0;
			_nop_();
		}
		scl = 1;
		_nop_();
		byte <<= 1;	// 0101 0100B 
	}
	scl = 0;
	_nop_();
	sda = 1;
	_nop_();
}


/*I2C 读一字节*/
uchar I2C_read_byte()
{
	uchar dat,i;
	scl = 0;
	_nop_();
	sda = 1;
	_nop_();
	for(i = 0 ; i < 8 ; i++)
	{
		scl = 1;
		_nop_();
		if (sda)			    
		{
			 dat |= 0x01; //
		}
		else
		{
			dat &=  0xfe;	//1111 1110
		}
		_nop_();
		scl = 0 ;
		_nop_();
		if(i < 7)
		{
			dat = dat << 1;	
		}
	}
	return(dat);
}

/*I2C发送数据*/
bit I2C_TransmitData(uchar ADDR, DAT)
{
	I2C_Start();
	I2C_send_byte(AT24C02_ADDR+0);
	if (!Test_ACK())
	{
		return(0);
	}
	I2C_send_byte(ADDR); 
	if (!Test_ACK())
	{
		return(0);
	}
	I2C_send_byte(DAT);
	if (!Test_ACK())
	{
		return(0);
	}
	I2C_Stop();
	return(1);	
}

/*I2C接收数据*/
uchar I2C_ReceiveData(uchar ADDR)
{
	uchar DAT;
	I2C_Start();
	I2C_send_byte(AT24C02_ADDR+0);
	if (!Test_ACK())
	{
		return(0);
	}
	I2C_send_byte(ADDR);
	Master_ACK(0);
	I2C_Start();
	I2C_send_byte(AT24C02_ADDR+1);
	if (!Test_ACK())
	{
		return(0);
	}
	DAT = I2C_read_byte();
	Master_ACK(0);
	I2C_Stop();
	return(DAT);	
}

