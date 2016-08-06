/******************** (C) ͨ���ͽ�ͨ�źŵƿ��ƻ����� ********************
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
˵�������ƻ�������
��Σ���
���أ���
**************************************************/	 
int main(void)
{		
	u8 bus1;
 	u8 rs485buf1[16] = {0,0,0,0,0,0};
	Stm32_Clock_Init(9);	//ϵͳʱ������
	delay_init(72);			//��ʱ��ʼ��
	JTAG_Set(SWD_ENABLE);	//��ʼ�����Խӿ�		
	uart_init(72,57600); 	//����2��ʼ��	
	RS485_Init();			//485��ʼ��
	TIM3_Init(9999,7199);	//1���жϣ����ڵ���ʱ 	
	IWDG_Init(6,2500);		//�������Ź���ʱ16��
	LED_Init();	   		 	//LED��ʼ��

	IIC_GPIO_Init();
	SPIx_Init();
	

//	Write_config();
	
	Read_config();
	DS1307_Time_Init(&TimeValue);
	

	IWDG_Feed();			//ι�������Ź�
//	while(Node_test());				//�ڵ���					

	Time_ana_start();				//ʱ���ж�				
	

	
	for(;;)
	{			
		Calibration_time();				//У׼ʱ�� 
		NET_Receive_Data();				//�������ݶ�ȡ
	 	RS485_Receive_Data(1,rs485buf1,&bus1);//���յ�������
		if(rs485buf1[0])
		{
			bus1=0;
//		printf("Coil--%x--%x--%x--%x--\n",rs485buf1[0],rs485buf1[1],rs485buf1[2],rs485buf1[3]);
			Bus_Data(rs485buf1);
			
			memset(rs485buf1,0,20);		
 		}	
		
	
		
	}
}













