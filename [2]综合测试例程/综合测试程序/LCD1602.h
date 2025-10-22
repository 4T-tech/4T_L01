#ifndef _LCD1602_H_
#define _LCD1602_H

#include "main.h"

extern void init_1602(void);
extern void write_command(unsigned char cmd);
extern void write_data(unsigned char dat);
extern void dsp_onechar(unsigned char pos,unsigned char c);
extern void dsp_string(unsigned char line,unsigned char *p,unsigned char length);

#endif 
