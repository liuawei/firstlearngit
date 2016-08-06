#ifndef __MBI5025_H
#define __MBI5025_H	
#include "sys.h"


#define JXI_SDI 	  PAout(1)	// 
#define JXI_CLK   	PAout(2)	// 
#define JXI_LE    	PAout(3)	// 


void MBI5025_Init(void);//≥ı ºªØ
void MBI5025_LED(u16 data);


#endif
