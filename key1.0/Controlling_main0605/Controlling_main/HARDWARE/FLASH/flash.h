#ifndef __FLASH_H
#define __FLASH_H			    
#include "sys.h" 
#define WREN  0X06
#define WRDI  0X04
#define RDSR  0X05
#define WRSR  0X01
#define READ  0X03
#define FSTRD 0X0B
#define WRITE 0X02
#define SLEEP 0XB9
#define RDID  0X9F
#define SNR   0XC3


#define 	BASIC_ADDR   	0+6       	//�����������ݵ�ַ
#define 	LED_CAR_ADDR 		50+6			//�������ݵ�ַ
#define 	LED_MAN_ADDR 		100+6			//�������ݵ�ַ
#define 	PHASE_ADDR      150+6			//��λ���浹����ʱ���ݵ�ַ
#define 	TIME_D_ADDR     200+6		//ʱ��������ݵ�ַ
#define		STAGE_ADDR		500+6			//�׶���λ���ݵ�ַ
#define		PHASE_MARK_ADDR		650+6			//��λ��ʶ���ݵ�ַ
#define		PLAN_ADDR		750+6			//�������ݵ�ַ
#define		File_Name_ADDR		1110+6		//������
#define		WEEK_ADDR		1180+6		//��������
#define		LED_CAR_Name		1300+6		//��������
#define		LED_MAN_Name		1350+6		//�˵�����
#define		STAGE_Name		1400+6		//��λ�׶�����
#define		LOG_ADDR		10000		//��־��ַ


extern u32 paln_no_addr[32];    				//�����ŵ�ַ	


void SPI_Flash_Init(void);
u8 SPIx_ReadWriteByte(u8 TxData);
void FramWriteInduce(u32 address,u32 number,u8 *p);
void FramReadInduce(u32 address,u32 number,u8 *p);
void FramWriteByte(u32 address,u8 da);
u8 FramReadByte(u32 address);
void FramWrite16Byte(u32 address,u16 da);
u16 FramRead16Byte(u32 address);
void Log_Write(u8 filg);
#endif














