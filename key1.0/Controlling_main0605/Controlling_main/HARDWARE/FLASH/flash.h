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


#define 	BASIC_ADDR   	0+6       	//基本配置数据地址
#define 	LED_CAR_ADDR 		50+6			//灯序数据地址
#define 	LED_MAN_ADDR 		100+6			//灯序数据地址
#define 	PHASE_ADDR      150+6			//相位跟随倒数计时数据地址
#define 	TIME_D_ADDR     200+6		//时间调度数据地址
#define		STAGE_ADDR		500+6			//阶段相位数据地址
#define		PHASE_MARK_ADDR		650+6			//相位标识数据地址
#define		PLAN_ADDR		750+6			//方案数据地址
#define		File_Name_ADDR		1110+6		//方案名
#define		WEEK_ADDR		1180+6		//日期类型
#define		LED_CAR_Name		1300+6		//车灯名称
#define		LED_MAN_Name		1350+6		//人灯名称
#define		STAGE_Name		1400+6		//相位阶段名称
#define		LOG_ADDR		10000		//日志地址


extern u32 paln_no_addr[32];    				//方案号地址	


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














