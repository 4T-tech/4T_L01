
extern void DS1302_Init();//声明初始化函数
extern void DS1302_WriteByte(unsigned char command,Data);//声明单字节写入函数
extern void DS1302_ReadByte(unsigned char command);//声明单字节读出函数
extern void DS1302_SetTime(unsigned char Date[]);//声明设置内部时间函数
extern void DS1302_ReadTime(unsigned char Date[]);//声明读取内部时间函数