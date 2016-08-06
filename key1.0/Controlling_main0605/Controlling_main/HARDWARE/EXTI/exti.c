#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "si4432.h"
//////////////////////////////////////////////////////////////////////////////////	 
//外部中断 驱动代码			   	  
////////////////////////////////////////////////////////////////////////////////// 	  
 

//外部中断15~10服务程序
void EXTI15_10_IRQHandler(void)
{			
	if((nIRQ==0)&&Tx_Rx)
	SI4432_RX();		/**SI4432接收函数**/
	EXTI->PR=1<<13;     //清除LINE13上的中断标志位  
}
//外部中断初始化程序
//初始化PA0,PA13,PA15为中断输入.
void EXTIX_Init(void)
{
	RCC->APB2ENR|=1<<4;     //使能PORTC时钟
	JTAG_Set(JTAG_SWD_DISABLE);//关闭JTAG和SWD   
  
	GPIOC->CRH&=0XFF0FFFFF;//PC13设置成输入	  
	GPIOC->CRH|=0X00800000; 				   
	GPIOC->ODR|=1<<13;	   //PC13上拉

//	Ex_NVIC_Config(GPIO_A,0,RTIR); //上升沿触发
	Ex_NVIC_Config(GPIO_C,13,FTIR);//下降沿触发
//	Ex_NVIC_Config(GPIO_A,15,FTIR);//下降沿触发

//	MY_NVIC_Init(2,2,EXTI0_IRQChannel,2);    //抢占2，子优先级2，组2
	MY_NVIC_Init(2,1,EXTI15_10_IRQChannel,2);//抢占2，子优先级1，组2	   
}












