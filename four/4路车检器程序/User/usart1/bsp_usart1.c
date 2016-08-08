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


void Usart_Start(void)
	{
		Usart1(0xfe);
	}
	
void Usart_CMD(u8 cmd)
	{
		Usart1(cmd);
	}
	
void Usart_Res(u8 Res1,u8 Res2)
	{
		Usart1(Res1);
		Usart1(Res2);
	}
void Usart_ID(u8 ID_L,u8 ID_H)
	{
		Usart1(ID_L);
		Usart1(ID_H);
	}

void Usart_Len(u8 Len_L,u8 Len_H)
	{
		Usart1(Len_L);
		Usart1(Len_H);
	}

void Usart_Data(u8 Lance,u8 Speed,u8 State,u8 QueceLen)
	{
		u8 i;
		Usart1(Lance);
		Usart1(Speed);
		Usart1(State);
		Usart1(QueceLen);
		for(i=0;i<24;i++)
		Usart1(0x00);
	}
	
u8 Usart_Start_Return(void)
	{
		return 0xfe;
	}
	
u8 Usart_CMD_Return(u8 cmd)
	{
		return cmd;
	}
	
u8 Usart_Res_Return(u8 Res1,u8 Res2)
	{
		return Res1^Res2;
	}
u8 Usart_ID_Return(u8 ID_L,u8 ID_H)
	{
		return ID_L^ID_H;
	}

u8 Usart_Len_Return(u8 Len_L,u8 Len_H)
	{
		return Len_L^Len_H;
	}

u8 Usart_Data_Return(u8 Lance,u8 Speed,u8 State,u8 QueceLen)
	{
		return Lance^Speed^State^QueceLen;//异或0的值不变，所以其他位省略
	}
void Usart_Check(u8 lance)
	{
		u8 check;
		check=Usart_Start_Return()^Usart_CMD_Return(0x01)^Usart_Res_Return(0xff,0xff)^Usart_ID_Return(0x1c,0)^Usart_Data_Return(lance,0,0,0);
		Usart1(check);
	}
		
void real_time(void)
{
		if(Lance==1)
			{				
				LED1(ON);
				Usart_Start();
				Usart_CMD(0x01);
				Usart_Res(0xff,0xff);				
				Usart_Len(0x1c,0);
				Usart_Data(Lance,0x00,0,0);
				Usart_Check(Lance);
				Lance=0;
			}
		else
		if(Lance==2)
			{				
				LED2(ON);
				Usart_Start();
				Usart_CMD(0x01);
				Usart_Res(0xff,0xff);				
				Usart_Len(0x1c,0);
				Usart_Data(Lance,0x00,0,0);
				Usart_Check(Lance);
				Lance=0;
			}
		else
		if(Lance==3)
			{				
				LED3(ON);
				Usart_Start();
				Usart_CMD(0x01);
				Usart_Res(0xff,0xff);				
				Usart_Len(0x1c,0);
				Usart_Data(Lance,0x00,0,0);
				Usart_Check(Lance);
				Lance=0;
			}
		
		if(Lance==4)
			{				
				LED4(ON);
				Usart_Start();
				Usart_CMD(0x01);
				Usart_Res(0xff,0xff);				
				Usart_Len(0x1c,0);
				Usart_Data(Lance,0x00,0,0);
				Usart_Check(Lance);
				Lance=0;
			}
				
	
}

