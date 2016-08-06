#include "mbi5025.h"
#include "delay.h"
#include "usart.h"	
#include "timer.h"
#include "sys.h"
#include "key.h"
//ALIENTEK Mini STM32�����巶������2
//��������ʵ��		   
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
 int main(void)
 {	
	u8 t;
//	u8 p[20]={1,2,3,4,5,6,7,8,9};
	delay_init();	    	 //��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����жϷ���
	RS485_Init(9600);
	MBI5025_Init();
	MBI5025_LED(0X0000);
	KEY_Init();          	//��ʼ���밴�����ӵ�Ӳ���ӿ�
	TIM3_Int_Init(999,7199);
	TIM4_Int_Init(1999,7199);
	while(1)
	{
		
			if(KEY0==0)
			{	
				t=KEY_Scan(0);		//�õ���ֵ
				switch(t)
				{				 
					case S1_PRES:
						TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 
						Ent_Write_Data(0X19);
						RS_Flag=1;//�Զ�
						break;
					case S2_PRES:
						TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
						Ent_Write_Data(0X1A);
						RS_Flag=2;//�ֶ�
						break;
					case S3_PRES:
						TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
						Ent_Write_Data(0X1B);
						RS_Flag=3;//ȫ��				
					break;
					case S4_PRES:
						TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
						Ent_Write_Data(0X1C);
						RS_Flag=4;//����
						break;
					case S5_PRES:
						TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
//					RS485_Send_2bData(0xff,0x00);
						RS_Flag=5;

					break;
					case S6_PRES:
						TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
//					RS485_Send_2bData(0xff,0x00);
						RS_Flag=6;
					break;
					case S7_PRES:
					TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
//					RS485_Send_2bData(0xff,0x00);
					RS_Flag=7;
					break;
					case S8_PRES:
					TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
//					RS485_Send_2bData(0xff,0x00);
					RS_Flag=8;
					break;
					case S9_PRES:
					TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);// flag=0x0100;
//					RS485_Send_2bData(0xff,0x00);
					RS_Flag=9;
					break;
					case S10_PRES:
					TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
//					RS485_Send_2bData(0xff,0x00);
					RS_Flag=10;
					break;
					case S11_PRES:
					TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 
//					RS485_Send_2bData(0xff,0x00);
					RS_Flag=11;
					break;
					case S12_PRES:
					TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
//					RS485_Send_2bData(0xff,0x00);
					RS_Flag=12;
					break;
					default:
					break;
			} 

//				if(RX_Flag==0)
//				{
//					RS485_ReSend_2bData(RS_Flag);
//					RS_Flag=0;
//				}
		}
		delay_ms(100);	
	}		
		
}
