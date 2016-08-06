#include "sys.h"	 

//////////////////////////////////////////////////////////////////////////////////	 
//RS485Çı¶¯ ´úÂë	   									  
//////////////////////////////////////////////////////////////////////////////////	
u8 Fault_det[4];	
u8 Temp_det[4]={0,0,0,0};
u8 Jiedian;
u8 RS485_BUF[5][32];  	//½ÓÊÕ»º³å,×î´ó64¸ö×Ö½Ú.
//½ÓÊÕµ½µÄÊı¾İ³¤¶È
u8 RS485_CNT[5]; 
u8 RS485_CNT_tmp[5];
//³õÊ¼»¯RS485_IO 	  
void RS485_Init(void)
{  	 

	RCC->APB2ENR|=1<<2;    //Ê¹ÄÜPORTAÊ±ÖÓ
	RCC->APB2ENR|=1<<3;    //Ê¹ÄÜPORTAÊ±ÖÓ
	GPIOA->CRL&=0XFFFFFF0F;
	GPIOA->CRL|=0X00000030;//PA1ÍÆÍìÊä³ö
	GPIOB->CRL&=0XFFFFFF0F;
	GPIOB->CRL|=0X00000030;//PB1ÍÆÍìÊä³ö
	CON1 = 1;CON2 = 1;
		
	CON1 = 0;CON2 = 0;
}

