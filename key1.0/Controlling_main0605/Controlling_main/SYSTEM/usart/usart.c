#include "sys.h"
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
		u8 buf[1];
		buf[0] = ch;
		DMA_Send_Buff(buf,1);
//	while((UART4->SR&0X40)==0);//循环发送,直到发送完毕   
//	UART4->DR = (u8) ch;     

	return ch;
}
#endif 

//////////////////////////////////////////////////////////////////

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序	    
void USART1_IRQHandler(void)
{
	u8 res;	    
	if(USART1->SR&(1<<5))//接收到数据
	{	 
		res=USART1->DR; 			 
		if(RS485_CNT[0]<32)
		{
			RS485_BUF[0][RS485_CNT[0]]=res;		//记录接收到的值
			RS485_CNT[0]++;						//接收数据增加1 
		} 
	}  
} 
#endif

#ifdef EN_USART2_RX   	//如果使能了接收   	  
//接收缓存区 	
void USART2_IRQHandler(void)
{
	u8 res;	    
	if(USART2->SR&(1<<5))//接收到数据
	{	 
		res=USART2->DR; 		
	if(res&0x100)return;					//屏蔽地址干扰				
		if(RS485_CNT[1]<32)
		{
			RS485_BUF[1][RS485_CNT[1]]=res;		//记录接收到的值
			RS485_CNT[1]++;						//接收数据增加1 
		} 
	}  											 
} 
#endif
										 
#if EN_USART3_RX   //如果使能了接收
//串口4中断服务程序	
u16 UART3_RX_STA=0;   						//接收数据状态 	    
void USART3_IRQHandler(void)
{
	u16 res;	    
	if(USART3->SR&(1<<5))//接收到数据
	{	 
	
		res = USART3->DR; 
		if(res&0x100)return;					//屏蔽地址干扰
		if((UART3_RX_STA&(1<<15))==0)//接收完的一批数据,还没有被处理,则不再接收其他数据
		{if(UART3_RX_STA<10)		//还可以接收数据
		{
			TIM7->CNT=0;         					//计数器清空
			if(UART3_RX_STA==0)TIM7->CR1|=1<<0;     //使能定时器4	 	
			RS485_BUF[2][UART3_RX_STA++]=res;		//记录接收到的值	
		}
		else 
		{
			UART3_RX_STA|=1<<15;					//强制标记接收完成
		}
	}
	}  
} 
#endif

