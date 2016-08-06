#include "sys.h"	 

//////////////////////////////////////////////////////////////////////////////////	 
//RS485���� ����	   									  
//////////////////////////////////////////////////////////////////////////////////	
u8 Fault_det[4];	
u8 Temp_det[4]={0,0,0,0};
u8 Jiedian;
u8 RS485_BUF[5][32];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
u8 RS485_CNT[5]; 
u8 RS485_CNT_tmp[5];
//��ʼ��RS485_IO 	  
void RS485_Init(void)
{  	 

	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��
	RCC->APB2ENR|=1<<3;    //ʹ��PORTAʱ��
	GPIOA->CRL&=0XFFFFFF0F;
	GPIOA->CRL|=0X00000030;//PA1�������
	GPIOB->CRL&=0XFFFFFF0F;
	GPIOB->CRL|=0X00000030;//PB1�������
	CON1 = 1;CON2 = 1;
		
	CON1 = 0;CON2 = 0;
}

//��ָ����ַ��ʼд��ָ������������
//uart :���ô���
//pBuffer   :���������׵�ַ
//Len       :���ݳ���
 void Write_Buff(USART_TypeDef *uart,u8 *pBuffer,u8 Len)
{
	u8 i;

	while((uart->SR&0X40)==0);//�ȴ����ͽ���		  

	for(i=0;i<Len;i++)		//ѭ����������
	
	{
		while((uart->SR&0X40)==0);//�ȴ����ͽ���		  	
		uart->DR = pBuffer[i];
	}	 
	while((uart->SR&0X40)==0);//�ȴ����ͽ���
}
//��ָ����ַ��ʼд��ָ������������
//pBuffer   :���������׵�ַ
//Len       :���ݳ���
 void DMA_Send_Buff(u8 *pBuffer,u16 Len)
{
	MYDMA_Config(DMA2_Channel5,(u32)&UART4->DR,(u32)pBuffer,Len);//DMA1ͨ��4,����Ϊ����1,�洢��ΪSendBuff,��(TEXT_LENTH+2)*100.
	 
	MYDMA_Enable(DMA2_Channel5,Len);//��ʼһ��DMA���䣡
}

//��ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ 
//flag:   ���ݱ��     0 �Լ�Ӧ��    1-*  �Զ���
//pBuffer   :���������׵�ַ
//Len       :���ݳ���
void Bus1_Write_Data(u8 WriteAddr,u8 flag,u8 *pBuffer,u8 Len)
{

	u8 i;
	CON1=1;
	while((USART2->SR&0X40)==0);//�ȴ����ͽ���	
	USART2->DR=WriteAddr|0x100;	//д��ַ		
	while((USART2->SR&0X40)==0);//�ȴ����ͽ���	
	USART2->DR=flag;	//д��ַ	
	for(i=0;i<Len;i++)		//ѭ����������
	
	{
		while((USART2->SR&0X40)==0);//�ȴ����ͽ���		  	
		USART2->DR = pBuffer[i];
	}	 
	while((USART2->SR&0X40)==0);//�ȴ����ͽ���
	//delay_ms(5);
	CON1=0;


}
//��ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ 
//flag:   ���ݱ��     0 �Լ�Ӧ��    1-*  �Զ���
//pBuffer   :���������׵�ַ
//Len       :���ݳ���
void Bus2_Write_Data(u8 WriteAddr,u8 flag,u8 *pBuffer,u8 Len)
{

	u8 i;
	CON2=1;
	delay_us(500);
	while((USART3->SR&0X40)==0);//�ȴ����ͽ���	
	USART3->DR=WriteAddr|0x100;	//д��ַ		
	while((USART3->SR&0X40)==0);//�ȴ����ͽ���	
	USART3->DR=flag;	//д��ַ	
	for(i=0;i<Len;i++)		//ѭ����������
	
	{
		while((USART3->SR&0X40)==0);//�ȴ����ͽ���		  	
		USART3->DR = pBuffer[i];
	}	 
	
	while((USART3->SR&0X40)==0);//�ȴ����ͽ���
	CON2=0;
//  delay_us(500);

}
//��ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ 
//flag:   ���ݱ��     0 �Լ�Ӧ��    1-*  �Զ���
//pBuffer   :���������׵�ַ
//Len       :���ݳ���
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
	while((UART4->SR&0X40)==0);//�ȴ����ͽ���	
	UART4->DR=0xc7;	//д��ַ	
	while((UART4->SR&0X40)==0);//�ȴ����ͽ���	
	UART4->DR=0x00;	//д��ַ	
	while((UART4->SR&0X40)==0);//�ȴ����ͽ���	
	UART4->DR=Len+3;	//д��ַ	
	while((UART4->SR&0X40)==0);//�ȴ����ͽ���	
	UART4->DR=cmd;	//д��ַ	
	while((UART4->SR&0X40)==0);//�ȴ����ͽ���	
	UART4->DR=flag;	//д��ַ	
	while((UART4->SR&0X40)==0);//�ȴ����ͽ���	
	UART4->DR=Len;	//д��ַ	
	Write_Buff(UART4,pBuffer, Len);
	while((UART4->SR&0X40)==0);//�ȴ����ͽ���	
	UART4->DR=(~sum&0xff)+1;	//д��ַ	
	while((UART4->SR&0X40)==0);//�ȴ����ͽ���	
}

