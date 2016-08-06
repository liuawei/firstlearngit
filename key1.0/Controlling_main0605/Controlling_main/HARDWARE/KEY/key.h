#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//按键输入 驱动代码		   
//********************************************************************************


#define KEY_GSM 	PDin(8)    
#define KEY_AH 		PDin(10)	
#define KEY_R 		PDin(12)	
#define KEY_G 		PDin(14)    

	 
void KEY_Init(void);//IO初始化
void KEY_Scan(void);  //按键扫描函数		
void LED_hand(u8 num);
#endif
