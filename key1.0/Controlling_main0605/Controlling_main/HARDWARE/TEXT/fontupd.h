#ifndef __FONTUPD_H__
#define __FONTUPD_H__	 
#include <stm32f10x_lib.h>	 
//////////////////////////////////////////////////////////////////////////////////	 
//中文汉字支持程序 驱动代码		   
//包括字体更新,以及字库首地址获取2个函数.
//////////////////////////////////////////////////////////////////////////////////	 


//#define EN_UPDATE_FONT //使能字体更新,通过关闭这里实现禁止字库更新

 
extern u32 FONT16ADDR ;
extern u32 FONT12ADDR ;
extern u32 UNI2GBKADDR;

u8 Update_Font(void);//更新字库   
u8 Font_Init(void);//初始化字库
#endif





















