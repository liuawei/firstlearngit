/******************** (C) 通信型交通信号灯控制机代码 ********************
* File Name          : test.c
* Author             : JingTai
* Version            : V0.1.7
* Date               : 26/07/2015
* Description        : This file contains all the functions prototypes for the
*                      CRC firmware library.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#include "sys.h"

/*************************************************
说明：控制机主函数
入参：无
返回：无
**************************************************/	 
int main(void)
{		
	u8 bus1;
 	u8 rs485buf1[16] = {0,0,0,0,0,0};
	Stm32_Clock_Init(9);	//系统时钟设置
	delay_init(72);			//延时初始化
	JTAG_Set(SWD_ENABLE);	//初始化调试接口		
	uart_init(72,57600); 	//串口2初始化	
	RS485_Init();			//485初始化
	TIM3_Init(9999,7199);	//1秒中断，用于倒计时 	
	IWDG_Init(6,2500);		//独立看门狗定时16秒
	LED_Init();	   		 	//LED初始化

	IIC_GPIO_Init();
	SPIx_Init();
	

//	Write_config();
	
	Read_config();
	DS1307_Time_Init(&TimeValue);
	

	IWDG_Feed();			//喂独立看门狗
//	while(Node_test());				//节点检测					

	Time_ana_start();				//时段判断				
	

	
	for(;;)
	{			
		Calibration_time();				//校准时间 
		NET_Receive_Data();				//网口数据读取
	 	RS485_Receive_Data(1,rs485buf1,&bus1);//接收到有数据
		if(rs485buf1[0])
		{
			bus1=0;
//		printf("Coil--%x--%x--%x--%x--\n",rs485buf1[0],rs485buf1[1],rs485buf1[2],rs485buf1[3]);
			Bus_Data(rs485buf1);
			
			memset(rs485buf1,0,20);		
 		}	
		
	
		
	}
}













