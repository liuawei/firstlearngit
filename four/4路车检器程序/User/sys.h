#ifndef __SYS_H
#define __SYS_H
#include "stm32f10x.h"
#define CLI()      __set_PRIMASK(1)  
#define SEI()      __set_PRIMASK(0)
#include "bsp_usart1.h"
#include "bsp_exti.h"
#include "bsp_led.h"
#include "bsp_usart1.h"
#include "bsp_TiMbase.h" 
extern u8 Ns_num;
extern u8 Ns_num1;
extern u8 Second_Num;
extern u16 Temp_Data_ch1[10];
extern u16 Temp_Data_ch2[10];
extern u16 Temp_Data_ch3[10];
extern u16 Temp_Data_ch4[10];
//extern u8  temp;
//extern u8  temp1;
//extern u8  temp2;
//extern u8  temp3;

extern u16 Temp_Data_Buffer1;
extern u16 Temp_Data_Buffer2;
extern u16 Temp_Data_Buffer3;
extern u16 Temp_Data_Buffer4;
extern u8  Mcu_Address;
extern u16  Sensity1;
extern u16  Sensity2;
extern u16  Sensity3;
extern u16  Sensity4;
extern u8  Car_Num1;
extern u8  Car_Num2;
extern u8  Car_Num3;
extern u8  Car_Num4;
extern u8  Car_Num1_Buffer;
extern u8  Car_Num2_Buffer;
extern u8  Car_Num3_Buffer;
extern u8  Car_Num4_Buffer;
extern u8  RS485_RealtimeData1;
extern u8  RS485_RealtimeData2;
extern u8  RS485_RealtimeData3;
extern u8  RS485_RealtimeData4;
extern u8  ydflag1;
extern u8  ydflag2;
extern u8  ydflag3;
extern u8  ydflag4;
extern u8   Tim_Value;
extern u8  Ns;
extern u8  capture_number;
extern u16 Ic1_readvalue1;
extern u16 Ic1_readvalue2;
extern u16 CAPTURE;
extern u16 TIM1_FREQ;
extern u8 RS485_RX_BUF[3]; 			//���ջ���
extern u8 RS485_RX_CNT;
void sys_init(void);
void NVIC_Configuration(void);
#endif
