#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//ͨ�ö�ʱ�� ��������			   
////////////////////////////////////////////////////////////////////////////////// 	  
int Second;						//����
u8 Inquiry_Second;							//����
//��ʱ��3�жϷ������	 
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//����ж�
	{
//		printf("Auto_hand---%x\n",Auto_hand);
//		Time_flag = ~Time_flag;			
		if(Auto_hand==0x1a)
			Run_plan();
		if(Auto_hand==0x19)
			Stage_Hand_Mode();
		Second++;
		Inquiry_Second++;

	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}
//��ʱ��2�жϷ������	 
void TIM2_IRQHandler(void)
{ 		   	  			    
	if(TIM2->SR&0X0001)//����ж�
	{
		
// 		if(Manual_Phase[0])
// 		{	Manual_Mode(Manual_Phase[1]);
// 			TIM3->CR1&=~(1<<0);  //�رն�ʱ��3	
// 		}
// 		else TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3 	
	}				   
	TIM2->SR&=~(1<<0);//����жϱ�־λ 	
    
}
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<0;	//TIM2ʱ��ʹ�� 
	RCC->APB1ENR|=1<<1;	//TIM3ʱ��ʹ��   
 	TIM2->ARR=7200;  	//�趨�������Զ���װֵ//�պ�1ms 
	TIM3->ARR=arr;  	//�趨�������Զ���װֵ//�պ�1ms   
	TIM2->PSC=10;  	//Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��
	TIM3->PSC=psc;  	//Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��		  
	TIM2->DIER|=1<<0;   //��������ж�
	TIM3->DIER|=1<<0;   //��������ж�	  
//	TIM2->CR1|=0x01;    //ʹ�ܶ�ʱ��2
//	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
//	MY_NVIC_Init(1,0,TIM2_IRQChannel,1);//��ռ1�������ȼ�3����2
	MY_NVIC_Init(1,0,TIM3_IRQChannel,2);//��ռ1�������ȼ�3����2									 
}
//��ʱ��7�жϷ������		    
void TIM7_IRQHandler(void)
{ 	
	if(TIM7->SR&0X01)//�Ǹ����ж�
	{	 		
//		UART3_RX_STA|=1<<15;	//��ǽ������
		UART4_RX_STA|=1<<15;	//��ǽ������
		TIM7->SR&=~(1<<0);		//����жϱ�־λ		   
		TIM7->CR1&=~(1<<0);//�رն�ʱ��4	   
	}	    
}
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM7_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<5;	//TIM4ʱ��ʹ��    
 	TIM7->ARR=arr;  	//�趨�������Զ���װֵ   
	TIM7->PSC=psc;  	//Ԥ��Ƶ��
 	TIM7->DIER|=1<<0;   //��������ж�				
 	TIM7->CR1|=0x01;  	//ʹ�ܶ�ʱ��4	  	   
  MY_NVIC_Init(1,3,TIM7_IRQChannel,2);//��ռ2�������ȼ�3����2	��2�����ȼ����								 
}








