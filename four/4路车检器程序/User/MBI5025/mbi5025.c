#include "mbi5025.h"
#include "bsp_usart2_485.h"
#include "bsp_led.h"  
/*
  SDI   pb8
  CLK   pc13
  LE     pb9
*/

void MBI5025_Init(void)//初始化
{	
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 													   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9| GPIO_Pin_8;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);		  
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 													   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);		
	  GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	  GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	  GPIO_ResetBits(GPIOC,GPIO_Pin_13); 
	  Close_led();	
}


void MBI5025_Write(u8 data)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		if(data&0x80)//串行数据输入1
		GPIO_SetBits(GPIOB,GPIO_Pin_8);			
		else
		GPIO_ResetBits(GPIOB,GPIO_Pin_8);
		delay(100);
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		data<<=1;	
		delay(100);
	}
}

void Close_led()
{
GPIO_ResetBits(GPIOB,GPIO_Pin_9);
MBI5025_Write(0xff);
GPIO_SetBits(GPIOB,GPIO_Pin_9);
}






