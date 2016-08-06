#ifndef __USART_H
#define __USART_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//���ڳ�ʼ��		   
//********************************************************************************
	  
 
	  	
//����봮���жϽ��գ��벻Ҫע�����º궨��
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0�����ڽ���
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0��RS485����
#define EN_USART3_RX 			1		//ʹ�ܣ�1��/��ֹ��0��SIM900����
#define EN_UART4_RX 			1		//ʹ�ܣ�1��/��ֹ��0��GPS����
#define EN_UART5_RX 			1		//ʹ�ܣ�1��/��ֹ��0��������ݽ���		

#define UART4_MAX_RECV_LEN		512				//�����ջ����ֽ���
#define UART4_MAX_SEND_LEN		512				//����ͻ����ֽ���

extern u8  UART4_RX_BUF[UART4_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  UART4_TX_BUF[UART4_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern u16 UART4_RX_STA;   						//��������״̬
extern u16 UART3_RX_STA;   						//��������״̬

extern u8 USART3_RXBuff[51];
extern u8 RS485_BUF[5][32];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
extern u8 RS485_CNT[5];  
extern u8 RS485_CNT_tmp[5]; 
void uart_init(u32 pclk2,u32 bound);
void TIM7_Init(u16 arr,u16 psc);


#endif	   
















