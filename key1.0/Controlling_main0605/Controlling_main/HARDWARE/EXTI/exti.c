#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "si4432.h"
//////////////////////////////////////////////////////////////////////////////////	 
//�ⲿ�ж� ��������			   	  
////////////////////////////////////////////////////////////////////////////////// 	  
 

//�ⲿ�ж�15~10�������
void EXTI15_10_IRQHandler(void)
{			
	if((nIRQ==0)&&Tx_Rx)
	SI4432_RX();		/**SI4432���պ���**/
	EXTI->PR=1<<13;     //���LINE13�ϵ��жϱ�־λ  
}
//�ⲿ�жϳ�ʼ������
//��ʼ��PA0,PA13,PA15Ϊ�ж�����.
void EXTIX_Init(void)
{
	RCC->APB2ENR|=1<<4;     //ʹ��PORTCʱ��
	JTAG_Set(JTAG_SWD_DISABLE);//�ر�JTAG��SWD   
  
	GPIOC->CRH&=0XFF0FFFFF;//PC13���ó�����	  
	GPIOC->CRH|=0X00800000; 				   
	GPIOC->ODR|=1<<13;	   //PC13����

//	Ex_NVIC_Config(GPIO_A,0,RTIR); //�����ش���
	Ex_NVIC_Config(GPIO_C,13,FTIR);//�½��ش���
//	Ex_NVIC_Config(GPIO_A,15,FTIR);//�½��ش���

//	MY_NVIC_Init(2,2,EXTI0_IRQChannel,2);    //��ռ2�������ȼ�2����2
	MY_NVIC_Init(2,1,EXTI15_10_IRQChannel,2);//��ռ2�������ȼ�1����2	   
}












