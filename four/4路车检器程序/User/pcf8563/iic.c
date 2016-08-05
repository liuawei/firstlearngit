#include "iic.h"
#include "bsp_usart2_485.h"
/**/
void IIC_Init_pcf8563(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
/* Configure I2C1 pins: SCL and SDA */
RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //????,????????????????IO???
GPIO_Init(GPIOA, &GPIO_InitStructure);
SCL_H(); //??
SDA_H();
}
void nus(u16 i)
{u16 j;

	for(j=0;j<i;j++)
         ;
}
void IIC_Start_pcf8563(void)

{
SDA_H();
SCL_H();
nus(2000);
SDA_L();
nus(2000);
SDA_L();
nus(2000);
}
void IIC_Stop_pcf8563(void)
{
SCL_L(); //1
nus(2000);// 2
SDA_L(); // 3. 1,2,3??????? delay_nus(20);
SCL_H();
nus(2000);
SDA_H();
nus(2000);
}

void IIC_WaitAck(void)
{
u16 k;
SCL_L();
SDA_H();
nus(2000);
SCL_H();
k = 0;
while((Read_SDA()!= 0) && (k < 60000))k++;
nus(2000);
SCL_L();
nus(2000);
}

void IIC_WriteByte(u8 byte)
{
u8 i = 0;
for(i = 0; i < 8; i++)
{
SCL_L();
nus(2000);
if(byte & 0x80)
{
SDA_H();
}
else
{
SDA_L();
}
nus(2000);
SCL_H();
nus(2000);
byte<<=1;
}
SCL_L();
nus(2000);
}
u8 IIC_ReadByte(void)
{
u8 i,ReadByte;
SDA_H();
for(i = 0; i < 8; i++)
{
ReadByte <<= 1;
SCL_L();
nus(2000);
SCL_H();
nus(2000);
if(Read_SDA())
{
ReadByte |= 0x01;
}
else
{
ReadByte &= ~(0x01);
}
}
return ReadByte;
}
void I2C_Ack(void)
{
SCL_L();
nus(2000);
SDA_L();
nus(2000);
SCL_H();
nus(2000);
SCL_L();
nus(2000);
}
void I2C_NoAck(void)
{
SCL_L();
nus(2000);
SDA_H();
nus(2000);
SCL_H();
nus(2000);
SCL_L();
nus(2000);
}
