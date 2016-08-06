#ifndef __RS485_H
#define __RS485_H			 
#include "sys.h"	 								  
//////////////////////////////////////////////////////////////////////////////////	 
//RS485驱动 代码	   									  
//////////////////////////////////////////////////////////////////////////////////	  	

//模式控制
#define RS485_TX_EN		PAout(11)	//485模式控制.1,接收;0,发送.
#define CON1		PAout(1)	//485模式控制.1,接收;0,发送.
#define CON2		PBout(1)	//485模式控制.1,接收;0,发送.
void RS485_Init(void);
void Write_Buff(USART_TypeDef *uart,u8 *pBuffer,u8 Len);
void RS485_Write_Data(USART_TypeDef *uart,u8 WriteAddr,u8 *pBuffer,u8 Len);
void RS485_Receive_Data(u8 con,u8 *buf,u8 *len);
void  Send_APhase(u8 *PhaseBuff);
void Send_Phase_State(u8 *PhaseBuff);
void Write_Data(u8 WriteAddr,u8 flag,u8 *pBuffer,u8 Len);
void DMA_Send_Buff(u8 *pBuffer,u16 Len);
void Bus1_Write_Data(u8 WriteAddr,u8 flag,u8 *pBuffer,u8 Len);
void Bus2_Write_Data(u8 WriteAddr,u8 flag,u8 *pBuffer,u8 Len);
void Ent_Write_Data(u8 cmd,u8 flag,u8 *pBuffer,u8 Len);
u8 Ccomm_board_data(void);
u8 Power_board_data(u8 cmd,u8 *rec_data);
#endif	   
















