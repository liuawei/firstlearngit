#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//SPI 驱动函数	   
//////////////////////////////////////////////////////////////////////////////////


#define MOSI PAout(7)
#define SCLK PAout(5)
#define MISO PAin(6)
#define MISO_IN() {GPIOA->CRL &= 0xff0fffff;GPIOA->CRL |= 0x00800000;}	//设置为输入模式或者开漏模式（外加上拉）
#define FRAM_CS  PAout(4)	//
// SPI总线速度设置  
#define SPI_SPEED_2   		0
#define SPI_SPEED_4   		1
#define SPI_SPEED_8   		2
#define SPI_SPEED_16  		3
#define SPI_SPEED_32 		4
#define SPI_SPEED_64 		5
#define SPI_SPEED_128 		6
#define SPI_SPEED_256 		7
						  	    													  
void SPIx_Init(void);			 //初始化SPI口
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI速度   
u8 SPIx_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
void wrbytes(u16 addr1,u8 *addr2,u16 length);
void rdbytes(u16 addr1,u8 *addr2,u16 length);
void FE_OUT(unsigned char WriteData);
unsigned char FE_IN(void);
#endif

