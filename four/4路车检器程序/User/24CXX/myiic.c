#include "myiic.h"
#include "iic.h"

#include "bsp_usart2_485.h"
//////////////////////////////////////////////////////////////////////////////////	 
//IIC 驱动函数	   
////////////////////////////////////////////////////////////////////////////////// 	  

//初始化IIC
void IIC_Init(void)
{					     
// 	RCC->APB2ENR|=1<<3;//先使能外设IO PORTB时钟 							 
// 	GPIOB->CRH&=0X000FFFFF;//PB13,14,15 推挽输出
// 	GPIOB->CRH|=0X33300000;	   
// 	GPIOB->ODR|=8<<13;//PB13,14,15 输出高
  	GPIO_InitTypeDef GPIO_InitStructure;
		/*开启GPIOB和GPIOF的外设时钟*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
		/*选择要控制的GPIOB引脚*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;	
		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	  GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	nus(500);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	nus(500);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	nus(500);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	nus(500);						   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
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
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	nus(500);
	IIC_SCL=1;
	nus(5);
	
}
//不产生ACK应答		    
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	  SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		nus(500);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		nus(500);
		IIC_SCL=0;	
		nus(500);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
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
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}



























