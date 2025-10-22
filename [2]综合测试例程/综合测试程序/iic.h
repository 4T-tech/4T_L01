#ifndef _IIC_H_
#define _IIC_H_

#include "main.h"

#define AT24C02_ADDR  0xa0	//AT24C02µÿ÷∑

sbit scl = P2^6;
sbit sda = P2^5;




void I2C_init();
void I2C_Start();
void I2C_Stop();
void Master_ACK(bit i);
void I2C_send_byte(unsigned char byte);
unsigned char I2C_read_byte();
bit Test_ACK();
bit I2C_TransmitData(unsigned char ADDR, DAT);
unsigned char I2C_ReceiveData(unsigned char ADDR);



#endif 