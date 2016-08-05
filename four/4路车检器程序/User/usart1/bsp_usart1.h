#ifndef __BSP_USART1_H
#define	__BSP_USART1_H

#include "stm32f10x.h"
#include <stdio.h>

void USART1_Config(void);
int  fputc(int ch, FILE *f);
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
void Usart1(u8 temp);
void Usartt1(u8 temp);
void RS485_Write_Data(u8 *pBuffer,u8 Len);
void Usart1_address(u8 temp);
void real_time(void);
#endif /* __USART1_H */