//u8 CONF_DATA[700];//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
u8  UART4_RX_BUF[UART4_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
u16 UART4_RX_STA=0;   
u8 paln_plag;    				//方案接收标志	
#if EN_UART4_RX   //如果使能了接收
//串口4中断服务程序	    
void UART4_IRQHandler(void)
{
	u8 res;
	if(UART4->SR&(1<<5))//接收到数据
	{	 
		res=UART4->DR;

			if((UART4_RX_STA&(1<<15))==0)//接收完的一批数据,还没有被处理,则不再接收其他数据
		{
		if(UART4_RX_STA<UART4_MAX_RECV_LEN)		//还可以接收数据
		{
			TIM7->CNT=0;         					//计数器清空
			if(UART4_RX_STA==0)TIM7->CR1|=1<<0;     //使能定时器4	 	
			UART4_RX_BUF[UART4_RX_STA++]=res;		//记录接收到的值	
		}
		else 
		{
			UART4_RX_STA|=1<<15;					//强制标记接收完成
		}
		}		
	} 
 
} 
#endif

#if EN_UART5_RX   //如果使能了接收
u8 UART5_RX_BUF[12];  	//接收缓冲,最大10个字节.
u8 UART5_RX_CNT=0;   	//接收到的数据长度
u8 UART5_MARk = 0;

//串口5中断服务程序	    
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
//初始化IO 串口
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	 	  
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction;
	 
	RCC->APB2ENR|=1<<14;  	//使能串口1时钟
	RCC->APB1ENR|=15<<17;  	//使能串口2,3,4,5时钟
	RCC->APB2ENR|=15<<2;   	//使能PORTA,B,C,D口时钟 
	GPIOA->CRH&=0XFFFFF00F; //IO状态设置串口1
	GPIOA->CRH|=0X000008B0;	//IO状态设置
	GPIOA->CRL&=0XFFFF00FF;	//IO状态设置串口2
	GPIOA->CRL|=0X00008B00;	//IO状态设置
	GPIOB->CRH&=0XFFFF00FF;	//IO状态设置串口3
	GPIOB->CRH|=0X00008B00;	//IO状态设置
	GPIOC->CRH&=0XFFF000FF;	//IO状态设置串口4
	GPIOC->CRH|=0X000B8B00;	//IO状态设置
	GPIOD->CRL&=0XFFFFF0FF;	//IO状态设置串口5
	GPIOD->CRL|=0X00000800;	//IO状态设置
	
	RCC->APB2RSTR|=1<<14;    //复位串口1
	RCC->APB2RSTR&=~(1<<14); //停止复位	 	  
	RCC->APB1RSTR|=15<<17;   //复位串口2,3,4,5
	RCC->APB1RSTR&=~(15<<17);//停止复位	   	   	   


	//波特率设置
	USART1->BRR=mantissa; 	// 波特率设置串口1	 
	USART1->CR1|=0X200C;  	//1位停止,无校验位.
 	USART2->BRR=(36000000)/(19200); 	// 波特率设置串口2	RS485 
	USART2->CR1|=0X380E;  	//1位停止,9位数据位.被地址标记唤醒
	USART3->BRR=(36000000)/(19200);; 	// 波特率设置串口3  
	USART3->CR1|=0X380E;  	//1位停止,9位数据位.被地址标记唤醒
	UART4->BRR=(36000000)/(57600);; 	// 波特率设置串口4		
	UART4->CR1|=0X200C;  	//1位停止,无校验位.
	UART5->BRR=(36000000)/(9600); ; 	// 波特率设置GPS
	UART5->CR1|=0X200C;  	//1位停止,无校验位.
//	UART4->CR3=1<<7;   		//使能串口4的DMA发送
//	MYDMA_Config(DMA2_Channel5,(u32)&UART4->DR,(u32)CONF_DATA,5168);

#ifdef EN_USART1_RX		  //如果串口1使能了接收
	USART1->CR1|=1<<8;    //PE中断使能
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(0,2,USART1_IRQChannel,2);//组2，最 优先级 
#endif

#ifdef EN_USART2_RX		  //如果串口2使能了接收
 	USART2->CR2|=Mian_addr;    //本机地址设置为15
	USART2->CR1|=1<<8;    //PE中断使能
	USART2->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(0,1,USART2_IRQChannel,2);//组2，最高优先级 
#endif

#ifdef EN_USART3_RX		  //如果串口3使能了接收
	USART3->CR2|=Mian_addr;//本机地址设置
	USART3->CR1|=1<<8;    //PE中断使能
	USART3->CR1|=1<<5;    //接收缓冲区非空中断使能	    
	UART3_RX_STA=0;	
	MY_NVIC_Init(0,3,USART3_IRQChannel,2);//组2，最低优先级 
#endif

#ifdef EN_UART4_RX		  //如果串口4使能了接收

	UART4->CR1|=1<<8;     //PE中断使能
	UART4->CR1|=1<<5;     //接收缓冲区非空中断使能	    	
	UART4->CR3|=1<<7;			//使能DMA
	MY_NVIC_Init(0,4,UART4_IRQChannel,2);//组2，最低优先级 	
 	TIM7_Init(99,7199);	//10ms中断
 	UART4_RX_STA=0;			//清零
 	TIM7->CR1&=~(1<<0);//关闭定时器4	   
#endif

#ifdef EN_UART5_RX		  //如果串口5使能了接收
	UART5->CR1|=1<<8;     //PE中断使能
	UART5->CR1|=1<<5;     //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(2,4,UART5_IRQChannel,2);//组2，最低优先级 
#endif
//	MYDMA_Config(DMA1_Channel2,(u32)&USART3->DR,(u32)USART3_RXBuff,51);//DMA1通道2,外设为串口3,存储器为SendBuff,长度51
}















