#ifndef __BSP_USART2_485_H
#define	__BSP_USART2_485_H
#include "stm32f10x.h"
#include <stdio.h>
#define CLI() __set_PRIMASK(1)		/*关总中断*/  
#define SEI() __set_PRIMASK(0)				/*开总中断*/


void USART2_Config(void);
int  fputc(int ch, FILE *f);
void USART2_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
void Usart2(u8 temp);

void Usart3(u8 temp);
void USART3_Config(void);
void delay(u16 tem);
void Usart1_info(void);
#endif /* __USART2_H */
