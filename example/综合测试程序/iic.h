#ifndef __IIC_H
#define __IIC_H

#include "stc12.h"
#include "intrins.h"
#include "delay.h"

#define uchar unsigned char
#define uint unsigned  int
#define AT24C02_ADDR  0xa0	//AT24C02µÿ÷∑

sbit scl = P2^6;
sbit sda = P2^5;




void I2C_init();
void I2C_Start();
void I2C_Stop();
void Master_ACK(bit i);
void I2C_send_byte(uchar byte);
uchar I2C_read_byte();
bit Test_ACK();
bit I2C_TransmitData(uchar ADDR, DAT);
uchar I2C_ReceiveData(uchar ADDR);





//typedef enum{
//    ack = 0,
//    noack
//}ack_signal;
// 
//void iic_stop();
//void iic_start();
//void iic_send_byte(unsigned char byt);
//void iic_send_ack(unsigned char ack_bit);
//unsigned char iic_wait_ack(void);
//unsigned char iic_receive_byte(void);

#endif 