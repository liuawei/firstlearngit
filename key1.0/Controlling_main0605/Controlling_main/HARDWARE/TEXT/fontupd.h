#ifndef __FONTUPD_H__
#define __FONTUPD_H__	 
#include <stm32f10x_lib.h>	 
//////////////////////////////////////////////////////////////////////////////////	 
//���ĺ���֧�ֳ��� ��������		   
//�����������,�Լ��ֿ��׵�ַ��ȡ2������.
//////////////////////////////////////////////////////////////////////////////////	 


//#define EN_UPDATE_FONT //ʹ���������,ͨ���ر�����ʵ�ֽ�ֹ�ֿ����

 
extern u32 FONT16ADDR ;
extern u32 FONT12ADDR ;
extern u32 UNI2GBKADDR;

u8 Update_Font(void);//�����ֿ�   
u8 Font_Init(void);//��ʼ���ֿ�
#endif




















