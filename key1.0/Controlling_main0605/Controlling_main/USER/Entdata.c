#include "sys.h" 


u8 Data_end = 0;
u8 BASIC_CONF[50];					//������������
u8 LED_CAR_CONF[30];					//��������
u8 LED_MAN_CONF[30];					//��������
u8 PHASE_CONF[50];					//��λ���浹����ʱ����
u8 TIME_D_CONF[32][13];				//ʱ���������
u8 PLAN_CONF[20][20];					//����������
u8 STAGE_CONF[32][4];				//�׶�����
u8 Manual_Phase[8];				//�ֶ���������
u8 Phase_Mark[32];				//��λ��ʶ
u16 Data_numbuf[20];				//��������
u8 File_Name[60];				//������
//NET��ѯ���յ�������
//�������ݸ�ʽC7 00 0b 00 cc 07 0f 0c 0f 02 0a 05 00 f2
//����ֵ��0����������ȷ��1���մ������·���
u8 NET_Receive_Data()
{
	u8 clean[400];
	u8 rxlen=UART4_RX_STA;
	u8 i=0;
	u8 err;
	u16 sum=0,data_sum=0;
	//u32 addr;
	u8 Ledbuff[16];
	const char s1[]="connect";
	const char s2[]="disconnect";
//	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if((rxlen==UART4_RX_STA&&rxlen)&&(UART4_RX_BUF[0]!=0xC7))
	{	
		
		if(strcmp(s1,UART4_RX_BUF)==0)
			printf("Connect  OK: 1--\n");
		else if(strcmp(s2,UART4_RX_BUF)==0)
			printf("Disconnect  OK: 0--\n");	
		else printf("Receive_err: %s--\n",UART4_RX_BUF);
		
		UART4_RX_STA=0;
		memset(UART4_RX_BUF,0,200);		
	}
	if((UART4_RX_STA&0x8000))//���յ�������,�ҽ��������
	{
		
		rxlen = UART4_RX_STA&0x7fff;
		memset(clean,0,400);		
			for(i=3;i<rxlen-1;i++)
			{
				sum+=UART4_RX_BUF[i];	
				//printf("Receive_data: %d--OK\n",UART4_RX_BUF[i]);
			}
			data_sum = ((UART4_RX_BUF[1]|data_sum)<<8)|UART4_RX_BUF[2];
			if(data_sum%2)
				sum+=1;
			if(UART4_RX_BUF[rxlen-1]==((~sum&0xff)+1))
			{	
				printf("Receive_data: %x--%x--OK\n",UART4_RX_BUF[4],UART4_RX_BUF[rxlen-1]);
				data_sum+=4;
				
				if(!UART4_RX_BUF[3]) 					//�жϷ��͵������ݻ�������	
				{
						
						switch (UART4_RX_BUF[4])				//�ж���������
						{
							case	0xc0:	
										Data_numbuf[0] = data_sum;	
										FramWriteInduce(BASIC_ADDR-6,data_sum,UART4_RX_BUF);				//������������
										FramReadInduce(BASIC_ADDR,data_sum-6,BASIC_CONF);		//������������
								break;
							case	0xc1:
										Data_numbuf[1] = data_sum;
										FramWriteInduce(LED_CAR_ADDR-6,data_sum,UART4_RX_BUF);				//����������
										FramReadInduce(LED_CAR_ADDR,data_sum-6,LED_CAR_CONF);			//����������
								break;
							case	0xc2:
										Data_numbuf[2] = data_sum;
										FramWriteInduce(LED_MAN_ADDR-6,data_sum,UART4_RX_BUF);				//�˵�������
										FramReadInduce(LED_MAN_ADDR,data_sum-6,LED_MAN_CONF);			//�˵�������
										Data_end = 1;
								break;			
							case	0xc3:
										Data_numbuf[3] = data_sum;
										FramWriteInduce(PHASE_ADDR-6,data_sum,UART4_RX_BUF);						//��λ���浹����ʱ����
										FramReadInduce(PHASE_ADDR,data_sum-6,PHASE_CONF);		//��λ���浹����ʱ����
								break;
							case	0xc4:
										Data_numbuf[4] = data_sum;
										FramWriteInduce(TIME_D_ADDR-6,400,clean);
										memset(*TIME_D_CONF,0,400);	
										FramWriteInduce(TIME_D_ADDR-6,data_sum,UART4_RX_BUF);				//ʱ���������
										FramReadInduce(TIME_D_ADDR,data_sum-7,*TIME_D_CONF);	//ʱ���������
								break;
							case	0xc5:	
										Data_numbuf[5] = data_sum;	
										Start_flag = 0;			
										FramWriteInduce(PLAN_ADDR-6,300,clean);								
										FramWriteInduce(PLAN_ADDR-6,data_sum,UART4_RX_BUF);//�����洢
						
								break;
							case	0xc6:
										Data_numbuf[6] = data_sum;
										Start_flag = 0;
										FramWriteInduce(STAGE_ADDR-6,data_sum,UART4_RX_BUF);				//�׶���λ���ݵ�ַ
										FramReadInduce(STAGE_ADDR,data_sum-6,*STAGE_CONF);		//�׶���λ����
								break;
							case	0xc7:
										Data_numbuf[7] = data_sum;
										Start_flag = 0;
										FramWriteInduce(PHASE_MARK_ADDR-6,data_sum,UART4_RX_BUF);		//��λ��ʶ
										FramReadInduce(STAGE_ADDR,128,STAGE_COMM);		//�׶���λ����

								break;
							case	0xc8:
										Data_numbuf[8] = data_sum;
										FramWriteInduce(File_Name_ADDR-6,data_sum,UART4_RX_BUF);		//������
								break;
							case	0xc9:
										Data_numbuf[9] = data_sum;
										FramWriteInduce(WEEK_ADDR-6,data_sum,UART4_RX_BUF);		//��������
								break;
							case	0xcc:
											TimeValue.year = UART4_RX_BUF[6];			//��
											TimeValue.month = UART4_RX_BUF[7];		//��
											TimeValue.date =UART4_RX_BUF[8];			//��
											TimeValue.week =UART4_RX_BUF[9];  		//����
											TimeValue.hour = UART4_RX_BUF[10];			//ʱ
											TimeValue.minute = UART4_RX_BUF[11];		//��
											TimeValue.second = UART4_RX_BUF[12];		//��
											DS1307_ReadWrite_Time(0);
								break;
							case	0xcf:
										for(i=0;i<UART4_RX_BUF[5];i++)
										{
											Manual_Phase[i]=UART4_RX_BUF[i+6];	
										}	
										Run_plan();
										
										//Write_Data(UART4,Manual_Phase,5);					
								break;
							default:
								break;
						}
						Start_config();
						//printf("Receive_data: --OK\n");

				}
				else
				{
					switch (UART4_RX_BUF[4])
					{
						case 0xc1:
										Data_numbuf[10] = data_sum;
										FramWriteInduce(LED_CAR_Name-6,data_sum,UART4_RX_BUF);		//��������
							break;
						case 0xc2:
										Data_numbuf[11] = data_sum;
										FramWriteInduce(LED_MAN_Name-6,data_sum,UART4_RX_BUF);		//�˵�����
							break;
						case 0xc6:
										Data_numbuf[12] = data_sum;
										FramWriteInduce(STAGE_Name-6,data_sum,UART4_RX_BUF);		//��λ�׶�����
							break;
						default:
							break;
					}
					Manual_Mode(UART4_RX_BUF);			//���ƻ��ֶ�����	
						switch (UART4_RX_BUF[6])				//�ж���������
						{
							case	0x00:	 																									//�ر������
													TIM3->CR1&=~(1<<0);  				//�رն�ʱ��3	
													Phase_cl(0xffffffff,Ledbuff,0);
													Send_APhase(Ledbuff);			//������λ����
													Bus1_Write_Data(Comm_addr,0xef,0,1);
													delay_ms(10);
													Bus1_Write_Data(Comm_addr,0xef,0,1);
													Log_Write(2);				//д��־
																break;
							case	0x01:																										//�����
													Bus1_Write_Data(Comm_addr,0xee,0,1);
													delay_ms(10);
													Bus1_Write_Data(Comm_addr,0xee,0,1);
													TIM3->CR1|=0x01;
													Log_Write(3);				//д��־
																break;
							case	0x02:		
													Log_Write(4);				//д��־
													TIM3->CR1&=~(1<<0);  				//�رն�ʱ��3					//ѭ������
								//					while(1)
													{
														for(i=0;i<16;i++)
														{		
																IWDG_Feed();			//ι�������Ź�
																Phase_cl(0xffffffff,Ledbuff,0);
																Send_APhase(Ledbuff);			//������λ����
															
																Phase_cl(0x80000000>>i,Ledbuff,1);
																Send_APhase(Ledbuff);			//������λ����
																delay_ms(1200);
																Phase_cl(0x80000000>>i,Ledbuff,2);
																Send_APhase(Ledbuff);			//������λ����
																delay_ms(1200);
																Phase_cl(0x80000000>>i,Ledbuff,3);
																Send_APhase(Ledbuff);			//������λ����
																delay_ms(1200);
														}
													}													
																
																break;
							case	0x03:				Sys_Soft_Reset();														//ϵͳ��λ
																Log_Write(5);				//д��־	
																break;
							
							case	0x04:				Up_LOG();																						//�鿴��־
																Log_Write(6);				//д��־
																break;
							case	0x05:																										//�������
																Log_Write(7);				//д��־
																break;
							case	0x06:																				//���Զ���λ����
								
																break;
							case	0x07:																										//�ϴ�����
															Up_config();	Log_Write(8);				//д��־
																break;
							case	0x08:		  //ʹ�ܶ�ʱ��3																				//������־
								
																break;
							default:
								break;
						}
				}

			}
			else
			{	
				printf("Receive_data: %x--%x--%x--fail\n",UART4_RX_BUF[4],((~sum&0xff)+1),UART4_RX_BUF[rxlen-1]);
				UART4_RX_STA=0;
				return 0;	
			}
						UART4_RX_STA=0;
		memset(UART4_RX_BUF,0,300);

	}
	if(Data_end)
	{	
		Data_end = 0;
		Sys_Soft_Reset();
		
		
	}
// 	else
// 	printf("Receive_err: %s--\n",UART4_RX_BUF);
	return err;
}

