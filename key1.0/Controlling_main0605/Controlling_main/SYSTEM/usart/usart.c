#include "sys.h"
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
		u8 buf[1];
		buf[0] = ch;
		DMA_Send_Buff(buf,1);
//	while((UART4->SR&0X40)==0);//ѭ������,ֱ���������   
//	UART4->DR = (u8) ch;     

	return ch;
}
#endif 

//////////////////////////////////////////////////////////////////

#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������	    
void USART1_IRQHandler(void)
{
	u8 res;	    
	if(USART1->SR&(1<<5))//���յ�����
	{	 
		res=USART1->DR; 			 
		if(RS485_CNT[0]<32)
		{
			RS485_BUF[0][RS485_CNT[0]]=res;		//��¼���յ���ֵ
			RS485_CNT[0]++;						//������������1 
		} 
	}  
} 
#endif

#ifdef EN_USART2_RX   	//���ʹ���˽���   	  
//���ջ����� 	
void USART2_IRQHandler(void)
{
	u8 res;	    
	if(USART2->SR&(1<<5))//���յ�����
	{	 
		res=USART2->DR; 		
	if(res&0x100)return;					//���ε�ַ����				
		if(RS485_CNT[1]<32)
		{
			RS485_BUF[1][RS485_CNT[1]]=res;		//��¼���յ���ֵ
			RS485_CNT[1]++;						//������������1 
		} 
	}  											 
} 
#endif
										 
