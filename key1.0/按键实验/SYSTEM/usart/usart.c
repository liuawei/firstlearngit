#include "sys.h"		    
#include "usart.h"	 
#include "delay.h"
#include "mbi5025.h"
#include "timer.h"



u8 RS_Flag=0;//���·��ͱ�־
u8 RS_Data=0;
u8 RS_Check=0;
#ifdef EN_USART1_RX   	//���ʹ���˽���


//���ջ����� 	
u8 RS485_RX_BUF[64];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
u8 RS485_RX_CNT=0;   		  
//u8 RX_Flag=0;  //1 �յ����� 0 δ�յ�����

u16 flag=0;
u8 buf[9]={0,0,0,0,0,0,0,0,0};
u8 buf_cnt=0;
void USART1_IRQHandler(void)
{	
 	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //���յ�����
	{	 	 			 
		buf[buf_cnt]=USART_ReceiveData(USART1); 	//��ȡ���յ�������
		
		if(buf[1]==0xc7&&buf[2]==0x00&&buf[3]==0x04&&buf[4]==0x01&&buf[5]==0xcf&&buf[6]==0x01&&buf[7]==RS_Data&&buf[8]==RS_Check)
		{
				switch (RS_Flag)
				{
					case 1:
								flag=0x01;
								time_time=0;
						break;
					case 2: 
								flag=0x02;
								time_time=0;
						break;
					case 3: 
								flag=0x04;
								time_time=0;
						break;
					case 4: 
								flag=0x08;
								time_time=0;
						break;
					case 5:
								flag=0x010;
								time_time=0;
						break;
					case 6: 
								flag=0x020;
								time_time=0;
						break;
					case 7: 
								flag=0x040;
								time_time=0;
						break;
					case 8: 
								flag=0x080;
								time_time=0;
						break;
					case 9:
								flag=0x0100;
								time_time=0;
						break;
					case 10: 
								flag=0x0200;
								time_time=0;
						break;
					case 11: 
								flag=0x0400;
								time_time=0;
						break;
					case 12: 
								flag=0x0800;
								time_time=0;

						break;
					default:
						break;
					}
			}
		buf_cnt++;
		if(buf_cnt==9)buf_cnt=0;
	}  											 
} 
#endif										 
//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void RS485_Init(u32 bound)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOA,Gʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART2ʱ��
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //PG9�˿�����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	//PA2
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

		RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,ENABLE);//��λ����2
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,DISABLE);//ֹͣ��λ
 
	
 #ifdef EN_USART1_RX		  	//���ʹ���˽���
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;///��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
	
	USART_WakeUpConfig(USART1, USART_WakeUp_AddressMark);
	USART_SetAddress(USART1, 0x0c);
	USART_ReceiverWakeUpCmd(USART1,ENABLE);

  USART_Init(USART1, &USART_InitStructure); //��ʼ������
  
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //ʹ�ܴ���2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������շǿ��ж�
   
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

 #endif

 RS485_TX_EN=0;			//Ĭ��Ϊ����ģʽ

  
 
}

//RS485����len���ֽ�.
//buf:�������׵�ַ 
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	RS485_TX_EN=1;			//����Ϊ����ģʽ
  	for(t=0;t<len;t++)		//ѭ����������
	{		   
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART1,buf[t]);
	}	 
 
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	RS485_RX_CNT=0;	  
	RS485_TX_EN=0;				//����Ϊ����ģʽ	
}
void RS485_Send_2bData(u8 m,u8 n)
	{
		RS485_TX_EN=1;			//����Ϊ����ģʽ  
		
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	 //�ж��Ƿ�����ɣ�δ������ɼ����ȴ�
		USART_SendData(USART1,m);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	 //�ж��Ƿ�����ɣ�δ������ɼ����ȴ�
		USART_SendData(USART1,n);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	 //�ж��Ƿ�����ɣ�δ������ɼ����ȴ�
		RS485_TX_EN=0;			//�رշ���ģʽ
//		RX_Flag=0;
	}
//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==RS485_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//��¼�������ݳ���
		RS485_RX_CNT=0;		//����
	}
}
void RS485_ReSend_2bData(u8 t)
	{
		switch(t)
		{
			case 1:RS485_Send_2bData(0xa0,0x01);break;
			case 2:RS485_Send_2bData(0xa1,0x02);break;
			case 3:RS485_Send_2bData(0xf0,0x03);break;
			case 4:RS485_Send_2bData(0xff,0x04);break;
			case 5:RS485_Send_2bData(0x00,0x05);break;
		}
	}
void Ent_Write_Data(u8 Buffer)
{
	u16 sum,data_sum;
	sum = 0X01+0XCF+0X01;
		
		sum+=Buffer;	
	data_sum = 0X01+3;
	if(data_sum%2)
		sum+=1;
	RS485_TX_EN=1;			//����Ϊ����ģʽ  
	while((USART1->SR&0X40)==0);//�ȴ����ͽ���	
	USART1->DR=(0x0c|0x100);	//д��ַ	
	while((USART1->SR&0X40)==0);//�ȴ����ͽ���	
	USART1->DR=0xc7;	//д��ַ	
	while((USART1->SR&0X40)==0);//�ȴ����ͽ���	
	USART1->DR=0x00;	//д��ַ	
	while((USART1->SR&0X40)==0);//�ȴ����ͽ���	
	USART1->DR=0X04;	//д��ַ	
	while((USART1->SR&0X40)==0);//�ȴ����ͽ���	
	USART1->DR=0X01;	//д��ַ	
	while((USART1->SR&0X40)==0);//�ȴ����ͽ���	
	USART1->DR=0XCF;	//д��ַ	
	while((USART1->SR&0X40)==0);//�ȴ����ͽ���	
	USART1->DR=0X01;	//д��ַ	
	while((USART1->SR&0X40)==0);//�ȴ����ͽ���	
	USART1->DR=Buffer;	//д��ַ	
	while((USART1->SR&0X40)==0);//�ȴ����ͽ���	
	USART1->DR=(~sum&0xff)+1;	//д��ַ	
	while((USART1->SR&0X40)==0);//�ȴ����ͽ���	
	RS485_TX_EN=0;	
	RS_Data=Buffer;
	RS_Check=(~sum&0xff)+1;
}





	
	
	
	
	
	
	
	