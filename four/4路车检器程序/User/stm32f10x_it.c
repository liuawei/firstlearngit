/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_led.h"
#include "sys.h"
#include "bsp_usart2_485.h"
#include "bsp_usart1.h"
#include "stdlib.h"
u8 Count,Count1,Count2,Count3;
extern void TimingDelay_Decrement(void);
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{ 
}

void EXTI4_IRQHandler(void)
{
if(EXTI_GetITStatus(EXTI_Line4) != RESET)
{ 
  //WatchDog_Feed(); 
}
EXTI_ClearITPendingBit(EXTI_Line4);
}

void EXTI0_IRQHandler(void)
{

}
void TIM1_CC_IRQHandler(void)
{
 if(TIM_GetFlagStatus(TIM1,TIM_FLAG_CC4)==SET)
    {
       TIM_ClearFlag(TIM1,TIM_FLAG_CC4);
       if(capture_number == 0)
       {
         Ic1_readvalue1 = TIM_GetCapture4(TIM1);
         capture_number = 1;
         
       }
       else if(capture_number == 1)
       {
         if(TIM_GetFlagStatus(TIM1,TIM_FLAG_Update) != SET)
         {
           Ic1_readvalue2 = TIM_GetCapture4(TIM1);
           Ic1_readvalue2 = Ic1_readvalue2 - Ic1_readvalue1;
         }
         else
         {
           TIM_ClearFlag(TIM1,TIM_FLAG_Update);
           Ic1_readvalue2 = 0xFFFF - Ic1_readvalue1 + TIM_GetCapture4(TIM1)+1;
         }
         capture_number = 0;
         if(Ic1_readvalue2!=0)
         {
           TIM1_FREQ= 72000000/Ic1_readvalue2;

         }
         TIM1_FREQ = TIM1_FREQ;
				  
       }
}
}
void TIM2_IRQHandler(void)
{ 
	u16 Temp_Data1=0,Temp_Data2=0,Temp_Data3=0,Temp_Data4=0;
	if ( TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET ) 
  {  
		Ns_num++;Ns_num1++;
		if(Ns_num==1) 
		{ 
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,GPIO_Pin_15);		
		  GPIO_SetBits(GPIOB,GPIO_Pin_13);
		  GPIO_SetBits(GPIOB,GPIO_Pin_14);
    }
		if(Ns_num==4)
		{
			Temp_Data_ch1[Count]=TIM1_FREQ/100; 
			Count++;
      if(Count>4)					
      { 
				for(Count=0;Count<5;Count++)
				{
					if(abs(Temp_Data_ch1[Count+1]-Temp_Data_ch1[Count])<Sensity1)
					{
						Temp_Data1=Temp_Data1+Temp_Data_ch1[Count];
          }
					else
					break;
				}
				Temp_Data1=(Temp_Data1/Count)*100; 
				if(Temp_Data1>(Temp_Data_Buffer1+Sensity1))						 
				{
					Car_Num1++;
					Lance=1;
					
				}	 
				if(Car_Num1>250) Car_Num1=0;
				Count=0; 
				Temp_Data_Buffer1=Temp_Data1;	
			}
    }
		if(Ns_num==6) 
		{ 
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,GPIO_Pin_15);		
		  GPIO_SetBits(GPIOB,GPIO_Pin_13);
		  GPIO_SetBits(GPIOB,GPIO_Pin_14);
		}
	  if(Ns_num==9)
		{
			Temp_Data_ch2[Count1]=TIM1_FREQ/100; 
			Count1++;
      if(Count1>4)					
      { 
				for(Count1=0;Count1<5;Count1++)
				{
					if(abs(Temp_Data_ch2[Count1+1]-Temp_Data_ch2[Count1])<Sensity2)
					{
						Temp_Data2=Temp_Data2+Temp_Data_ch2[Count1];
          }
				  else
					break;
				}
				Temp_Data2=(Temp_Data2/Count1)*100;	
				if(Temp_Data2>(Temp_Data_Buffer2+Sensity2))						 
				{
					Car_Num2++;
					Lance=2;
					
				}
				if(Car_Num2>250) Car_Num2=0;
				Count1=0;
				Temp_Data_Buffer2=Temp_Data2;
			}
     }
		 if(Ns_num==11) 
		 { 
			 GPIO_SetBits(GPIOA,GPIO_Pin_12);
			 GPIO_SetBits(GPIOA,GPIO_Pin_15);		
		   GPIO_ResetBits(GPIOB,GPIO_Pin_13);
		   GPIO_SetBits(GPIOB,GPIO_Pin_14);
     }
		 if(Ns_num==14)
		 {
				Temp_Data_ch3[Count2]=TIM1_FREQ/100; 
				Count2++;
        if(Count2>4)					
        { 
					for(Count2=0;Count2<5;Count2++)
					{
						if(abs(Temp_Data_ch3[Count2+1]-Temp_Data_ch3[Count2])<Sensity3)
						{
							Temp_Data3=Temp_Data3+Temp_Data_ch3[Count2];
            }
						else
						break;
					}
          Temp_Data3=(Temp_Data3/Count2)*100;
          if(Temp_Data3>(Temp_Data_Buffer3+Sensity3))						 
          {
						Car_Num3++;
						Lance=3;
						
					}
					if(Car_Num3>250) Car_Num3=0;
					Count2=0;
					Temp_Data_Buffer3=Temp_Data3;
				} 
       }
			 if(Ns_num==16) 
			 { 
				 GPIO_SetBits(GPIOA,GPIO_Pin_12);
			   GPIO_SetBits(GPIOA,GPIO_Pin_15);		
		     GPIO_SetBits(GPIOB,GPIO_Pin_13);
		     GPIO_ResetBits(GPIOB,GPIO_Pin_14);
       }
			 if(Ns_num==19)
			 {
				 Temp_Data_ch4[Count3]=TIM1_FREQ/100; 
				 Count3++;
         if(Count3>4)					
         { 
					 for(Count3=0;Count3<5;Count3++)
					 {
						 if(abs(Temp_Data_ch4[Count3+1]-Temp_Data_ch4[Count3])<Sensity4)
						 {
								Temp_Data4=Temp_Data4+Temp_Data_ch4[Count3];
             }
						 else
						 break;
						}
						Temp_Data4=(Temp_Data4/Count3)*100;	
						if(Temp_Data4>(Temp_Data_Buffer4+Sensity4))						 
					  {
						  Car_Num4++;
							Lance=4;
	
					  }
					  if(Car_Num4>250) Car_Num4=0;
					  Count3=0;
					  Temp_Data_Buffer4=Temp_Data4;
					}
        }
				if(Ns_num>20) Ns_num=0;
				if(Ns_num1>200) 
			  {
					Ns_num1=0;
          Second_Num++;
					if(Second_Num>Tim_Value)
					{
						Second_Num=0;
//						Car_Num1_Buffer=Car_Num1;Car_Num1=0;
//            Car_Num2_Buffer=Car_Num2;Car_Num2=0;
//						Car_Num3_Buffer=Car_Num3;Car_Num3=0;
//						Car_Num4_Buffer=Car_Num4;Car_Num4=0;
           } 
         }
        
	    TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update); 			
  }		 	  	
   
}  

	
void TIM3_IRQHandler(void)
{ 
	if ( TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET ) 
   {      CLI(); 
		     

          SEI();
	  }		
     			
 		 TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);  		 
	 	
} 
void USART1_IRQHandler(void)
{  u8 res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)    
	{
		res = USART1->DR; 
		//Usart1(res);
		RS485_RX_BUF[RS485_RX_CNT] = res;  
    RS485_RX_CNT++; 
	 	
	}  		 
      USART_ClearITPendingBit(USART1, USART_IT_RXNE); 	 
}
