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
#define   Sens1  (~GPIO_ReadInputData(GPIOB))&0x000b       //��ȡPB0--3λ��ֵ
#define   Sens2 (~GPIO_ReadInputData(GPIOB)&0x0070)>>4   //��ȡPB4 5 6λ��ֵ
#define   Sens3 (~GPIO_ReadInputData(GPIOB)&0x0380)>>7   //��ȡPB7 8 9λ��ֵ
#define   Sens4 (~GPIO_ReadInputData(GPIOB)&0x1c00)>>10  //��ȡPB10 11 12λ��ֵ
#define   Sens5 (~GPIO_ReadInputData(GPIOB)&0x1FFF)      //��ȡPB0--12λ��ֵ
#define   address (~GPIO_ReadInputData(GPIOC)&0xC000)>>14//��ȡPC14 15λ��ֵ
#define   tim_temp (~GPIO_ReadInputData(GPIOA)&0x000E)>>1//��ȡPA��1 2 3λ��ֵ			
void nms(u16 i);
void GPIO_Config(void);
void Led_Start(void);
void Switch_Set(void);
#endif /* __LED_H */
