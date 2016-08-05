#include "myiic.h"
#include "iic.h"

#include "bsp_usart2_485.h"
//////////////////////////////////////////////////////////////////////////////////	 
//IIC ��������	   
////////////////////////////////////////////////////////////////////////////////// 	  

//��ʼ��IIC
void IIC_Init(void)
{					     
// 	RCC->APB2ENR|=1<<3;//��ʹ������IO PORTBʱ�� 							 
// 	GPIOB->CRH&=0X000FFFFF;//PB13,14,15 �������
// 	GPIOB->CRH|=0X33300000;	   
// 	GPIOB->ODR|=8<<13;//PB13,14,15 �����
  	GPIO_InitTypeDef GPIO_InitStructure;
		/*����GPIOB��GPIOF������ʱ��*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
		/*ѡ��Ҫ���Ƶ�GPIOB����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;	
		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	  GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	nus(500);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	nus(500);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	nus(500);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	nus(500);						   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;
	nus(500);	   
	IIC_SCL=1;
	nus(500);
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	nus(500);
	IIC_SCL=1;
	nus(5);
	
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	nus(500);
	IIC_SCL=1;
	nus(500);
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
		nus(500);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		nus(500);
		IIC_SCL=0;	
		nus(500);
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
        nus(1000);
		    IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		nus(1000); 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}



























