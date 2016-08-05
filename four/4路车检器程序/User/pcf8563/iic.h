#ifndef _IIC_H_
#define _IIC_H_
#include "stm32f10x.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/*#define I2C_Speed 100000
#define I2C1_SLAVE_ADDRESS7 0xA0
#define I2C_PageSize 256 */
//SCL PA5
//SDA PA6
#define SCL_H() GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define SCL_L() GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define SDA_H() GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define SDA_L() GPIO_ResetBits(GPIOA,GPIO_Pin_6)
#define Read_SDA() GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
u8 IIC_ReadByte(void);
void IIC_WriteByte(u8 byte);
void IIC_WaitAck(void);
void IIC_Stop_pcf8563(void);
void IIC_Start_pcf8563(void);
void IIC_Init_pcf8563(void);
void I2C_Ack(void);
void I2C_NoAck(void);
void nus(u16 i);
#endif

