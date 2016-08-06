#include "sys.h"   

/********************************************************
//project：STM32F1  SPI读写FM25CL64驱动代码  
//date：2013.09.26
//author：huangzheng
//annotation:适用铁电存储器fm25系列读写代码，直接读写一个字节
//
**********************************************************/

///////////////////////////////////////////
///////////////////////////////////////////
u16 LOG_ADDR_P;		//日志地址指针
u8 LOG_num_P=0;		//日志总数
void FramWriteByte(u32 address,u8 da)
{
		u8 temH,temM,temL;
		temH=(u8)((address&0xff0000)>>16);
		temM=(u8)((address&0x00ff00)>>8);
		temL=(u8)(address&0x0000ff);

		FRAM_CS = 0;
		SPIx_ReadWriteByte(WREN);         //???
		FRAM_CS = 1;

		//delay_us(1);
		delay_us(2);

		FRAM_CS = 0;
		SPIx_ReadWriteByte(WRITE);        //???
		SPIx_ReadWriteByte(temH);         //????
		SPIx_ReadWriteByte(temM);         //????
		SPIx_ReadWriteByte(temL);         //????
		SPIx_ReadWriteByte(da);           //????
		FRAM_CS = 1;
		//mdelay(5);
		delay_us(2);
		FRAM_CS = 0;
		SPIx_ReadWriteByte(WRDI);         //???
		FRAM_CS = 1;
}
void FramWrite16Byte(u32 address,u16 da)
{
		u8 temH,temL;
		temH=(u8)((da&0xff00)>>8);
		temL=(u8)(da&0x00ff);
		FramWriteByte(address,temH);
		FramWriteByte(address+1,temL);
}

u8 FramReadByte(u32 address)
{
		u8 temp;
		u8 temH,temM,temL;
		temH=(u8)((address&0xff0000)>>16);
		temM=(u8)((address&0x00ff00)>>8);
		temL=(u8)(address&0x0000ff);

		FRAM_CS = 0;           //??CS=0
		SPIx_ReadWriteByte(READ);      //???
		SPIx_ReadWriteByte(temH);      //????
		SPIx_ReadWriteByte(temM);      //????
		SPIx_ReadWriteByte(temL);      //????
		temp = SPIx_ReadWriteByte(0xF0);
		FRAM_CS = 1;           //??CS=1
		return temp;
}
//
u16 FramRead16Byte(u32 address)
{
		u16 tem;
		tem = ((FramReadByte(address)|tem)<<8);
		tem |=FramReadByte(address+1);
		return tem;
}
void FramWriteInduce(u32 address,u32 number,u8 *p)
{
		u8 temH,temM,temL;
		u32 i;
		temH=(u8)((address&0xff0000)>>16);
		temM=(u8)((address&0x00ff00)>>8);
		temL=(u8)(address&0x0000ff);

		FRAM_CS = 0;
		SPIx_ReadWriteByte(WREN);         //???
		FRAM_CS = 1;

		delay_us(2);
		// udelay(2);

		FRAM_CS = 0;
		SPIx_ReadWriteByte(WRITE);        //???
		SPIx_ReadWriteByte(temH);         //????
		SPIx_ReadWriteByte(temM);         //????
		SPIx_ReadWriteByte(temL);         //????
		//SPIx_ReadWriteByte(da);           //????
		for(i=0;i<number;i++)
		{
		SPIx_ReadWriteByte(*p++);
		}
		FRAM_CS = 1;
		delay_us(2);
		//mdelay(5);
		FRAM_CS = 0;
		SPIx_ReadWriteByte(WRDI);         //???
		FRAM_CS = 1;
}

void FramReadInduce(u32 address,u32 number,u8 *p)
{
		u8 temH,temM,temL;
		u32 i;
		temH=(u8)((address&0xff0000)>>16);
		temM=(u8)((address&0x00ff00)>>8);
		temL=(u8)(address&0x0000ff);

		FRAM_CS = 0;           //??CS=0
		SPIx_ReadWriteByte(READ);      //???
		SPIx_ReadWriteByte(temH);      //????
		SPIx_ReadWriteByte(temM);      //????
		SPIx_ReadWriteByte(temL);      //????
		//temp = SPIx_ReadWriteByte(0xF0); //???
		for(i=0;i<number;i++)
		{
		*p++ = SPIx_ReadWriteByte(0xF0);
		}
		FRAM_CS = 1;           //??CS=1
}
void Log_Write(u8 filg)
{
	if(LOG_ADDR_P>(LOG_ADDR+1000))
		LOG_ADDR_P = LOG_ADDR;
	FramWriteInduce(LOG_ADDR_P+0,1,&TimeValue.year);
	FramWriteInduce(LOG_ADDR_P+1,1,&TimeValue.month);
	FramWriteInduce(LOG_ADDR_P+2,1,&TimeValue.date);
	FramWriteInduce(LOG_ADDR_P+3,1,&TimeValue.week);
	FramWriteInduce(LOG_ADDR_P+4,1,&TimeValue.hour);
	FramWriteInduce(LOG_ADDR_P+5,1,&TimeValue.minute);
	FramWriteInduce(LOG_ADDR_P+6,1,&TimeValue.second);
	FramWriteInduce(LOG_ADDR_P+7,1,&filg);
	LOG_ADDR_P+=8;
	FramWrite16Byte(LOG_ADDR-2,LOG_ADDR_P);
}
	


