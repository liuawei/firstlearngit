#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//�������� ��������		   
//********************************************************************************


#define KEY_GSM 	PDin(8)    
#define KEY_AH 		PDin(10)	
#define KEY_R 		PDin(12)	
#define KEY_G 		PDin(14)    

	 
void KEY_Init(void);//IO��ʼ��
void KEY_Scan(void);  //����ɨ�躯��		
void LED_hand(u8 num);
#endif
