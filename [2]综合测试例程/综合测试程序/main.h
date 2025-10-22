#ifndef _MAIN_H_
#define _MAIN_H_


/*MCU基础头文件*/
#include "stc12.h"
/*标准库头文件*/
#include "intrins.h"
#include "stdio.h"
#include "string.h"
/*应用.h头文件*/
#include "iic.h"
#include "LCD1602.h"
#include "Uart.h"
#include "delay.h"
#include "DS18B20.h"

/*数码管显示相关*/
extern unsigned char SegCode[10];
extern unsigned char BitCode[4];


#endif
