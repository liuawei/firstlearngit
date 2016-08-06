#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//SPI ��������	   
//////////////////////////////////////////////////////////////////////////////////


#define MOSI PAout(7)
#define SCLK PAout(5)
#define MISO PAin(6)
#define MISO_IN() {GPIOA->CRL &= 0xff0fffff;GPIOA->CRL |= 0x00800000;}	//����Ϊ����ģʽ���߿�©ģʽ�����������
#define FRAM_CS  PAout(4)	//
// SPI�����ٶ�����  
#define SPI_SPEED_2   		0
#define SPI_SPEED_4   		1
#define SPI_SPEED_8   		2
#define SPI_SPEED_16  		3
#define SPI_SPEED_32 		4
#define SPI_SPEED_64 		5
#define SPI_SPEED_128 		6
#define SPI_SPEED_256 		7
						  	    													  
void SPIx_Init(void);			 //��ʼ��SPI��
void SPI1_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPIx_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
void wrbytes(u16 addr1,u8 *addr2,u16 length);
void rdbytes(u16 addr1,u8 *addr2,u16 length);
void FE_OUT(unsigned char WriteData);
unsigned char FE_IN(void);
#endif

