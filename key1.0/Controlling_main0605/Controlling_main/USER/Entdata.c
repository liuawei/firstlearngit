#include "sys.h" 


u8 Data_end = 0;
u8 BASIC_CONF[50];					//基本配置数据
u8 LED_CAR_CONF[30];					//灯序数据
u8 LED_MAN_CONF[30];					//灯序数据
u8 PHASE_CONF[50];					//相位跟随倒数计时数据
u8 TIME_D_CONF[32][13];				//时间调度数据
u8 PLAN_CONF[20][20];					//方案总数据
u8 STAGE_CONF[32][4];				//阶段内容
u8 Manual_Phase[8];				//手动控制数据
u8 Phase_Mark[32];				//相位标识
u16 Data_numbuf[20];				//数据总数
u8 File_Name[60];				//方案名
//NET查询接收到的数据
//接收数据格式C7 00 0b 00 cc 07 0f 0c 0f 02 0a 05 00 f2
//返回值：0接收数据正确，1接收错误重新发送
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
//	delay_ms(10);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
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
	if((UART4_RX_STA&0x8000))//接收到了数据,且接收完成了
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
				
				if(!UART4_RX_BUF[3]) 					//判断发送的是数据还是命令	
				{
						
						switch (UART4_RX_BUF[4])				//判断数据内容
						{
							case	0xc0:	
										Data_numbuf[0] = data_sum;	
										FramWriteInduce(BASIC_ADDR-6,data_sum,UART4_RX_BUF);				//基本配置数据
										FramReadInduce(BASIC_ADDR,data_sum-6,BASIC_CONF);		//基本配置数据
								break;
							case	0xc1:
										Data_numbuf[1] = data_sum;
										FramWriteInduce(LED_CAR_ADDR-6,data_sum,UART4_RX_BUF);				//车灯序数据
										FramReadInduce(LED_CAR_ADDR,data_sum-6,LED_CAR_CONF);			//车灯序数据
								break;
							case	0xc2:
										Data_numbuf[2] = data_sum;
										FramWriteInduce(LED_MAN_ADDR-6,data_sum,UART4_RX_BUF);				//人灯序数据
										FramReadInduce(LED_MAN_ADDR,data_sum-6,LED_MAN_CONF);			//人灯序数据
										Data_end = 1;
								break;			
							case	0xc3:
										Data_numbuf[3] = data_sum;
										FramWriteInduce(PHASE_ADDR-6,data_sum,UART4_RX_BUF);						//相位跟随倒数计时数据
										FramReadInduce(PHASE_ADDR,data_sum-6,PHASE_CONF);		//相位跟随倒数计时数据
								break;
							case	0xc4:
										Data_numbuf[4] = data_sum;
										FramWriteInduce(TIME_D_ADDR-6,400,clean);
										memset(*TIME_D_CONF,0,400);	
										FramWriteInduce(TIME_D_ADDR-6,data_sum,UART4_RX_BUF);				//时间调度数据
										FramReadInduce(TIME_D_ADDR,data_sum-7,*TIME_D_CONF);	//时间调度数据
								break;
							case	0xc5:	
										Data_numbuf[5] = data_sum;	
										Start_flag = 0;			
										FramWriteInduce(PLAN_ADDR-6,300,clean);								
										FramWriteInduce(PLAN_ADDR-6,data_sum,UART4_RX_BUF);//方案存储
						
								break;
							case	0xc6:
										Data_numbuf[6] = data_sum;
										Start_flag = 0;
										FramWriteInduce(STAGE_ADDR-6,data_sum,UART4_RX_BUF);				//阶段相位数据地址
										FramReadInduce(STAGE_ADDR,data_sum-6,*STAGE_CONF);		//阶段相位数据
								break;
							case	0xc7:
										Data_numbuf[7] = data_sum;
										Start_flag = 0;
										FramWriteInduce(PHASE_MARK_ADDR-6,data_sum,UART4_RX_BUF);		//相位标识
										FramReadInduce(STAGE_ADDR,128,STAGE_COMM);		//阶段相位数据

								break;
							case	0xc8:
										Data_numbuf[8] = data_sum;
										FramWriteInduce(File_Name_ADDR-6,data_sum,UART4_RX_BUF);		//方案名
								break;
							case	0xc9:
										Data_numbuf[9] = data_sum;
										FramWriteInduce(WEEK_ADDR-6,data_sum,UART4_RX_BUF);		//日期类型
								break;
							case	0xcc:
											TimeValue.year = UART4_RX_BUF[6];			//年
											TimeValue.month = UART4_RX_BUF[7];		//月
											TimeValue.date =UART4_RX_BUF[8];			//日
											TimeValue.week =UART4_RX_BUF[9];  		//星期
											TimeValue.hour = UART4_RX_BUF[10];			//时
											TimeValue.minute = UART4_RX_BUF[11];		//分
											TimeValue.second = UART4_RX_BUF[12];		//秒
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
										FramWriteInduce(LED_CAR_Name-6,data_sum,UART4_RX_BUF);		//车灯名称
							break;
						case 0xc2:
										Data_numbuf[11] = data_sum;
										FramWriteInduce(LED_MAN_Name-6,data_sum,UART4_RX_BUF);		//人灯名称
							break;
						case 0xc6:
										Data_numbuf[12] = data_sum;
										FramWriteInduce(STAGE_Name-6,data_sum,UART4_RX_BUF);		//相位阶段名称
							break;
						default:
							break;
					}
					Manual_Mode(UART4_RX_BUF);			//控制机手动控制	
						switch (UART4_RX_BUF[6])				//判断数据内容
						{
							case	0x00:	 																									//关闭外外灯
													TIM3->CR1&=~(1<<0);  				//关闭定时器3	
													Phase_cl(0xffffffff,Ledbuff,0);
													Send_APhase(Ledbuff);			//发送相位设置
													Bus1_Write_Data(Comm_addr,0xef,0,1);
													delay_ms(10);
													Bus1_Write_Data(Comm_addr,0xef,0,1);
													Log_Write(2);				//写日志
																break;
							case	0x01:																										//打开外灯
													Bus1_Write_Data(Comm_addr,0xee,0,1);
													delay_ms(10);
													Bus1_Write_Data(Comm_addr,0xee,0,1);
													TIM3->CR1|=0x01;
													Log_Write(3);				//写日志
																break;
							case	0x02:		
													Log_Write(4);				//写日志
													TIM3->CR1&=~(1<<0);  				//关闭定时器3					//循环测试
								//					while(1)
													{
														for(i=0;i<16;i++)
														{		
																IWDG_Feed();			//喂独立看门狗
																Phase_cl(0xffffffff,Ledbuff,0);
																Send_APhase(Ledbuff);			//发送相位设置
															
																Phase_cl(0x80000000>>i,Ledbuff,1);
																Send_APhase(Ledbuff);			//发送相位设置
																delay_ms(1200);
																Phase_cl(0x80000000>>i,Ledbuff,2);
																Send_APhase(Ledbuff);			//发送相位设置
																delay_ms(1200);
																Phase_cl(0x80000000>>i,Ledbuff,3);
																Send_APhase(Ledbuff);			//发送相位设置
																delay_ms(1200);
														}
													}													
																
																break;
							case	0x03:				Sys_Soft_Reset();														//系统复位
																Log_Write(5);				//写日志	
																break;
							
							case	0x04:				Up_LOG();																						//查看日志
																Log_Write(6);				//写日志
																break;
							case	0x05:																										//清除故障
																Log_Write(7);				//写日志
																break;
							case	0x06:																				//打开自动复位功能
								
																break;
							case	0x07:																										//上传配置
															Up_config();	Log_Write(8);				//写日志
																break;
							case	0x08:		  //使能定时器3																				//保存日志
								
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
函数名：总线数据解析
参数：*buff总线数据     返回值 err 解析结果 
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
函数名：上传配置
参数：
**************************************************/	 
u8  UART4_TX_BUF[UART4_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节
void Up_config()
{
	
		FramReadInduce(BASIC_ADDR-6,Data_numbuf[0],UART4_TX_BUF);		//基本配置数据
			DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[0]);
		delay_ms(send_dely);
		FramReadInduce(LED_CAR_ADDR-6,Data_numbuf[1],UART4_TX_BUF);			//车灯序数据
			DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[1]);
		delay_ms(send_dely);
		FramReadInduce(LED_MAN_ADDR-6,Data_numbuf[2],UART4_TX_BUF);			//人灯序数据
			DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[2]);
		delay_ms(send_dely);
		FramReadInduce(PHASE_ADDR-6,Data_numbuf[3],UART4_TX_BUF);		//相位跟随倒数计时数据
			DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[3]);
		delay_ms(send_dely);
		FramReadInduce(TIME_D_ADDR-6,Data_numbuf[4],UART4_TX_BUF);	//时间调度数据
			DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[4]);
		delay_ms(100);
		FramReadInduce(PLAN_ADDR-6,Data_numbuf[5],UART4_TX_BUF);//方案存储
			DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[5]);
		delay_ms(send_dely);
		FramReadInduce(STAGE_ADDR-6,Data_numbuf[6],UART4_TX_BUF);		//阶段相位数据
			DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[6]);
		delay_ms(send_dely);
		FramReadInduce(PHASE_MARK_ADDR-6,Data_numbuf[7],UART4_TX_BUF);		//相位标识
				DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[7]);
		delay_ms(send_dely);
		FramReadInduce(File_Name_ADDR-6,Data_numbuf[8],UART4_TX_BUF);		//相位标识
				DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[8]);
		delay_ms(send_dely);
		FramReadInduce(WEEK_ADDR-6,Data_numbuf[9],UART4_TX_BUF);		//相位标识
				DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[9]);
		delay_ms(send_dely);
		FramReadInduce(LED_CAR_Name-6,Data_numbuf[10],UART4_TX_BUF);		//相位标识
				DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[10]);
		delay_ms(send_dely);
		FramReadInduce(LED_MAN_Name-6,Data_numbuf[11],UART4_TX_BUF);		//相位标识
				DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[11]);
		delay_ms(send_dely);
		FramReadInduce(STAGE_Name-6,Data_numbuf[12],UART4_TX_BUF);		//相位标识
				DMA_Send_Buff(UART4_TX_BUF,Data_numbuf[12]);
		delay_ms(send_dely);
		
}
//上传日志
void Up_LOG()
{
	u8 logbuff[8];
	u8 i;
	u8 Send_data[10][10]={
											"开机",
											"时间校准",
											"关闭外灯",
											"打开外灯",
											"循环测灯",
											"系统复位",
											"查看日志",
											"清除故障",
											"上传配置",
											"下载方案",
											
											};
	
//	printf("Send_data: %d--%d\n",LOG_ADDR_P,(LOG_ADDR_P-LOG_ADDR));
		for(i=0;i<((LOG_ADDR_P-LOG_ADDR)/8);i++)
	{
		
		FramReadInduce(LOG_ADDR+i*8,8,logbuff);
		printf("第%d条日志--%d年%d月-%d日-星期%d-%d时-%d分-%d秒--%s\n",
		i,logbuff[0],logbuff[1],logbuff[2],logbuff[3],logbuff[4],logbuff[5],logbuff[6],Send_data[logbuff[7]]);    //时间
		
	}
	
	
	
	
}













