#include "sys.h"		    
#include "usart.h"	 
#include "delay.h"
#include "mbi5025.h"
#include "timer.h"



u8 RS_Flag=0;//重新发送标志
u8 RS_Data=0;
u8 RS_Check=0;
#ifdef EN_USART1_RX   	//如果使能了接收


//接收缓存区 	
u8 RS485_RX_BUF[64];  	//接收缓冲,最大64个字节.
//接收到的数据长度
u8 RS485_RX_CNT=0;   		  
//u8 RX_Flag=0;  //1 收到数据 0 未收到数据

u16 flag=0;
u8 buf[9]={0,0,0,0,0,0,0,0,0};
u8 buf_cnt=0;
void USART1_IRQHandler(void)
{	
 	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收到数据
	{	 	 			 
		buf[buf_cnt]=USART_ReceiveData(USART1); 	//读取接收到的数据
		
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
//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率	  
void RS485_Init(u32 bound)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA,G时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART2时钟
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //PG9端口配置
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	//PA2
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

		RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,ENABLE);//复位串口2
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1,DISABLE);//停止复位
 
	
 #ifdef EN_USART1_RX		  	//如果使能了接收
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;//8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;///奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式
	
	USART_WakeUpConfig(USART1, USART_WakeUp_AddressMark);
	USART_SetAddress(USART1, 0x0c);
	USART_ReceiverWakeUpCmd(USART1,ENABLE);

  USART_Init(USART1, &USART_InitStructure); //初始化串口
  
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //使能串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启接收非空中断
   
    USART_Cmd(USART1, ENABLE);                    //使能串口 

 #endif

 RS485_TX_EN=0;			//默认为接收模式

  
 
}

//RS485发送len个字节.
//buf:发送区首地址 
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	RS485_TX_EN=1;			//设置为发送模式
  	for(t=0;t<len;t++)		//循环发送数据
	{		   
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART1,buf[t]);
	}	 
 
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	RS485_RX_CNT=0;	  
	RS485_TX_EN=0;				//设置为接收模式	
}
void RS485_Send_2bData(u8 m,u8 n)
	{
		RS485_TX_EN=1;			//设置为发送模式  
		
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	 //判断是否发送完成，未发送完成继续等待
		USART_SendData(USART1,m);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	 //判断是否发送完成，未发送完成继续等待
		USART_SendData(USART1,n);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	 //判断是否发送完成，未发送完成继续等待
		RS485_TX_EN=0;			//关闭发送模式
//		RX_Flag=0;
	}
//RS485查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//默认为0
	delay_ms(10);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
	if(rxlen==RS485_RX_CNT&&rxlen)//接收到了数据,且接收完成了
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//记录本次数据长度
		RS485_RX_CNT=0;		//清零
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
	RS485_TX_EN=1;			//设置为发送模式  
	while((USART1->SR&0X40)==0);//等待发送结束	
	USART1->DR=(0x0c|0x100);	//写地址	
	while((USART1->SR&0X40)==0);//等待发送结束	
	USART1->DR=0xc7;	//写地址	
	while((USART1->SR&0X40)==0);//等待发送结束	
	USART1->DR=0x00;	//写地址	
	while((USART1->SR&0X40)==0);//等待发送结束	
	USART1->DR=0X04;	//写地址	
	while((USART1->SR&0X40)==0);//等待发送结束	
	USART1->DR=0X01;	//写地址	
	while((USART1->SR&0X40)==0);//等待发送结束	
	USART1->DR=0XCF;	//写地址	
	while((USART1->SR&0X40)==0);//等待发送结束	
	USART1->DR=0X01;	//写地址	
	while((USART1->SR&0X40)==0);//等待发送结束	
	USART1->DR=Buffer;	//写地址	
	while((USART1->SR&0X40)==0);//等待发送结束	
	USART1->DR=(~sum&0xff)+1;	//写地址	
	while((USART1->SR&0X40)==0);//等待发送结束	
	RS485_TX_EN=0;	
	RS_Data=Buffer;
	RS_Check=(~sum&0xff)+1;
}





	
	
	
	
	
	
	
	