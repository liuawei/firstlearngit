#include "sys.h"
int main(void)
{ 
  sys_init();
	while(1)
	{
		real_time();
//		GPIO_SetBits(GPIOA,GPIO_Pin_8);
//		printf("\n\r%d-%d--%d---%d\n\r",Car_Num1,Car_Num2,Car_Num3,Car_Num4);
//		GPIO_ResetBits(GPIOA,GPIO_Pin_8);	
//		nms(500);
		LED1(OFF);LED2(OFF);LED3(OFF);LED4(OFF);	
	}	
}
 
