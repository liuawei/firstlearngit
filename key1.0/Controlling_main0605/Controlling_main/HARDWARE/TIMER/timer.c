#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//通用定时器 驱动代码			   
////////////////////////////////////////////////////////////////////////////////// 	  
int Second;						//秒数
u8 Inquiry_Second;							//秒数
//定时器3中断服务程序	 
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{
//		printf("Auto_hand---%x\n",Auto_hand);
//		Time_flag = ~Time_flag;			
		if(Auto_hand==0x1a)
			Run_plan();
		if(Auto_hand==0x19)
			Stage_Hand_Mode();
		Second++;
		Inquiry_Second++;

	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}
//定时器2中断服务程序	 
void TIM2_IRQHandler(void)
{ 		   	  			    
	if(TIM2->SR&0X0001)//溢出中断
	{
		
// 		if(Manual_Phase[0])
// 		{	Manual_Mode(Manual_Phase[1]);
// 			TIM3->CR1&=~(1<<0);  //关闭定时器3	
// 		}
// 		else TIM3->CR1|=0x01;    //使能定时器3 	
	}				   
	TIM2->SR&=~(1<<0);//清除中断标志位 	
    
}
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<0;	//TIM2时钟使能 
	RCC->APB1ENR|=1<<1;	//TIM3时钟使能   
 	TIM2->ARR=7200;  	//设定计数器自动重装值//刚好1ms 
	TIM3->ARR=arr;  	//设定计数器自动重装值//刚好1ms   
	TIM2->PSC=10;  	//预分频器7200,得到10Khz的计数时钟
	TIM3->PSC=psc;  	//预分频器7200,得到10Khz的计数时钟		  
	TIM2->DIER|=1<<0;   //允许更新中断
	TIM3->DIER|=1<<0;   //允许更新中断	  
//	TIM2->CR1|=0x01;    //使能定时器2
//	TIM3->CR1|=0x01;    //使能定时器3
//	MY_NVIC_Init(1,0,TIM2_IRQChannel,1);//抢占1，子优先级3，组2
	MY_NVIC_Init(1,0,TIM3_IRQChannel,2);//抢占1，子优先级3，组2									 
}
//定时器7中断服务程序		    
void TIM7_IRQHandler(void)
{ 	
	if(TIM7->SR&0X01)//是更新中断
	{	 		
//		UART3_RX_STA|=1<<15;	//标记接收完成
		UART4_RX_STA|=1<<15;	//标记接收完成
		TIM7->SR&=~(1<<0);		//清除中断标志位		   
		TIM7->CR1&=~(1<<0);//关闭定时器4	   
	}	    
}
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM7_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<5;	//TIM4时钟使能    
 	TIM7->ARR=arr;  	//设定计数器自动重装值   
	TIM7->PSC=psc;  	//预分频器
 	TIM7->DIER|=1<<0;   //允许更新中断				
 	TIM7->CR1|=0x01;  	//使能定时器4	  	   
  MY_NVIC_Init(1,3,TIM7_IRQChannel,2);//抢占2，子优先级3，组2	在2中优先级最低								 
}