#if EN_USART3_RX   //���ʹ���˽���
//����4�жϷ������	
u16 UART3_RX_STA=0;   						//��������״̬ 	    
void USART3_IRQHandler(void)
{
	u16 res;	    
	if(USART3->SR&(1<<5))//���յ�����
	{	 
	
		res = USART3->DR; 
		if(res&0x100)return;					//���ε�ַ����
		if((UART3_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
		{if(UART3_RX_STA<10)		//�����Խ�������
		{
			TIM7->CNT=0;         					//���������
			if(UART3_RX_STA==0)TIM7->CR1|=1<<0;     //ʹ�ܶ�ʱ��4	 	
			RS485_BUF[2][UART3_RX_STA++]=res;		//��¼���յ���ֵ	
		}
		else 
		{
			UART3_RX_STA|=1<<15;					//ǿ�Ʊ�ǽ������
		}
	}
	}  
} 
#endif

//u8 CONF_DATA[700];//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
u8  UART4_RX_BUF[UART4_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
u16 UART4_RX_STA=0;   
u8 paln_plag;    				//�������ձ�־	
#if EN_UART4_RX   //���ʹ���˽���
//����4�жϷ������	    
void UART4_IRQHandler(void)
{
	u8 res;
	if(UART4->SR&(1<<5))//���յ�����
	{	 
		res=UART4->DR;

			if((UART4_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
		{
		if(UART4_RX_STA<UART4_MAX_RECV_LEN)		//�����Խ�������
		{
			TIM7->CNT=0;         					//���������
			if(UART4_RX_STA==0)TIM7->CR1|=1<<0;     //ʹ�ܶ�ʱ��4	 	
			UART4_RX_BUF[UART4_RX_STA++]=res;		//��¼���յ���ֵ	
		}
		else 
		{
			UART4_RX_STA|=1<<15;					//ǿ�Ʊ�ǽ������
		}
		}		
	} 
 
} 
#endif

#if EN_UART5_RX   //���ʹ���˽���
u8 UART5_RX_BUF[12];  	//���ջ���,���10���ֽ�.
u8 UART5_RX_CNT=0;   	//���յ������ݳ���
u8 UART5_MARk = 0;

//����5�жϷ������	    
void UART5_IRQHandler(void)
{

		u8 res;	
	if(UART5->SR&(1<<5))//?????
	{	 
		res=UART5->DR;
		GPS(res); 	
	}
} 
#endif
u8 USART3_RXBuff[51];										 
//��ʼ��IO ����
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	 	  
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction;
	 
	RCC->APB2ENR|=1<<14;  	//ʹ�ܴ���1ʱ��
	RCC->APB1ENR|=15<<17;  	//ʹ�ܴ���2,3,4,5ʱ��
	RCC->APB2ENR|=15<<2;   	//ʹ��PORTA,B,C,D��ʱ�� 
	GPIOA->CRH&=0XFFFFF00F; //IO״̬���ô���1
	GPIOA->CRH|=0X000008B0;	//IO״̬����
	GPIOA->CRL&=0XFFFF00FF;	//IO״̬���ô���2
	GPIOA->CRL|=0X00008B00;	//IO״̬����
	GPIOB->CRH&=0XFFFF00FF;	//IO״̬���ô���3
	GPIOB->CRH|=0X00008B00;	//IO״̬����
	GPIOC->CRH&=0XFFF000FF;	//IO״̬���ô���4
	GPIOC->CRH|=0X000B8B00;	//IO״̬����
	GPIOD->CRL&=0XFFFFF0FF;	//IO״̬���ô���5
	GPIOD->CRL|=0X00000800;	//IO״̬����
	
	RCC->APB2RSTR|=1<<14;    //��λ����1
	RCC->APB2RSTR&=~(1<<14); //ֹͣ��λ	 	  
	RCC->APB1RSTR|=15<<17;   //��λ����2,3,4,5
	RCC->APB1RSTR&=~(15<<17);//ֹͣ��λ	   	   	   


	//����������
	USART1->BRR=mantissa; 	// ���������ô���1	 
	USART1->CR1|=0X200C;  	//1λֹͣ,��У��λ.
 	USART2->BRR=(36000000)/(19200); 	// ���������ô���2	RS485 
	USART2->CR1|=0X380E;  	//1λֹͣ,9λ����λ.����ַ��ǻ���
	USART3->BRR=(36000000)/(19200);; 	// ���������ô���3  
	USART3->CR1|=0X380E;  	//1λֹͣ,9λ����λ.����ַ��ǻ���
	UART4->BRR=(36000000)/(57600);; 	// ���������ô���4		
	UART4->CR1|=0X200C;  	//1λֹͣ,��У��λ.
	UART5->BRR=(36000000)/(9600); ; 	// ����������GPS
	UART5->CR1|=0X200C;  	//1λֹͣ,��У��λ.
//	UART4->CR3=1<<7;   		//ʹ�ܴ���4��DMA����
//	MYDMA_Config(DMA2_Channel5,(u32)&UART4->DR,(u32)CONF_DATA,5168);

#ifdef EN_USART1_RX		  //�������1ʹ���˽���
	USART1->CR1|=1<<8;    //PE�ж�ʹ��
	USART1->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,2,USART1_IRQChannel,2);//��2���� ���ȼ� 
#endif

#ifdef EN_USART2_RX		  //�������2ʹ���˽���
 	USART2->CR2|=Mian_addr;    //������ַ����Ϊ15
	USART2->CR1|=1<<8;    //PE�ж�ʹ��
	USART2->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,1,USART2_IRQChannel,2);//��2��������ȼ� 
#endif

#ifdef EN_USART3_RX		  //�������3ʹ���˽���
	USART3->CR2|=Mian_addr;//������ַ����
	USART3->CR1|=1<<8;    //PE�ж�ʹ��
	USART3->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    
	UART3_RX_STA=0;	
	MY_NVIC_Init(0,3,USART3_IRQChannel,2);//��2��������ȼ� 
#endif

#ifdef EN_UART4_RX		  //�������4ʹ���˽���

	UART4->CR1|=1<<8;     //PE�ж�ʹ��
	UART4->CR1|=1<<5;     //���ջ������ǿ��ж�ʹ��	    	
	UART4->CR3|=1<<7;			//ʹ��DMA
	MY_NVIC_Init(0,4,UART4_IRQChannel,2);//��2��������ȼ� 	
 	TIM7_Init(99,7199);	//10ms�ж�
 	UART4_RX_STA=0;			//����
 	TIM7->CR1&=~(1<<0);//�رն�ʱ��4	   
#endif

#ifdef EN_UART5_RX		  //�������5ʹ���˽���
	UART5->CR1|=1<<8;     //PE�ж�ʹ��
	UART5->CR1|=1<<5;     //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(2,4,UART5_IRQChannel,2);//��2��������ȼ� 
#endif
//	MYDMA_Config(DMA1_Channel2,(u32)&USART3->DR,(u32)USART3_RXBuff,51);//DMA1ͨ��2,����Ϊ����3,�洢��ΪSendBuff,����51
}















