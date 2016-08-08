
#include "sys.h"
u8 Ns_num=0;
u8 Ns_num1=0;
u8 Second_Num=0;
u8 Ns=0;
u8 capture_number=0;
u16 Ic1_readvalue1=0;
u16 Ic1_readvalue2=0;
u16 CAPTURE=2000;
u16 TIM1_FREQ;

u16 Temp_Data_Buffer1;
u16 Temp_Data_Buffer2;
u16 Temp_Data_Buffer3;
u16 Temp_Data_Buffer4;
u16 Temp_Data_ch1[10]={0,0,0,0,0,0,0,0,0,0};
u16 Temp_Data_ch2[10]={0,0,0,0,0,0,0,0,0,0};
u16 Temp_Data_ch3[10]={0,0,0,0,0,0,0,0,0,0};
u16 Temp_Data_ch4[10]={0,0,0,0,0,0,0,0,0,0};
//u8  temp=0;
//u8  temp1=0;
//u8  temp2=0;
//u8  temp3=0;
u8  Mcu_Address;
u16  Sensity1;
u16  Sensity2;
u16  Sensity3;
u16  Sensity4;
u8  Car_Num1;
u8  Car_Num2;
u8  Car_Num3;
u8  Car_Num4;
u8  Car_Num1_Buffer;
u8  Car_Num2_Buffer;
u8  Car_Num3_Buffer;
u8  Car_Num4_Buffer;
u8  Lance=0;
u8  ydflag1=1;
u8  ydflag2=1;
u8  ydflag3=1;
u8  ydflag4=1;
u8   com_addr;
u8   Tim_Value;
u8 RS485_RX_BUF[3]={0,0,0}; 			//接收缓冲
u8 RS485_RX_CNT=0;
/*
   1直红 左红  绿人   0x89
   2直红 左绿  绿人   0x91
   3直红 左黄  绿人   0xa1
   4直绿 左绿  红人   0x52
   5直绿 左黄  红人   0x62
   6直绿 左红  红人   0x4a
   7直黄 左红  红人   0x4c
   8直黄 左绿  红人   0x54
   9直黄 左黄  红人   0x64
   10直黄 左黄        0x24
   11                 0x00
   12直红 左红  红人  0x49



*/
//系统初始化
void sys_init()
{
CLI();
GPIO_Config();
Switch_Set();
USART1_Config();
Tim1_IO_Config();
TIM2_Configuration();
Tim1_IO_Config();
TIM1_Configuration();
TIM3_Configuration();
NVIC_Configuration();//1 2
Led_Start();
SEI();
//WatchDog_Init(5,625);//2s
}


void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 

												
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;	
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  NVIC_InitStructure.NVIC_IRQChannel= TIM1_CC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;	
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	
	NVIC_InitStructure.NVIC_IRQChannel =USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
}

