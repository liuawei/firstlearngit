#include "bsp_led.h" 
#include "sys.h"   
 /**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
void nms(u16 i)
{
  u16 j,k;
	for(j=0;j<i;j++)
	  for(k=0;k<10000;k++)
	      {;}
}
void GPIO_Config(void)
{		GPIO_InitTypeDef GPIO_InitStructure;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
    //PB13 Pb14  PA15 PA12 是四路  分时复用  开关时序
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	
    //PA4 5 6 7 是四个LED状态指示灯。
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); 													   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_12|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	  //PB0 1 2 3 4 5 6 7 8 9 10 11 是12位拨码开关接口
	  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 													   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	  //时间程度设定
	  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); 													   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		GPIO_SetBits(GPIOA,GPIO_Pin_15);
		GPIO_SetBits(GPIOA,GPIO_Pin_12);		
		GPIO_SetBits(GPIOB,GPIO_Pin_14);		
		GPIO_SetBits(GPIOB,GPIO_Pin_13);
		
			
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 
	 	/*选择要控制的GPIOD引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;	
    
	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU/*GPIO_Mode_Out_OD*/;   

	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/*调用库函数，初始化GPIOD*/
  	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	  
	  PWR->CR|=1<<8;
    RCC->BDCR&=0xFFFFFFFE;
    BKP->CR&=0xFFFFFFFE;
    PWR->CR&=0xFFFFFEFF;
    	
}

void Led_Start(void)
{   LED1(ON);LED2(ON);LED3(ON);LED4(ON);
    nms(1000);
	  LED1(OFF);LED2(OFF);LED3(OFF);LED4(OFF);
}
void Switch_Set() 
{   u16 lmd,lmd1,lmd2,lmd3,add,time_time;
	  add=address;
	  switch(add)
	  { case 0 :  nms(10); add=address;
	             if(0==add)  {Mcu_Address=7;}
               break;
      case 1 :  nms(10);add=address;
	             if(1==add)  {Mcu_Address=8;}
               break;
			case 2 :  nms(10);add=address;
	             if(2==add)  {Mcu_Address=9;}
               break;
			case 3 :  nms(10);add=address;
	             if(3==add)  {Mcu_Address=10;}
               break;
			default: break;
		} 
	  time_time=tim_temp;
	  switch(time_time)
	  { case 1 :  nms(10);time_time=tim_temp;
	             if(1==time_time)  {Tim_Value=240;}
               break;
      case 2 :  nms(10);time_time=tim_temp;
	             if(2==time_time)  {Tim_Value=60;}
               break;
			case 4 :  nms(10);time_time=tim_temp;
	             if(4==time_time)  {Tim_Value=30;}
               break;
		
			default: break;
		}
	  lmd=Sens1;
    switch(lmd)
	  { case 1 :  nms(10); lmd=Sens1;
	             if(1==lmd)  {Sensity1=700;}
               break;
      case 2 :  nms(10); lmd=Sens1;
	             if(2==lmd)   {Sensity1=800;}
               break;
			case 8 :  nms(10); lmd=Sens1;
	             if(8==lmd)   {Sensity1=900;}
               break;
			default: break;
		} 
		lmd1=Sens2;
		switch(lmd1)
	  { case 1 :  nms(10); lmd1=Sens2;
	             if(1==lmd1)  {Sensity2=700;}
               break;
      case 2 :  nms(10); lmd1=Sens2;
	             if(2==lmd1)  {Sensity2=800;}
               break;
			case 4 :  nms(10); lmd1=Sens2;
	             if(4==lmd1)  {Sensity2=900;}
               break;
			default: break;
		} 
		lmd2=Sens3;
		switch(lmd2)
	  { case 1 :  nms(10); lmd2=Sens3;
	             if(1==lmd2)  {Sensity3=700;}
      case 2 :  nms(10); lmd2=Sens3;
	             if(2==lmd2)  {Sensity3=800;}
               break;
			case 4 :  nms(10); lmd2=Sens3;
	             if(4==lmd2)  {Sensity3=900;}
               break;
			default: break;
		} 
		lmd3=Sens4;
		switch(lmd3)
	  { case 1 :  nms(10); lmd3=Sens4;
	             if(1==lmd3)  {Sensity4=700; }
               break;
      case 2 :  nms(10); lmd3=Sens4;
	             if(2==lmd3)  {Sensity4=800; }
               break;
			case 4 :  nms(10); lmd3=Sens4;
	             if(4==lmd3)  {Sensity4=900; }
               break;
			default: break;
		}
}
