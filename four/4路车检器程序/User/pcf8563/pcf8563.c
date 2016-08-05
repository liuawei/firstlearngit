#include "pcf8563.h"
#include "sys.h"
#include "mbi5025.h"
#include "24cxx.h"
#include "math.h"
#include "gps.h"
#include "bsp_usart2_485.h"
#include "bsp_usart1.h"
#include "bsp_led.h"
u8 PCF8563_Time[7]={00,34,21,21,3,7,15};//秒 分 时 日 星期  月  年
void PCF8563_Init(void)
{
IIC_Init_pcf8563(); 
PCF8563_Time[0] = ((PCF8563_Time[0]/10) << 4) | (PCF8563_Time[0]%10); 
PCF8563_Time[1] = ((PCF8563_Time[1]/10) << 4) | (PCF8563_Time[1]%10); 
PCF8563_Time[2] = ((PCF8563_Time[2]/10) << 4) | (PCF8563_Time[2]%10); 
PCF8563_Time[3] = ((PCF8563_Time[3]/10) << 4) | (PCF8563_Time[3]%10); 
PCF8563_Time[4] = ((PCF8563_Time[4]/10 << 4)) | (PCF8563_Time[4]%10); 
PCF8563_Time[5] = ((PCF8563_Time[5]/10 << 4)) | (PCF8563_Time[5]%10); 
PCF8563_Time[6] = ((PCF8563_Time[6]/10 << 4)) | (PCF8563_Time[6]%10); 
PCF8563_CLKOUT_1s();
PCF8563_WriteAdress(0x00, 0x20); //??RTC source clock	
// PCF8563_WriteAdress(0x02, PCF8563_Time[0]); 
// PCF8563_WriteAdress(0x03, PCF8563_Time[1]);
// PCF8563_WriteAdress(0x04, PCF8563_Time[2]);
// PCF8563_WriteAdress(0x05, PCF8563_Time[3]);
// PCF8563_WriteAdress(0x06, PCF8563_Time[4]);
// PCF8563_WriteAdress(0x07, PCF8563_Time[5]);
// PCF8563_WriteAdress(0x08, PCF8563_Time[6]);
PCF8563_WriteAdress(0x00, 0x00); //Enable RTC sorce clock
}
void Gps_W_Pcf8563(void)
{
PCF8563_WriteAdress(0x00, 0x20);
g_timer[3] = ((g_timer[3]/10) << 4) | (g_timer[3]%10); 
g_timer[4] = ((g_timer[4]/10 << 4)) | (g_timer[4]%10); 
g_timer[5] = ((g_timer[5]/10 << 4)) | (g_timer[5]%10); 
PCF8563_WriteAdress(0x02, g_timer[5]); 
PCF8563_WriteAdress(0x03, g_timer[4]);
PCF8563_WriteAdress(0x04, g_timer[3]);
PCF8563_WriteAdress(0x00, 0x00);
}
u8 PCF8563_ReaDAdress(u8 Adress)
{
u8 ReadData;
IIC_Start_pcf8563();
IIC_WriteByte(0xa2);
IIC_WaitAck();
IIC_WriteByte(Adress);
IIC_WaitAck();
IIC_Start_pcf8563();
IIC_WriteByte(0xa3);
IIC_WaitAck();
ReadData = IIC_ReadByte();
IIC_Stop_pcf8563();
return ReadData;
}
void PCF8563_WriteAdress(u8 Adress,u8 DataTX) //?
{IIC_Start_pcf8563();
IIC_WriteByte(0xa2);
IIC_WaitAck();
IIC_WriteByte(Adress);
IIC_WaitAck();
IIC_WriteByte(DataTX);
IIC_WaitAck();
IIC_Stop_pcf8563();
}
void PCF8563_ReadTimes(void)
{
IIC_Start_pcf8563();
IIC_WriteByte(0xa2);
IIC_WaitAck();
IIC_WriteByte(0x02);
IIC_WaitAck();
IIC_Start_pcf8563();
IIC_WriteByte(0xa3);
IIC_WaitAck();
PCF8563_Time[0] = IIC_ReadByte()&0x7f;
I2C_Ack();
PCF8563_Time[1] = IIC_ReadByte()&0x7f;
I2C_Ack();
PCF8563_Time[2] = IIC_ReadByte()&0x3f;
I2C_Ack();
PCF8563_Time[3] = IIC_ReadByte()&0x3f;
I2C_Ack();
PCF8563_Time[4] = IIC_ReadByte()&0x07;
I2C_Ack();
PCF8563_Time[5] = IIC_ReadByte()&0x1f;
I2C_Ack();
PCF8563_Time[6] = IIC_ReadByte();
I2C_NoAck();
IIC_Stop_pcf8563();
PCF8563_Time[0] = ((PCF8563_Time[0]&0xf0)>>4)*10 + (PCF8563_Time[0]&0x0f); 
PCF8563_Time[1] = ((PCF8563_Time[1]&0xf0)>>4)*10 + (PCF8563_Time[1]&0x0f); 
PCF8563_Time[2] = ((PCF8563_Time[2]&0xf0)>>4)*10 + (PCF8563_Time[2]&0x0f); 
PCF8563_Time[3] = ((PCF8563_Time[3]&0xf0)>>4)*10 + (PCF8563_Time[3]&0x0f); 
//PCF8563_Time[4] = ((PCF8563_Time[4]&0xf0)>>4)*10 + (PCF8563_Time[4]&0x0f); 
PCF8563_Time[5] = ((PCF8563_Time[5]&0xf0)>>4)*10 + (PCF8563_Time[5]&0x0f); 
PCF8563_Time[6] = ((PCF8563_Time[6]&0xf0)>>4)*10 + (PCF8563_Time[6]&0x0f);
}

void PCF8563_CLKOUT_1s(void)
{
PCF8563_WriteAdress(0x01, 0);
PCF8563_WriteAdress(0x0e, 0);
PCF8563_WriteAdress(0x0d, 0x83);
}

void EXTI_Configuration(void) 
{ 
  EXTI_InitTypeDef EXTI_InitStructure;
  EXTI_ClearITPendingBit(EXTI_Line4);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	
  EXTI_InitStructure.EXTI_Line = EXTI_Line4;  	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);  
	EXTI_GenerateSWInterrupt(EXTI_Line4);         
} 
void GPIO_Configuration(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}


void WatchDog_Init(u8 prer, u16 reld)
{
   IWDG->KR=0x5555; //允许访问PR和RLR寄存器
   IWDG->PR=prer;//设置分频
   IWDG->RLR=reld; //设定计数器初值
   IWDG->KR=0xaaaa;//初次装初值
   IWDG->KR=0xcccc;//启动看门狗
}

void WatchDog_Feed(void)
{
 IWDG->KR=0xaaaa;
}