//��ָ����ַ��ʼд��ָ������������
//uart :���ô���
//WriteAddr :��ʼд��ĵ�ַ 
//pBuffer   :���������׵�ַ
//Len       :���ݳ���
 void RS485_Write_Data(USART_TypeDef *uart,u8 WriteAddr,u8 *pBuffer,u8 Len)
{
	u8 i;
	CON1=1;
	while((uart->SR&0X40)==0);//�ȴ����ͽ���		  
	uart->DR=WriteAddr|0x100;	//д��ַ	
	for(i=0;i<Len;i++)		//ѭ����������
	
	{
		while((uart->SR&0X40)==0);//�ȴ����ͽ���		  	
		uart->DR = pBuffer[i];
	}	 
	while((uart->SR&0X40)==0);//�ȴ����ͽ���
	
	delay_ms(5);
	CON1=0;
}
//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void RS485_Receive_Data(u8 con,u8 *buf,u8 *len)
{
	u8 rxlen=RS485_CNT[con];
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==RS485_CNT[con]&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_BUF[con][i];	
		}		
		*len=RS485_CNT[con];	//��¼�������ݳ���
		RS485_CNT_tmp[con] = RS485_CNT[con];
		RS485_CNT[con]=0;		//����
	}
}




//��λ�ĸ�����
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
//			RS485_CNT[2]=0;		//���
//	delay_ms(10);
//		if((RS485_BUF[2][1]!=0xdd)||(RS485_BUF[2][2]!=0xdd))
// 			{
//				printf("Get_led==%x==%x\n",RS485_BUF[2][1],RS485_BUF[2][2]);
// 				delay_ms(10);
// 				goto  Send_Drive;
// 			}
//			else return;

}
//����������λ
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
//���ư���λ״̬����
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
��������ͨ�Ű�����
������
**************************************************/	 
u8 Ccomm_board_data()
{
		u8 err=0;
		err_num++;
		IWDG_Feed();			//ι�������Ź�
		delay_ms(10);
		Bus1_Write_Data(Comm_addr,0x01,Run_plan_data,20);		//�������з�������
		delay_ms(10);
		Bus1_Write_Data(Comm_addr,0x02,*STAGE_CONF,20);		//������λ
		delay_ms(10);
		Bus1_Write_Data(Comm_addr,0x03,*Countdown,30);		//������λ��ʶ
//	printf("Countdown---%x-%x-%x-%x-%x-%x\n",Countdown[0][0],Countdown[0][1],Countdown[0][2],Countdown[1][0],Countdown[1][1],Countdown[1][2]);	
		delay_ms(10);
		Bus1_Write_Data(Comm_addr,0x04,LED_CAR_CONF,8);		//������λ��ʶ
		RS485_CNT[1]=0;		//����	
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
����������Դ������
������
**************************************************/	 
u8 Power_board_data(u8 cmd,u8 *rec_data)
{
		u8 err=0;
		u8 power_dat[2];
		err_num++;
		IWDG_Feed();			//ι�������Ź�
		delay_ms(10);
		Bus1_Write_Data(Power_addr,cmd,power_dat,2);

		RS485_CNT[1]=0;		//����	
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










