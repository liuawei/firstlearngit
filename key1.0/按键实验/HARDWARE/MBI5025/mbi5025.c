#include "mbi5025.h"	
#include "delay.h"
#include "led.h"

void MBI5025_Init(void)//��ʼ��
{
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��	
	GPIOA->CRL&=0XFFFF000F;
	GPIOA->CRL|=0X00003330;//PA8�������
	GPIOA->ODR|=7<<1;     	//PA8����� 	


}

void MBI5025_LED(u16 data)
{
	u8 i;
		JXI_LE = 0;
	for(i=0;i<16;i++)
	{
		JXI_CLK = 0;
		if(data&0x8000)//������������1
		JXI_SDI = 1;			
		else
		JXI_SDI = 0;
		JXI_CLK = 1;
		data<<=1;	
	}
		JXI_LE = 1;
}






	







