#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK miniSTM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


//#define KEY0 PCin(5)   	
//#define KEY1 PAin(15)	 
//#define WK_UP  PAin(0)	 
 
#define KEY0  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)//读取按键0
#define S1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)//读取按键0
#define S2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)//读取按键1
#define S3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)//读取按键2 
#define S4  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)//读取按键0
#define S5  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)//读取按键1
#define S6  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)//读取按键2 
#define S7  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)//读取按键0
#define S8  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)//读取按键1
#define S9  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)//读取按键2 
#define S10 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)//读取按键0
#define S11 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//读取按键1
#define S12 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//读取按键2 
 

#define S1_PRES	1		//S1  
#define S2_PRES	2		//S2 
#define S3_PRES	3		//S3 
#define S4_PRES	4 	//S4  
#define S5_PRES	5		//S5 
#define S6_PRES	6		//S6 
#define S7_PRES	7		//S7  
#define S8_PRES	8		//S8 
#define S9_PRES	9		//S9 
#define S10_PRES	10		//S10  
#define S11_PRES	11		//S11 
#define S12_PRES	12		//S12 

void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8 mode);  	//按键扫描函数					    
#endif
