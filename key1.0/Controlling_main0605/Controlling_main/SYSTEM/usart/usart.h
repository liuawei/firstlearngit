#ifndef __USART_H
#define __USART_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//串口初始化		   
//********************************************************************************
	  
 
	  	
//如果想串口中断接收，请不要注释以下宏定义
#define EN_USART1_RX 			1		//使能（1）/禁止（0）网口接收
#define EN_USART2_RX 			1		//使能（1）/禁止（0）RS485接收
#define EN_USART3_RX 			1		//使能（1）/禁止（0）SIM900接收
#define EN_UART4_RX 			1		//使能（1）/禁止（0）GPS接收
#define EN_UART5_RX 			1		//使能（1）/禁止（0）面板数据接收		

#define UART4_MAX_RECV_LEN		512				//最大接收缓存字节数
#define UART4_MAX_SEND_LEN		512				//最大发送缓存字节数

extern u8  UART4_RX_BUF[UART4_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern u8  UART4_TX_BUF[UART4_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern u16 UART4_RX_STA;   						//接收数据状态
extern u16 UART3_RX_STA;   						//接收数据状态

extern u8 USART3_RXBuff[51];
extern u8 RS485_BUF[5][32];  	//接收缓冲,最大64个字节.
//接收到的数据长度
extern u8 RS485_CNT[5];  
extern u8 RS485_CNT_tmp[5]; 
void uart_init(u32 pclk2,u32 bound);
void TIM7_Init(u16 arr,u16 psc);


#endif	   
















