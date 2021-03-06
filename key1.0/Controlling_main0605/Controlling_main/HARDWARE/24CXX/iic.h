/******************************************************************************
* @ File name --> iic.h
* @ Author    --> By@ Sam Chan
* @ Version   --> V1.0
* @ Date      --> 07 - 10 - 2012
* @ Brief     --> MCU模拟IIC通讯函数
* @           --> 要改变传输频率，请修改延时函数中的数值即可
*
* @ Copyright (C) 20**
* @ All rights reserved
*******************************************************************************
*
*                                  File Update
* @ Version   --> V1.1
* @ Author    --> By@ Sam Chan
* @ Date      --> 10 - 01 - 2013
* @ Revise    --> A、修正兼容夸平台移植
* @           --> B、修正接收数据时错位BUG
*
* @ Version   --> V1.1.1
* @ Author    --> By@ Sam Chan
* @ Date      --> 12 - 10 - 2013
* @ Revise    --> 增加是否使用延时函数调整通讯频率宏定义 
*
* @ Version   --> V1.1.2
* @ Author    --> By@ Sam Chan
* @ Date      --> 05 - 01 - 2014
* @ Revise    --> 简化代码量和优化风格，简化初始化控制端口代码，尽量在头文件中修改宏定义，方便移植
*
******************************************************************************/

#ifndef _iic_h_ 
#define _iic_h_

/******************************************************************************
                                 外部函数头文件                        
******************************************************************************/
#include "sys.h"
#include "delay.h"
//#include "STM32_config.h"

/******************************************************************************
                             对于低速晶振的支持
                     是否使用延时函数进行调整通讯频率
******************************************************************************/

#define _USER_DELAY_CLK					1	//定义了则使用延时调整通讯频率
											//0：不使用延时函数调整通讯频率，对于低速MCU时候用
											//1：使用延时函数调整通讯频率，对于高速MCU时候用

/******************************************************************************
                                定义模拟接口
                         所接的IO不一样请修改这里
           设置IO的方向请修改这里，对于不是具有准双向IO的MCU
******************************************************************************/

// #define IIC_SDA_Init          RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_1,GPIO_Speed_2MHz,GPIO_Mode_Out_PP
// #define IIC_SCL_Init          RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_2,GPIO_Speed_2MHz,GPIO_Mode_Out_PP

//#define IIC_SCL					PAout(6)
//#define IIC_SDA					PAout(5)	//IIC发送数据用
//#define IN_SDA					PAin(5)	//IIC读取数据用

////改变SDA所接的IO输入/输出

//#define IIC_SDA_IN()			{GPIOA->CRL &= 0xff0fffff;GPIOA->CRL |= 0x00800000;}	//设置为输入模式或者开漏模式（外加上拉）
//#define IIC_SDA_OUT()			{GPIOA->CRL &= 0xff0fffff;GPIOA->CRL |= 0x00300000;}	//设置成准双向或者推挽输出，最大速度50MHz
#define IIC_INT					PCout(4)
#define IIC_SCL					PBout(0)
#define IIC_SDA					PCout(5)	//IIC发送数据用
#define IN_SDA					PCin(5)	//IIC读取数据用

//改变SDA所接的IO输入/输出

#define IIC_SDA_IN()			{GPIOC->CRL &= 0xff0fffff;GPIOC->CRL |= 0x00800000;}	//设置为输入模式或者开漏模式（外加上拉）
#define IIC_SDA_OUT()			{GPIOC->CRL &= 0xff0fffff;GPIOC->CRL |= 0x00300000;}	//设置成准双向或者推挽输出，最大速度50MHz
/******************************************************************************
                               通讯频率延时函数
                    需要调整通讯频率的请修改此函数值即可
******************************************************************************/

#if	_USER_DELAY_CLK==1	//定义了则使用

	#define IIC_Delay()                 delay_us(2) //要改变请修改delay_us()中的数值即可

#endif

/******************************************************************************
                                 外部功能函数
******************************************************************************/

void IIC_GPIO_Init(void);	//GPIO初始化

void IIC_Start(void);	//IIC启动

void IIC_Stop(void);	//IIC停止

void IIC_Ack(u8 a);	//主机向从机发送应答信号

u8   IIC_Write_Byte(u8 dat);	//向IIC总线发送一个字节数据

u8   IIC_Read_Byte(void);	//从IIC总线上读取一个字节数据



#endif  /* end iic.h */




