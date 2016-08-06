#ifndef __USART_H
#define __USART_H			 
#include "sys.h"	 								  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//RS485驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
	  		  	
extern u8 RS485_RX_BUF[64]; 		//接收缓冲,最大64个字节
extern u8 RS485_RX_CNT;   			//接收到的数据长度
//extern u8 RX_Flag; 
extern u8 RS_Flag;
extern u16 flag;
extern u8 RS_Data;
extern u8 RS_Check;
	
//模式控制
#define RS485_TX_EN		PAout(11)	//485模式控制.0,接收;1,发送.
//如果想串口中断接收，请不要注释以下宏定义
#define EN_USART1_RX 	1			//0,不接收;1,接收.


void RS485_ReSend_2bData(u8 t);
void RS485_Send_2bData(u8 m,u8 n);
void RS485_Init(u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);
void RS485_Receive_Data(u8 *buf,u8 *len);
void Ent_Write_Data(u8 Buffer);

#endif	   