//ÔÚÖ¸¶¨µØÖ·¿ªÊ¼Ğ´ÈëÖ¸¶¨¸öÊıµÄÊı¾İ
//uart :ÉèÖÃ´®¿Ú
//pBuffer   :Êı¾İÊı×éÊ×µØÖ·
//Len       :Êı¾İ³¤¶È
 void Write_Buff(USART_TypeDef *uart,u8 *pBuffer,u8 Len)
{
	u8 i;

	while((uart->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø		  

	for(i=0;i<Len;i++)		//Ñ­»··¢ËÍÊı¾İ
	
	{
		while((uart->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø		  	
		uart->DR = pBuffer[i];
	}	 
	while((uart->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø
}
//ÔÚÖ¸¶¨µØÖ·¿ªÊ¼Ğ´ÈëÖ¸¶¨¸öÊıµÄÊı¾İ
//pBuffer   :Êı¾İÊı×éÊ×µØÖ·
//Len       :Êı¾İ³¤¶È
 void DMA_Send_Buff(u8 *pBuffer,u16 Len)
{
	MYDMA_Config(DMA2_Channel5,(u32)&UART4->DR,(u32)pBuffer,Len);//DMA1Í¨µÀ4,ÍâÉèÎª´®¿Ú1,´æ´¢Æ÷ÎªSendBuff,³¤(TEXT_LENTH+2)*100.
	 
	MYDMA_Enable(DMA2_Channel5,Len);//¿ªÊ¼Ò»´ÎDMA´«Êä£¡
}

//ÔÚÖ¸¶¨µØÖ·¿ªÊ¼Ğ´ÈëÖ¸¶¨¸öÊıµÄÊı¾İ
//WriteAddr :¿ªÊ¼Ğ´ÈëµÄµØÖ· 
//flag:   Êı¾İ±ê¼Ç     0 ×Ô¼ìÓ¦´ğ    1-*  ×Ô¶¨Òå
//pBuffer   :Êı¾İÊı×éÊ×µØÖ·
//Len       :Êı¾İ³¤¶È
void Bus1_Write_Data(u8 WriteAddr,u8 flag,u8 *pBuffer,u8 Len)
{

	u8 i;
	CON1=1;
	while((USART2->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø	
	USART2->DR=WriteAddr|0x100;	//Ğ´µØÖ·		
	while((USART2->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø	
	USART2->DR=flag;	//Ğ´µØÖ·	
	for(i=0;i<Len;i++)		//Ñ­»··¢ËÍÊı¾İ
	
	{
		while((USART2->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø		  	
		USART2->DR = pBuffer[i];
	}	 
	while((USART2->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø
	//delay_ms(5);
	CON1=0;


}
//ÔÚÖ¸¶¨µØÖ·¿ªÊ¼Ğ´ÈëÖ¸¶¨¸öÊıµÄÊı¾İ
//WriteAddr :¿ªÊ¼Ğ´ÈëµÄµØÖ· 
//flag:   Êı¾İ±ê¼Ç     0 ×Ô¼ìÓ¦´ğ    1-*  ×Ô¶¨Òå
//pBuffer   :Êı¾İÊı×éÊ×µØÖ·
//Len       :Êı¾İ³¤¶È
void Bus2_Write_Data(u8 WriteAddr,u8 flag,u8 *pBuffer,u8 Len)
{

	u8 i;
	CON2=1;
	delay_us(500);
	while((USART3->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø	
	USART3->DR=WriteAddr|0x100;	//Ğ´µØÖ·		
	while((USART3->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø	
	USART3->DR=flag;	//Ğ´µØÖ·	
	for(i=0;i<Len;i++)		//Ñ­»··¢ËÍÊı¾İ
	
	{
		while((USART3->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø		  	
		USART3->DR = pBuffer[i];
	}	 
	
	while((USART3->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø
	CON2=0;
//  delay_us(500);

}
//ÔÚÖ¸¶¨µØÖ·¿ªÊ¼Ğ´ÈëÖ¸¶¨¸öÊıµÄÊı¾İ
//WriteAddr :¿ªÊ¼Ğ´ÈëµÄµØÖ· 
//flag:   Êı¾İ±ê¼Ç     0 ×Ô¼ìÓ¦´ğ    1-*  ×Ô¶¨Òå
//pBuffer   :Êı¾İÊı×éÊ×µØÖ·
//Len       :Êı¾İ³¤¶È
void Ent_Write_Data(u8 cmd,u8 flag,u8 *pBuffer,u8 Len)
{
	u8 i;
	u16 sum,data_sum;
	sum = cmd+flag+Len;
		for(i=0;i<Len;i++)
	{
		sum+=pBuffer[i];	
		//printf("Receive_data: %d--OK\n",UART4_RX_BUF[i]);
	}
	data_sum = Len+3;
	if(data_sum%2)
		sum+=1;
	while((UART4->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø	
	UART4->DR=0xc7;	//Ğ´µØÖ·	
	while((UART4->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø	
	UART4->DR=0x00;	//Ğ´µØÖ·	
	while((UART4->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø	
	UART4->DR=Len+3;	//Ğ´µØÖ·	
	while((UART4->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø	
	UART4->DR=cmd;	//Ğ´µØÖ·	
	while((UART4->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø	
	UART4->DR=flag;	//Ğ´µØÖ·	
	while((UART4->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø	
	UART4->DR=Len;	//Ğ´µØÖ·	
	Write_Buff(UART4,pBuffer, Len);
	while((UART4->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø	
	UART4->DR=(~sum&0xff)+1;	//Ğ´µØÖ·	
	while((UART4->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø	
}

//ÔÚÖ¸¶¨µØÖ·¿ªÊ¼Ğ´ÈëÖ¸¶¨¸öÊıµÄÊı¾İ
//uart :ÉèÖÃ´®¿Ú
//WriteAddr :¿ªÊ¼Ğ´ÈëµÄµØÖ· 
//pBuffer   :Êı¾İÊı×éÊ×µØÖ·
//Len       :Êı¾İ³¤¶È
 void RS485_Write_Data(USART_TypeDef *uart,u8 WriteAddr,u8 *pBuffer,u8 Len)
{
	u8 i;
	CON1=1;
	while((uart->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø		  
	uart->DR=WriteAddr|0x100;	//Ğ´µØÖ·	
	for(i=0;i<Len;i++)		//Ñ­»··¢ËÍÊı¾İ
	
	{
		while((uart->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø		  	
		uart->DR = pBuffer[i];
	}	 
	while((uart->SR&0X40)==0);//µÈ´ı·¢ËÍ½áÊø
	
	delay_ms(5);
	CON1=0;
}
//RS485²éÑ¯½ÓÊÕµ½µÄÊı¾İ
//buf:½ÓÊÕ»º´æÊ×µØÖ·
//len:¶Áµ½µÄÊı¾İ³¤¶È
void RS485_Receive_Data(u8 con,u8 *buf,u8 *len)
{
	u8 rxlen=RS485_CNT[con];
	u8 i=0;
	*len=0;				//Ä¬ÈÏÎª0
	delay_ms(10);		//µÈ´ı10ms,Á¬Ğø³¬¹ı10msÃ»ÓĞ½ÓÊÕµ½Ò»¸öÊı¾İ,ÔòÈÏÎª½ÓÊÕ½áÊø
	if(rxlen==RS485_CNT[con]&&rxlen)//½ÓÊÕµ½ÁËÊı¾İ,ÇÒ½ÓÊÕÍê³ÉÁË
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_BUF[con][i];	
		}		
		*len=RS485_CNT[con];	//¼ÇÂ¼±¾´ÎÊı¾İ³¤¶È
		RS485_CNT_tmp[con] = RS485_CNT[con];
		RS485_CNT[con]=0;		//ÇåÁã
	}
}




//ÏàÎ»ËÄ¸ö·¢ËÍ
//
//
void Send_Phase(u8 *PhaseBuff,u8 num)
{
	u8 Sendbuf[8];
//	Sendbuf[0] = 0xd0|num;
	Sendbuf[0] = PhaseBuff[0+(num*4)];
	Sendbuf[1] = PhaseBuff[1+(num*4)];
	Sendbuf[2] = PhaseBuff[2+(num*4)];
	Sendbuf[3] = PhaseBuff[3+(num*4)];
	Sendbuf[4] = 0xdd;
	Sendbuf[5] = 0xdd;

	//Send_Drive:	
//	printf("Send_Phase: --%x--\n",num);
			//	printf("Send_Phase==%x==%x==%x==%x\n",Sendbuf[0],Sendbuf[1],Sendbuf[2],Sendbuf[3]);	
			Bus2_Write_Data(Drive_addr+num,0x02,Sendbuf,6);
//			memset(RS485_BUF[2],0,20);	
//			RS485_CNT[2]=0;		//ÇåÁ
//	delay_ms(10);
//		if((RS485_BUF[2][1]!=0xdd)||(RS485_BUF[2][2]!=0xdd))
// 			{
//				printf("Get_led==%x==%x\n",RS485_BUF[2][1],RS485_BUF[2][2]);
// 				delay_ms(10);
// 				goto  Send_Drive;
// 			}
//			else return;

}
//·¢ËÍËùÓĞÏàÎ»
void Send_APhase(u8 *PhaseBuff)
{
	u8 i=0;
//	printf("Send_Phase: --%x--\n",i);
	for(i=0;i<4;i++)
	{
		delay_ms(1);	
		Send_Phase(PhaseBuff,i);
		
	}

//	delay_ms(10);
	
}
//¿ØÖÆ°åÏàÎ»×´Ì¬·¢ËÍ
void Send_Phase_State(u8 *PhaseBuff)
{
//	u8 i;
	if(Manual_Phase[0]==0xd0)
	{
		Bus1_Write_Data(0x0a,0xd2,PhaseBuff,16);
	}
	
}
u8 err_num=0;
/*************************************************
º¯ÊıÃû£ºÍ¨ĞÅ°åÊı¾İ
²ÎÊı£º
**************************************************/	 
u8 Ccomm_board_data()
{
		u8 err=0;
		err_num++;
		IWDG_Feed();			//Î¹¶ÀÁ¢¿´ÃÅ¹·
		delay_ms(10);
		Bus1_Write_Data(Comm_addr,0x01,Run_plan_data,20);		//·¢ËÍÔËĞĞ·½°¸Êı¾İ
		delay_ms(10);
		Bus1_Write_Data(Comm_addr,0x02,*STAGE_CONF,20);		//·¢ËÍÏàÎ»
		delay_ms(10);
		Bus1_Write_Data(Comm_addr,0x03,*Countdown,30);		//·¢ËÍÏàÎ»±êÊ¶
//	printf("Countdown---%x-%x-%x-%x-%x-%x\n",Countdown[0][0],Countdown[0][1],Countdown[0][2],Countdown[1][0],Countdown[1][1],Countdown[1][2]);	
		delay_ms(10);
		Bus1_Write_Data(Comm_addr,0x04,LED_CAR_CONF,8);		//·¢ËÍÏàÎ»±êÊ¶
		RS485_CNT[1]=0;		//ÇåÁã	
		memset(RS485_BUF[1],0,100);
			delay_ms(20);
//	printf("Get_led==%x---%x\n",RS485_BUF[1][0],RS485_BUF[1][1]);
		if(RS485_BUF[1][1]==0xaa)							
		{	err = 0; FAULE_LED = 1;}
		else  {	
						err = 1; FAULE_LED = 0; delay_ms(10);
						if(err_num==50)
							{err_num=0;err=0;}
					}

		return err;
}
/*************************************************
º¯ÊıÃû£ºµçÔ´°åÊı¾İ
²ÎÊı£º
**************************************************/	 
u8 Power_board_data(u8 cmd,u8 *rec_data)
{
		u8 err=0;
		u8 power_dat[2];
		err_num++;
		IWDG_Feed();			//Î¹¶ÀÁ¢¿´ÃÅ¹·
		delay_ms(10);
		Bus1_Write_Data(Power_addr,cmd,power_dat,2);

		RS485_CNT[1]=0;		//ÇåÁã	
		memset(RS485_BUF[1],0,100);
			delay_ms(70);
		rec_data[0] = RS485_BUF[1][4];
		rec_data[1] = RS485_BUF[1][5];
		rec_data[2] = RS485_BUF[1][6];
		rec_data[3] = RS485_BUF[1][7];
//	printf("Get_led==%x---%x\n",RS485_BUF[1][0],RS485_BUF[1][1]);
//		if(RS485_BUF[1][1]==0xaa)							
//		{	err = 0; FAULE_LED = 1;}
//		else  {	
//						err = 1; FAULE_LED = 0; delay_ms(10);
//						if(err_num==50)
//							{err_num=0;err=0;}
//					}

		return err;
}	