/*************************************************
���������������ݽ���
������*buff��������     ����ֵ err ������� 
**************************************************/	 
u8 Bus_Data(u8 *buff)
{
		u8 err;
		switch (buff[1])
		{
			case 0x00:
				break;
			case 0x01:
				break;
			case 0x03:
				break;
			case 0x04:
				break;
			case 0x05:
				break;
			case 0x06:
				break;
			case 0x07:
				break;
			case 0x08:
				break;
			case 0x0e:			Manual_Phase[0] = buff[2];
											Manual_Phase[1] = buff[3];
											Run_plan();
				break;
			
			default:
				break;
		}
	
	
		return err;
}
#define send_dely 150
/*************************************************
���������ϴ�����
������
**************************************************/	 
u8  UART4_TX_BUF[UART4_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
void Up_config()
{
	
		FramReadInduce(BASIC_ADDR-6,Data_numbuf[0],UART4_TX_BUF);		//������������
			DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[0]);
		delay_ms(send_dely);
		FramReadInduce(LED_CAR_ADDR-6,Data_numbuf[1],UART4_TX_BUF);			//����������
			DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[1]);
		delay_ms(send_dely);
		FramReadInduce(LED_MAN_ADDR-6,Data_numbuf[2],UART4_TX_BUF);			//�˵�������
			DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[2]);
		delay_ms(send_dely);
		FramReadInduce(PHASE_ADDR-6,Data_numbuf[3],UART4_TX_BUF);		//��λ���浹����ʱ����
			DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[3]);
		delay_ms(send_dely);
		FramReadInduce(TIME_D_ADDR-6,Data_numbuf[4],UART4_TX_BUF);	//ʱ���������
			DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[4]);
		delay_ms(100);
		FramReadInduce(PLAN_ADDR-6,Data_numbuf[5],UART4_TX_BUF);//�����洢
			DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[5]);
		delay_ms(send_dely);
		FramReadInduce(STAGE_ADDR-6,Data_numbuf[6],UART4_TX_BUF);		//�׶���λ����
			DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[6]);
		delay_ms(send_dely);
		FramReadInduce(PHASE_MARK_ADDR-6,Data_numbuf[7],UART4_TX_BUF);		//��λ��ʶ
				DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[7]);
		delay_ms(send_dely);
		FramReadInduce(File_Name_ADDR-6,Data_numbuf[8],UART4_TX_BUF);		//��λ��ʶ
				DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[8]);
		delay_ms(send_dely);
		FramReadInduce(WEEK_ADDR-6,Data_numbuf[9],UART4_TX_BUF);		//��λ��ʶ
				DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[9]);
		delay_ms(send_dely);
		FramReadInduce(LED_CAR_Name-6,Data_numbuf[10],UART4_TX_BUF);		//��λ��ʶ
				DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[10]);
		delay_ms(send_dely);
		FramReadInduce(LED_MAN_Name-6,Data_numbuf[11],UART4_TX_BUF);		//��λ��ʶ
				DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[11]);
		delay_ms(send_dely);
		FramReadInduce(STAGE_Name-6,Data_numbuf[12],UART4_TX_BUF);		//��λ��ʶ
				DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[12]);
		delay_ms(send_dely);
		
}
//�ϴ���־
void Up_LOG()
{
	u8 logbuff[8];
	u8 i;
	u8 Send_data[10][10]={
											"����",
											"ʱ��У׼",
											"�ر����",
											"�����",
											"ѭ�����",
											"ϵͳ��λ",
											"�鿴��־",
											"�������",
											"�ϴ�����",
											"���ط���",
											
											};
	
//	printf("Send_data: %d--%d\n",LOG_ADDR_P,(LOG_ADDR_P-LOG_ADDR));
		for(i=0;i<((LOG_ADDR_P-LOG_ADDR)/8);i++)
	{
		
		FramReadInduce(LOG_ADDR+i*8,8,logbuff);
		printf("��%d����־--%d��%d��-%d��-����%d-%dʱ-%d��-%d��--%s\n",
		i,logbuff[0],logbuff[1],logbuff[2],logbuff[3],logbuff[4],logbuff[5],logbuff[6],Send_data[logbuff[7]]);    //ʱ��
		
	}
	
	
	
	
}













