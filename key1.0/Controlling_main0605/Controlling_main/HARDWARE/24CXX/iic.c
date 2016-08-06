/******************************************************************************
* @ File name --> iic.c
* @ Author    --> By@ Sam Chan
* @ Version   --> V1.0
* @ Date      --> 07 - 10 - 2012
* @ Brief     --> MCUģ��IICͨѶ����
* @           --> Ҫ�ı䴫��Ƶ�ʣ����޸���ʱ�����е���ֵ����
*
* @ Copyright (C) 20**
* @ All rights reserved
*******************************************************************************
*
*                                  File Update
* @ Version   --> V1.1
* @ Author    --> By@ Sam Chan
* @ Date      --> 10 - 01 - 2013
* @ Revise    --> A���������ݿ�ƽ̨��ֲ
* @           --> B��������������ʱ��λBUG
*
* @ Version   --> V1.1.1
* @ Author    --> By@ Sam Chan
* @ Date      --> 12 - 10 - 2013
* @ Revise    --> �����Ƿ�ʹ����ʱ��������ͨѶƵ�ʺ궨�� 
*
* @ Version   --> V1.1.2
* @ Author    --> By@ Sam Chan
* @ Date      --> 05 - 01 - 2014
* @ Revise    --> �򻯴��������Ż���񣬼򻯳�ʼ�����ƶ˿ڴ��룬������ͷ�ļ����޸ĺ궨�壬������ֲ
*
******************************************************************************/

#include "iic.h"
#include "usart.h"
/******************************************************************************
* Function Name --> GPIO��ʼ��
* Description   --> STM32ƽ̨������ƽ̨���޸Ļ���ע�͵�
* Input         --> none
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void IIC_GPIO_Init(void)
{
 	RCC->APB2ENR|=1<<3;//��ʹ������IO PORTAʱ�� 		
	RCC->APB2ENR|=1<<4;//��ʹ������IO PORTAʱ�� 	
	GPIOC->CRL&=0XFF00FFFF;//PB1/11 �������
	GPIOC->CRL|=0X00330000;	   
//	GPIOC->ODR|=3<<4;     //PB10,11 �����
	GPIOB->CRL&=0XFFFFFFF0;//PB1/11 �������
	GPIOB->CRL|=0X00000003;	   
//	GPIOB->ODR|=1<<0;     //PB10,11 �����
	IIC_SDA = 1;  //��IIC���߿���
	IIC_SCL = 1;
	IIC_INT = 1;
}

/******************************************************************************
* Function Name --> IIC����
* Description   --> SCL�ߵ�ƽ�ڼ䣬SDA�ɸߵ�ƽͻ�䵽�͵�ƽʱ��������
*                   SCL: __________
*                                  \__________
*                   SDA: _____
*                             \_______________
* Input         --> none
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void IIC_Start(void)
{
	IIC_SDA_OUT();	//���ó����

	IIC_SDA = 1;	//ΪSDA�½�������׼��
	IIC_SCL = 1;	//��SCL�ߵ�ƽʱ��SDAΪ�½���ʱ����������
	
#if _USER_DELAY_CLK==1  /* ��������ʹ����ʱ�������ı�ͨѶƵ�� */
	
	IIC_Delay();
	IIC_SDA = 0;	//ͻ�䣬��������
	IIC_Delay();
	IIC_SCL = 0;
	IIC_Delay();

#else  /* ����ʹ����ʱ�����ı�ͨѶƵ�� */
	
	IIC_SDA = 0;	//ͻ�䣬��������
	IIC_SCL = 0;

#endif  /* end __USER_DELAY_CLK */
}
/******************************************************************************
* Function Name --> IICֹͣ
* Description   --> SCL�ߵ�ƽ�ڼ䣬SDA�ɵ͵�ƽͻ�䵽�ߵ�ƽʱֹͣ����
*                   SCL: ____________________
*                                  __________
*                   SDA: _________/
* Input         --> none
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void IIC_Stop(void)
{
	IIC_SDA_OUT();	//���ó����

	IIC_SDA = 0;	//ΪSDA������׼��
	
#if _USER_DELAY_CLK==1  /* ��������ʹ����ʱ�������ı�ͨѶƵ�� */

	IIC_Delay();
	IIC_SCL = 1;	//��SCL�ߵ�ƽʱ��SDAΪ������ʱ������ֹͣ
	IIC_Delay();
	IIC_SDA = 1;	//ͻ�䣬����ֹͣ
	IIC_Delay();

#else  /* ����ʹ����ʱ�����ı�ͨѶƵ�� */

	IIC_SCL = 1;	//��SCL�ߵ�ƽʱ��SDAΪ������ʱ������ֹͣ
	IIC_SDA = 1;	//ͻ�䣬����ֹͣ

