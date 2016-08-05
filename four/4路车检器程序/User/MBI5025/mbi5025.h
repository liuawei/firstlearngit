#ifndef __MBI5025_H
#define __MBI5025_H	

#include "stm32f10x.h"

u8 static led[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f,0xff};
void MBI5025_Init(void);//≥ı ºªØ
void MBI5025_Write(u8 data);
void LEDTEST(u8 temp);
void Led_c(void);
void Led_display(u8 temp);
void Close_led(void);
#endif
