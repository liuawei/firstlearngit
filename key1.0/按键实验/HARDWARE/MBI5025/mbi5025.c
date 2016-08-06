#include "mbi5025.h"	
#include "delay.h"
#include "led.h"

void MBI5025_Init(void)//初始化
{
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟	
	GPIOA->CRL&=0XFFFF000F;
	GPIOA->CRL|=0X00003330;//PA8推挽输出
	GPIOA->ODR|=7<<1;     	//PA8输出高 	


}

void MBI5025_LED(u16 data)
{
	u8 i;
		JXI_LE = 0;
	for(i=0;i<16;i++)
	{
		JXI_CLK = 0;
		if(data&0x8000)//串行数据输入1
		JXI_SDI = 1;			
		else
		JXI_SDI = 0;
		JXI_CLK = 1;
		data<<=1;	
	}
		JXI_LE = 1;
}






	







