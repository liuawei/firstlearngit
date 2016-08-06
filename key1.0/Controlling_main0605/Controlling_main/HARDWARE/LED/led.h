#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//LED驱动代码			 


#define EN_FAULT_DET	1 //使能（1）/禁止（0）故障处理
//LED端口定义
#define RUN_LED 	PCout(1)	//
#define FAULE_LED 	PCout(2)	//
#define POWER_LED	PCout(3)	//
void USART1_send(u8 *buf,u8 len);
void LED_Init(void);//初始化
void LED_USART(u8 res);

#endif

















