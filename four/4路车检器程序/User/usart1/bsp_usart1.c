#include "bsp_usart1.h"
#include "sys.h"
 /**
  * @brief  USART1 GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
void USART1_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		
		/* config USART1 clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); 
		/* USART1 GPIO config */
		/* Configure USART1 Tx (PA.09) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		/* Configure USART1 Rx (PA.10) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
     /* 485收发控制管脚 */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_8); 		
		/* USART1 mode config */
		USART_InitStructure.USART_BaudRate =19200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		 
		USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);//如果打开会存在重大问题
		
//		USART_WakeUpConfig(USART1, USART_WakeUp_AddressMark);//唤醒方式
//		USART_SetAddress(USART1,Mcu_Address);//设置的节点地址
//		USART_ReceiverWakeUpCmd(USART1,ENABLE);//检查USART是否处于静默模式
		
		USART_Init(USART1, &USART_InitStructure);
		USART_Cmd(USART1, ENABLE);
}
void Usart1(u8 temp)
 { GPIO_SetBits(GPIOA,GPIO_Pin_8);
	 while(!(USART1->SR&0X40)); 
	 USART_SendData(USART1,temp);
	 while(!(USART1->SR&0X40)); 
	 GPIO_ResetBits(GPIOA,GPIO_Pin_8);

	}
void Usart1_address(u8 temp)
 { GPIO_SetBits(GPIOA,GPIO_Pin_8);
	 while(!(USART1->SR&0X40)); 
	 USART_SendData(USART1,temp|0x100);
	 while(!(USART1->SR&0X40)); 
	 GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	}	
void RS485_Write_Data(u8 *pBuffer,u8 Len)
{
	u8 i;
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	while((USART1->SR&0X40)==0);
	for(i=0;i<Len;i++)		//循环发送数据
	
	{
		while((USART1->SR&0X40)==0);//等待发送结束		  	
		USART1->DR = pBuffer[i];
	}	 
	while((USART1->SR&0X40)==0);//等待发送结束
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
}
void Usartt1(u8 temp)
 { 
	 while(!(USART1->SR&0X40)); 
	 USART_SendData(USART1,temp);
	 while(!(USART1->SR&0X40)); 	
	}
///重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}
/*********************************************END OF FILE**********************/
void real_time(void)
{
		u8 i=0;
		if(RS485_RealtimeData1)
			{
				RS485_RealtimeData1=0;
				LED1(ON);
				Usart1(0xfe);
				Usart1(0x01);
				Usart1(0xff);
				Usart1(0xff);
				Usart1(Mcu_Address);
				Usart1(0x00);
				Usart1(0x25);
				Usart1(0xff);
				Usart1(0x01);
				Usart1(0x00);
				Usart1(ydflag1);
				Usart1(0x00);
				for(i=0;i<24;i++)
				Usart1(0xff);	
			}
		
		if(RS485_RealtimeData2)
			{
				RS485_RealtimeData2=0;
				LED2(ON);
				Usart1(0xfe);
				Usart1(0x01);
				Usart1(0xff);
				Usart1(0xff);
				Usart1(Mcu_Address);
				Usart1(0x00);
				Usart1(0x25);
				Usart1(0xff);
				Usart1(0x02);
				Usart1(0x00);
				Usart1(ydflag2);
				Usart1(0x00);
				for(i=0;i<24;i++)
				Usart1(0xff);
				
			}
		
		if(RS485_RealtimeData3)
			{
				RS485_RealtimeData3=0;
				LED3(ON);
				Usart1(0xfe);
				Usart1(0x01);
				Usart1(0xff);
				Usart1(0xff);
				Usart1(Mcu_Address);
				Usart1(0x00);
				Usart1(0x25);
				Usart1(0xff);
				Usart1(0x03);
				Usart1(0x00);
				Usart1(ydflag3);
				Usart1(0x00);
				for(i=0;i<24;i++)
				Usart1(0xff);
			}
		
		if(RS485_RealtimeData4)
			{
				RS485_RealtimeData4=0;
				LED4(ON);
				Usart1(0xfe);
				Usart1(0x01);
				Usart1(0xff);
				Usart1(0xff);
				Usart1(Mcu_Address);
				Usart1(0x00);
				Usart1(0x25);
				Usart1(0xff);
				Usart1(0x04);
				Usart1(0x00);
				Usart1(ydflag4);
				Usart1(0x00);
				for(i=0;i<24;i++)
				Usart1(0xff);
			
			}
				
	
}

