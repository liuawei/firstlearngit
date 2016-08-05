#include "bsp_usart2_485.h"
#include "bsp_usart1.h"
#include "stdlib.h"


void delay(u16 tem)
{u16 i;
  for(i=0;i<tem;i++)
    {;}
}


void USART2_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)    
	{	
		res=USART2->DR;	


	}
		USART_ClearFlag(USART2,USART_FLAG_RXNE);  
    USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
}
	
