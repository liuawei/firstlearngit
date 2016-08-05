#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"

/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define   ON  0
#define   OFF 1
#define   LED(a)	if (a)	\
          {GPIO_SetBits(GPIOA,GPIO_Pin_7);GPIO_SetBits(GPIOA,GPIO_Pin_4);GPIO_SetBits(GPIOA,GPIO_Pin_6);GPIO_SetBits(GPIOA,GPIO_Pin_5);}\
					else		\
					{GPIO_ResetBits(GPIOA,GPIO_Pin_7);GPIO_ResetBits(GPIOA,GPIO_Pin_4);GPIO_ResetBits(GPIOA,GPIO_Pin_6);GPIO_ResetBits(GPIOA,GPIO_Pin_5);}
#define   LED1(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define   LED2(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_5);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define   LED3(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_6);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_6)
#define   LED4(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_7);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define   Sens1  (~GPIO_ReadInputData(GPIOB))&0x000b       //读取PB0--3位的值
#define   Sens2 (~GPIO_ReadInputData(GPIOB)&0x0070)>>4   //读取PB4 5 6位的值
#define   Sens3 (~GPIO_ReadInputData(GPIOB)&0x0380)>>7   //读取PB7 8 9位的值
#define   Sens4 (~GPIO_ReadInputData(GPIOB)&0x1c00)>>10  //读取PB10 11 12位的值
#define   Sens5 (~GPIO_ReadInputData(GPIOB)&0x1FFF)      //读取PB0--12位的值
#define   address (~GPIO_ReadInputData(GPIOC)&0xC000)>>14//读取PC14 15位的值
#define   tim_temp (~GPIO_ReadInputData(GPIOA)&0x000E)>>1//读取PA第1 2 3位的值			
void nms(u16 i);
void GPIO_Config(void);
void Led_Start(void);
void Switch_Set(void);
#endif /* __LED_H */