#endif  /* end __USER_DELAY_CLK */
}
/******************************************************************************
* Function Name --> ������ӻ�����Ӧ���ź�
* Description   --> none
* Input         --> a��Ӧ���ź�
*                      0��Ӧ���ź�
*                      1����Ӧ���ź�
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void IIC_Ack(u8 a)
{
	IIC_SDA_OUT();	//���ó����

	if(a)	IIC_SDA = 1;	//����Ӧ���źŵ�ƽ
	else	IIC_SDA = 0;
	
#if _USER_DELAY_CLK==1  /* ��������ʹ����ʱ�������ı�ͨѶƵ�� */

	IIC_Delay();
	IIC_SCL = 1;	//ΪSCL�½���׼��
	IIC_Delay();
	IIC_SCL = 0;	//ͻ�䣬��Ӧ���źŷ��͹�ȥ
	IIC_Delay();

#else  /* ����ʹ����ʱ�����ı�ͨѶƵ�� */

	IIC_SCL = 1;	//ΪSCL�½���׼��
	IIC_SCL = 0;	//ͻ�䣬��Ӧ���źŷ��͹�ȥ

#endif  /* end __USER_DELAY_CLK */
}
/******************************************************************************
* Function Name --> ��IIC���߷���һ���ֽ�����
* Description   --> none
* Input         --> dat��Ҫ���͵�����
* Output        --> none
* Reaturn       --> ack������Ӧ���ź�
******************************************************************************/
u8 IIC_Write_Byte(u8 dat)
{
	u8 i;
	u8 iic_ack=0;	//iicӦ���־

	IIC_SDA_OUT();	//���ó����

	for(i = 0;i < 8;i++)
	{
		if(dat & 0x80)	IIC_SDA = 1;	//�жϷ���λ���ȷ��͸�λ
		else	IIC_SDA = 0;

#if _USER_DELAY_CLK==1  /* ��������ʹ����ʱ�������ı�ͨѶƵ�� */
			
		IIC_Delay();
		IIC_SCL = 1;	//ΪSCL�½���׼��
		IIC_Delay();
		IIC_SCL = 0;	//ͻ�䣬������λ���͹�ȥ
		dat<<=1;	//��������һλ
	}	//�ֽڷ�����ɣ���ʼ����Ӧ���ź�

	IIC_SDA = 1;	//�ͷ�������

	IIC_SDA_IN();	//���ó�����

	IIC_Delay();
	IIC_SCL = 1;	//ΪSCL�½���׼��
	IIC_Delay();

#else  /* ����ʹ����ʱ�����ı�ͨѶƵ�� */

		IIC_SCL = 1;	//ΪSCL�½���׼��
		IIC_SCL = 0;	//ͻ�䣬������λ���͹�ȥ
		dat<<=1;	//��������һλ
	}	//�ֽڷ�����ɣ���ʼ����Ӧ���ź�

	IIC_SDA = 1;	//�ͷ�������

	IIC_SDA_IN();	//���ó�����

	IIC_SCL = 1;	//ΪSCL�½���׼��

#endif  /* end __USER_DELAY_CLK */
	
	iic_ack |= IN_SDA;	//����Ӧ��λ
	IIC_SCL = 0;
	//printf("ds1339ack=%d",iic_ack);
	return iic_ack;	//����Ӧ���ź�
	
}
/******************************************************************************
* Function Name --> ��IIC�����϶�ȡһ���ֽ�����
* Description   --> none
* Input         --> none
* Output        --> none
* Reaturn       --> x����ȡ��������
******************************************************************************/
u8 IIC_Read_Byte(void)
{
	u8 i;
	u8 x=0;

	IIC_SDA = 1;	//������������Ϊ�ߵ�ƽ

	IIC_SDA_IN();	//���ó�����

	for(i = 0;i < 8;i++)
	{
		x <<= 1;	//�������ݣ���λ��ǰ

#if _USER_DELAY_CLK==1  /* ��������ʹ����ʱ�������ı�ͨѶƵ�� */

		IIC_Delay();
		IIC_SCL = 1;	//ͻ��
		IIC_Delay();
		
		if(IN_SDA)	x |= 0x01;	//�յ��ߵ�ƽ

		IIC_SCL = 0;
		IIC_Delay();
	}	//���ݽ������

#else  /* ����ʹ����ʱ�����ı�ͨѶƵ�� */

		IIC_SCL = 1;	//ͻ��
		
		if(IN_SDA)	x |= 0x01;	//�յ��ߵ�ƽ

		IIC_SCL = 0;
	}	//���ݽ������

#endif  /* end __USER_DELAY_CLK */

	IIC_SCL = 0;

	return x;	//���ض�ȡ��������
}





