#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//LED��������			 


#define EN_FAULT_DET	1 //ʹ�ܣ�1��/��ֹ��0�����ϴ���
//LED�˿ڶ���
#define RUN_LED 	PCout(1)	//
#define FAULE_LED 	PCout(2)	//
#define POWER_LED	PCout(3)	//
void USART1_send(u8 *buf,u8 len);
void LED_Init(void);//��ʼ��
void LED_USART(u8 res);

#endif

















