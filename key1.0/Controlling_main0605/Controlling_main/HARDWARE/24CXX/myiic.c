#include "sys.h"
#include "usart.h"		
#include "delay.h"
#include "wdg.h"	
#include "timer.h"
#include "rs485.h"	
#include "stmflash.h"
#include "rtc.h" 
#include "malloc.h"
#include "led.h"
#include "myiic.h"
//////////////////////////////////////////////////////////////////////////////////	 
//IIC ��������	   
////////////////////////////////////////////////////////////////////////////////// 	  

//��ʼ��IIC
void IIC_Init(void)
{					     
	RCC->APB2ENR|=1<<2;//��ʹ������IO PORTDʱ�� 							 
	GPIOA->CRL&=0XF000FFFF;//PD8,9 �������
	GPIOA->CRL|=0X03330000;	   
	GPIOA->ODR|=1<<4;      //PD8,9 �����
	IIC_INT = 1;
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  
	delay_us(1);	
	IIC_SCL=1;
	delay_us(1);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(1);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	delay_us(1);
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(1);
	IIC_SCL=1; 
	delay_us(1);
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_OUT();//sda�����
	IIC_SDA=1;delay_us(1);
	SDA_IN();      //SDA����Ϊ����  	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop(); printf("ʧ��");
			return 1;
		}
	}
	printf("�ɹ�");
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(1);
				IIC_SCL=1;
				delay_us(5);
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}

void DS1339_Write(u8 Address,u8 Data)  
{
  IIC_Start();
  IIC_Send_Byte(0xD0);
  IIC_Wait_Ack();
  IIC_Send_Byte(Address);
  IIC_Wait_Ack();
  IIC_Send_Byte(Data);
  IIC_Wait_Ack();
  IIC_Stop();
}


u8 DS1339_Read(u8 Address)
{
  u8 TmpData;
  IIC_Start();
  IIC_Send_Byte(0xD0);
  IIC_Wait_Ack();
  IIC_Send_Byte(Address);
  IIC_Wait_Ack();
  IIC_Start();
  IIC_Send_Byte(0xD1);
  IIC_Ack();
  TmpData=IIC_Read_Byte(0);
  IIC_Stop();
  return(TmpData);
}

void DS1339_Init(void)
{
	
	IIC_Init();
//	DS1339_Write(0x0f,0x00);
//	DS1339_Write(0x0e,0x00);
//	DS1339_Write(0x10,0x00);
}	
























