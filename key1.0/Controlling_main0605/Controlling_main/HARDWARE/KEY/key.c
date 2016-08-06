#include "key.h"
#include "delay.h"
#include "led.h"
#include "wdg.h"
#include "sim900a.h"
#include "rs485.h"	
#include "mbi5025.h"
//////////////////////////////////////////////////////////////////////////////////	 
//�������� ��������		   
//********************************************************************************
								    
//������ʼ������
void KEY_Init(void)
{
	u8 i;
	RCC->APB2ENR|=1<<5;    //ʹ��PORTDʱ��	
	GPIOD->CRH&=0XF0F0F0F0;
	GPIOD->CRH|=0X03030303;	//PD8 10 12 14�������
	GPIOD->ODR|=1<<8;     	//PD8 10 12 14����� 	
	GPIOD->ODR|=1<<10;     	//PD8 10 12 14����� 
	GPIOD->ODR|=1<<12;     	//PD8 10 12 14����� 
	GPIOD->ODR|=1<<14;     	//PD8 10 12 14����� 
	delay_ms(10);
	if(KEY_AH==0)
	{
		delay_ms(10);
		if(KEY_AH==0)
		{
			for(;;)
			{
				for(i=0;i<99;i++)
				{
					if(KEY_AH==1)
					{
						delay_ms(10);
						if(KEY_AH==1)
						{
							LED_ceshi(0x00);
							return;				//�˳��ֶ�ģʽ
						}		
					}
					delay_ms(10);
				}
				SLED_control();
			}
		}
	}
} 
//����������
void KEY_Scan(void)
{
	u8 temp,KEY_up=3; //��־
	if(KEY_AH==0)
	{
		delay_ms(10);
		if(KEY_AH==0)
		{
			TIM2->CR1&=~(1<<0);  //�رն�ʱ��2
			TIM3->CR1&=~(1<<0);  //�رն�ʱ��3
			RS485_TX_EN = 1;
			delay_us(500);
			for(temp=0;temp<4;temp++)//interNum
			{	
				RS485_Write_Data(temp,&KEY_up,1);//ǿ�ƹص�
			}
			RS485_TX_EN = 0;
			LED0=0;
			temp=0;
			KEY_up=0;
			for(;;)
			{
				if(KEY_AH==1)
				{
					delay_ms(10);
					if(KEY_AH==1)
					{
						LED_ceshi(0x00);
						LED_control_time=0;	 //
						LED_control_phase=0;
						TIM3->CNT|=0x00;
						TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
						return;				//�˳��ֶ�ģʽ
					}		
				}
				else
				{
					delay_ms(10);
					if(KEY_G==0||KEY_R==0)
					{
						delay_ms(10);
						if(KEY_R==0)
							KEY_up = 1;
						else
							if(KEY_G==0)
								KEY_up = 2;
					}else
					{
						delay_ms(10);
						if(KEY_G==1||KEY_R==1)
						KEY_up = 3;
					}
					if(KEY_up!=temp)
					{
						temp = KEY_up;
						LED_hand(temp);
					}
				}
				//sim900a_sms_read_test();//���Ŵ���
				LED_USART_Flash();		//���ڽ��յ���Ч���ݣ���ת�浽FLASH��
				IWDG_Feed();			//ι�������Ź�
			}
		}
	}
}



void LED_hand(u8 num)
{
	u8 pBuffer[2];  //���ݻ���
	u8 tt[5];								 
	RS485_TX_EN = 1;//485����ģʽ
	delay_us(500);
	pBuffer[0] = 6;//ָ��
	switch(num)
	{
		case 1:	pBuffer[1] = 1;	//��
				RS485_Write_Data(0,pBuffer,2);
				RS485_Write_Data(2,pBuffer,2);
				pBuffer[1] = 2;	//��
				RS485_Write_Data(1,pBuffer,2);
				RS485_Write_Data(3,pBuffer,2);
				tt[0] = 0x55;tt[1] = 0xaa;tt[2] = 0x55;tt[3] = 0xaa;tt[4] = 0x99;
				break;
		case 2:	pBuffer[1] = 2;	//��
				RS485_Write_Data(0,pBuffer,2);
				RS485_Write_Data(2,pBuffer,2);
				pBuffer[1] = 1;	//��
				RS485_Write_Data(1,pBuffer,2);
				RS485_Write_Data(3,pBuffer,2);
				tt[0] = 0xaa;tt[1] = 0x55;tt[2] = 0xaa;tt[3] = 0x55;tt[4] = 0x66;
				break;
		case 3:	pBuffer[1] = 3;	//��
				RS485_Write_Data(0,pBuffer,2);
				RS485_Write_Data(2,pBuffer,2);
				pBuffer[1] = 1;	//��
				RS485_Write_Data(1,pBuffer,2);
				RS485_Write_Data(3,pBuffer,2);
				tt[0] = 0xff;tt[1] = 0x55;tt[2] = 0xff;tt[3] = 0x55;tt[4] = 0x55;
				break;
		default:break;
	}
	LED_display(tt);//LED���
	RS485_TX_EN = 0;//485����ģʽ
}































