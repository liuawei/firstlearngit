#ifndef _PCF8563_H_
#define _PCF8563_H_
#include "iic.h"
#define ReadCode 0xa3
#define WriteCode 0xa2
void PCF8563_Init(void);
u8 PCF8563_ReaDAdress(u8 Adress);
void PCF8563_WriteAdress(u8 Adress, u8 DataTX);
extern u8 PCF8563_Time[7];
void PCF8563_ReadTimes(void);
void PCF8563_CLKOUT_1s(void);
void Wait_time(void);
void Time_J(void);
void WatchDog_Init(u8 prer, u16 reld);
void WatchDog_Feed(void);
void EXTI_Configuration(void);
void GPIO_Configuration(void);
void EXIT_NVIC_Configuration(void);
void Gps_W_Pcf8563(void);
#endif
