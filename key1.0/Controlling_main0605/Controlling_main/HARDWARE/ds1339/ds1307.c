/******************************************************************************
* @ File name --> ds1307.c
* @ Author    --> By@ Sam Chan
* @ Version   --> V1.0
* @ Date      --> 11 - 27 - 2013
* @ Brief     --> ʱ��оƬDS1307����
* @           --> ��������������DS1307��DS1338��DS1338ZоƬ
*
* @ Copyright (C) 20**
* @ All rights reserved
*******************************************************************************
*
*                                  File Update
* @ Version   --> V1.
* @ Author    --> By@
* @ Date      --> 
* @ Revise    --> 
*
******************************************************************************/
#include "sys.h"


/******************************************************************************
                                    �������
******************************************************************************/
extern Phones_Typedef Phones_Data;
//Time_Typedef TimeValue;  //����ʱ�仺��ָ��
u8 Time_Buffer[8];	//ʱ���������ݻ���
/******************************************************************************
* Function Name --> DS1307ĳ�Ĵ���д��һ���ֽ�����
* Description   --> none
* Input         --> REG_ADD��Ҫ�����Ĵ�����ַ
*                   dat��Ҫд�������
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void DS1307_Write_Byte(u8 REG_ADD,u8 dat)
{
	IIC_Start();
	if(!(IIC_Write_Byte(DS1307_Write))) //����д������Ӧ��λ
	{
		IIC_Write_Byte(REG_ADD);
		IIC_Write_Byte(dat);
	}
	IIC_Stop();
}
/******************************************************************************
* Function Name --> DS1307ĳ�Ĵ�����ȡһ���ֽ�����
* Description   --> none
* Input         --> REG_ADD��Ҫ�����Ĵ�����ַ
* Output        --> none
* Reaturn       --> ��ȡ������ֵ
******************************************************************************/
u8 DS1307_Read_Byte(u8 REG_ADD)
{
	u8 rcv;

	IIC_Start();
	if(!(IIC_Write_Byte(DS1307_Write))) //����д������Ӧ��λ
	{
        IIC_Write_Byte(REG_ADD);    //����Ҫ�����ļĴ�����ַ
        IIC_Start();    //��������
        IIC_Write_Byte(DS1307_Read);    //���Ͷ�ȡ����
        rcv = IIC_Read_Byte();
        IIC_Ack(0x01); //���ͷ�Ӧ���ź�
	}
	IIC_Stop();
	return rcv;
}


char bcd2bin(char bcd_value)
{ 
  char temp; 
  temp = bcd_value; 
  temp >>= 1; 
  temp &= 0x78; 
  return(temp + (temp >> 2) + (bcd_value & 0x0f)); 
} 


unsigned char bin2bcd(unsigned char value) 
{ 
  char retval; 
  retval = 0; 

  while(1) 
    { 
        if(value >= 10) 
        { 
         value -= 10; 
         retval += 0x10; 
        } 
        else 
        { 
         retval += value; 
         break; 
        } 
    } 
  return(retval);
}

/******************************************************************************
* Function Name --> DS1307��ʱ�������Ĵ���������д�����ݻ��߶�ȡ����
* Description   --> ����д��n�ֽڻ���������ȡn�ֽ�����
* Input         --> REG_ADD��Ҫ�����Ĵ�����ʼ��ַ
*                   *WBuff��д�����ݻ���
*                   num��д����������
*                   mode������ģʽ��0��д�����ݲ�����1����ȡ���ݲ���
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void DS1307_Operate_Register(u8 REG_ADD,u8 *pBuff,u8 num,u8 mode)
{
	u8 i;
	if(mode)	//��ȡ����
	{
		IIC_Start();
		if(!(IIC_Write_Byte(DS1307_Write)))	//����д������Ӧ��λ
		{
			IIC_Write_Byte(REG_ADD);	//��λ��ʼ�Ĵ�����ַ
			IIC_Start();	//��������
			IIC_Write_Byte(DS1307_Read);	//���Ͷ�ȡ����
			for(i = 0;i < num;i++)
			{
				*pBuff = bcd2bin(IIC_Read_Byte());	//��ȡ����
				if(i == (num - 1))	IIC_Ack(0x01);	//���ͷ�Ӧ���ź�
				else IIC_Ack(0x00);	//����Ӧ���ź�
				pBuff++;
			}
		}
		IIC_Stop();	
	}
	else	//д������
	{		 	
		IIC_Start();
		if(!(IIC_Write_Byte(DS1307_Write)))	//����д������Ӧ��λ
		{
			IIC_Write_Byte(REG_ADD);	//��λ��ʼ�Ĵ�����ַ
			for(i = 0;i < num;i++)
			{
				IIC_Write_Byte(bin2bcd(*pBuff));	//д������
				pBuff++;
			}
		}
		IIC_Stop();
	}
}
u8 Read_reg(u8 add,u8 reg)
{
		u8 vluae;
//		IIC_Start();
//		if(!(IIC_Write_Byte(DS1307_Write)))	//����д������Ӧ��λ
//		{
//			IIC_Write_Byte(add);	//��λ��ʼ�Ĵ�����ַ

//				IIC_Write_Byte(bin2bcd(reg));	//д������

//		}
//		IIC_Stop();
			IIC_Start();
		if(!(IIC_Write_Byte(DS1307_Write)))	//����д������Ӧ��λ
		{
			IIC_Write_Byte(add);	//��λ��ʼ�Ĵ�����ַ
			IIC_Start();	//��������
			IIC_Write_Byte(DS1307_Read);	//���Ͷ�ȡ����
			vluae = bcd2bin(IIC_Read_Byte());	//��ȡ����
				IIC_Ack(0x00);	//����Ӧ���ź�
		}
		IIC_Stop();	
	return vluae;

}
/******************************************************************************
* Function Name --> DS1307��ȡ����д��ʱ����Ϣ
* Description   --> ����д��n�ֽڻ���������ȡn�ֽ�����
* Input         --> *pBuff��д�����ݻ���
*                   mode������ģʽ��0��д�����ݲ�����1����ȡ���ݲ���
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void DS1307_ReadWrite_Time(u8 mode)
{
	u8 Time_Register[8];	//����ʱ�仺��
	
	if(mode)	//��ȡʱ����Ϣ
	{
		DS1307_Operate_Register(Address_second,Time_Register,7,1);	//�����ַ��0x00����ʼ��ȡʱ����������
		
		/******�����ݸ��Ƶ�ʱ��ṹ���У��������������******/
		TimeValue.second = Time_Register[0] & Shield_secondBit;	//������
		TimeValue.minute = Time_Register[1] & Shield_minuteBit;	//��������
		TimeValue.hour = Time_Register[2] & Shield_hourBit;	//Сʱ����
		TimeValue.week = Time_Register[3] & Shield_weekBit;	//��������
		TimeValue.date = Time_Register[4] & Shield_dateBit;	//������
		TimeValue.month = Time_Register[5] & Shield_monthBit;	//������
		TimeValue.year = Time_Register[6];	//������
	}
	else
	{
		/******��ʱ��ṹ���и������ݽ���******/
		Time_Register[0] = TimeValue.second | Control_Chip_Run;	//�룬����оƬ
		Time_Register[1] = TimeValue.minute;	//����
		Time_Register[2] = TimeValue.hour | Hour_Mode24;	//Сʱ��24Сʱ��
		Time_Register[3] = TimeValue.week;	//����
		Time_Register[4] = TimeValue.date;	//��		
		Time_Register[5] = TimeValue.month;	//��
		Time_Register[6] = TimeValue.year;	//��
		
		DS1307_Operate_Register(Address_second,Time_Register,7,0);	//�����ַ��0x00����ʼд��ʱ����������
	}
}
/******************************************************************************
* Function Name --> DS1307���Ժû�
* Description   --> ��DS1307оƬ��RAM�����һ����ַд��һ�����ݲ��������ж�
*                   ���ϴ�д���ֵ��ȣ����ǵ�һ���ϵ磬�������ʼ��ʱ��
* Input         --> none
* Output        --> none
* Reaturn       --> 0���豸���������ǵ�һ���ϵ�
*                   1���豸�����������
******************************************************************************/	
u8 DS1307_Check(void)
{
	if(DS1307_Read_Byte(RAM_Address55) == test_data)    return 0;    //�豸���������ǵ�һ���ϵ�
	else    return 1;
}
/******************************************************************************
* Function Name --> ʱ��������ʼ��
* Description   --> none
* Input         --> *TimeVAL��RTCоƬ�Ĵ���ֵָ��
* Output        --> none
* Reaturn       --> none
******************************************************************************/
Time_Typedef TimeValue={11,18,17,
						2,26,11,15};
void DS1307_Time_Init(Time_Typedef *TimeVAL)
{	
//	
	DS1307_ReadWrite_Time(1);	
//	printf("DS1307_ReadWrite_Time---%x\n",Read_reg(0x0e,3));

	if(TimeValue.year <2)
		{
			TimeValue.year = 15;
			TimeValue.month = 5;
			TimeValue.date = 1;
			TimeValue.week = 5;
			TimeValue.hour = 12;
			TimeValue.minute = 1;
			TimeValue.second = 1;
			DS1307_ReadWrite_Time(0);
		}
		Log_Write(0);				//д��־
//			printf("ʱ��: %d��%d��-%d��-����%d-%dʱ-%d��-%d��\n",
//			TimeValue.year,TimeValue.month,TimeValue.date,TimeValue.week,TimeValue.hour,TimeValue.minute,TimeValue.second);    //ʱ��
//	
}


//�����2000�꿪ʼ��Ҫ����ʱ�������char *slTime 
 int _mktime(int year,u8 mon,u8 day) /** yyyy-mm-dd **/
{
    struct tm tm_t;
	struct tm start={100,0,1,0,0,0,0,0,0};
//     int year;
//     int mon;
//     int day;
// 	int hour;
// 	int min;
// 	int sec;
 	time_t diff;
//    sscanf( slTime, "%4d%2d%2d%2d%2d%2d", &year, &mon, &day, &hour, &min, &sec );
	year +=2000;
    tm_t.tm_year = year - 1900;
    tm_t.tm_mon = mon - 1;
    tm_t.tm_mday = day;
    tm_t.tm_hour = 0;
    tm_t.tm_min = 0;
    tm_t.tm_sec = 0;
    tm_t.tm_wday = 0;
    tm_t.tm_yday = 0;
    tm_t.tm_isdst = 0;
	diff = abs( mktime( &tm_t ) - mktime( &start ) );
	//diff = difftime(mktime( &tm_t ),mktime( &start ));
    return (int)diff;
}


/*************************************************
���������жϵ�ǰʱ���Ƿ���ʱ����
���������� 1  ��   0  ��  
**************************************************/	 //

u8 daydiff( u8 *date1)
{
	u8 temp[3];
	u8 flg;
	DS1307_ReadWrite_Time(1);
	temp[0] = TimeValue.year;
	temp[1] = TimeValue.month;
	temp[2] = TimeValue.date;
	//��ʽ�����ݵ��ַ���
// 	sprintf(temp,"20""%2d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d",
// 	TimeValue.year,TimeValue.month/10,TimeValue.month%10,TimeValue.date/10,TimeValue.date%10,
// 	TimeValue.hour/10,TimeValue.hour%10,TimeValue.minute/10,TimeValue.minute%10,TimeValue.second/10,TimeValue.second%10);

// 	printf("�ַ���%s\n",temp );
// 	printf("�ж�:%d\n",_mktime( date1 ));
// 	printf("�ж�:%d\n",_mktime( temp ));
// 	printf("�ж�:%d\n",_mktime( date2 ));
	
	if((_mktime( *(date1),*(date1+1),*(date1+2) )<_mktime(  temp[0],temp[1],temp[2] ))&&(_mktime(  temp[0],temp[1],temp[2] )<_mktime(  *(date1+3),*(date1+4),*(date1+5) )))
			flg = 1;
	else    flg = 0;
    return flg;
}
/*************************************************
���������뵱ǰʱ��Ա�
������
�ȵ�ǰʱ����  ����0	���򷵻�1
**************************************************/	 
u8 hourdiff(u8 hour,u8 min,u8 sec)
{
	u32 now;
	u32 temp;
	u8 diff;
	now = TimeValue.hour*3600+TimeValue.minute*60+TimeValue.second;
//	printf("result: %d\n",now);
	temp = hour*3600+min*60+sec;
//	printf("result: %d\n",temp);
	if (now>=temp)
		diff=1;
	else
		diff=0;
	return  diff;
}





u8 Time_flag;
u8 Erro_flag=0;
void Calibration_time()
{
		static u8 min,sec,hour;
		u8 Power_dat[10];
		if(TimeValue.minute != min)		//ÿ����һ��
		{
		
			min = TimeValue.minute;
			if(Start_flag==0||Start_flag==2)
				Time_ana_start();				//ʱ���ж�				
	
		printf("ʱ��: %d��%d��-%d��-����%d-%dʱ-%d��-%d��\n",
		TimeValue.year,TimeValue.month,TimeValue.date,TimeValue.week,TimeValue.hour,TimeValue.minute,TimeValue.second);    //ʱ��
		Power_board_data(0,Power_dat);
		printf("�¶�=%d.%d ��	��ѹ=%d V\n",Power_dat[0],Power_dat[1],Power_dat[2]*100+Power_dat[3]); 
			GPS_display();			//ÿ����Уʱһ��
		}
		if(TimeValue.second != sec)
		{
			sec = TimeValue.second;
			
			IWDG_Feed();			//ι�������Ź�
			RUN_LED = ~RUN_LED;
//			Erro_Resolve();
		}
				if(TimeValue.hour != hour)
		{
			hour = TimeValue.hour;
			Log_Write(1);				//д��־
		}
			DS1307_ReadWrite_Time(1);

}
u8 STAGE_COMM[32];			//�׶����ݽ���
//���Ͻ���
void Erro_Resolve()
{
	u8 i=1;
	u8 Sendbuff[16];
			memset(RS485_BUF[2],0,20);
//			if((Inquiry_Second>2)&&(Inquiry_Second<7))
			if(Inquiry_Second==2)
			{
//				i=Inquiry_Second-3;
				for(i=0;i<4;i++)
				{
				delay_ms(10);	memset(RS485_BUF[2],0,20);
				Bus2_Write_Data(Drive_addr+i,0x01,0,1);
//					printf("Drive_addr==%x",Drive_addr+i);
					UART3_RX_STA=0;
				while(!(UART3_RX_STA&0x8000));
				printf("Send_Phase==%x==%x==%x==%x==%x==%x\n",
					RS485_BUF[2][0],RS485_BUF[2][1],RS485_BUF[2][2],RS485_BUF[2][3],RS485_BUF[2][4],RS485_BUF[2][5]);
	

				if(RS485_BUF[2][1]==0xbb)		
				{
					Erro_flag = 1;
					Phones_Data[RS485_BUF[2][0]*4+16].Error = RS485_BUF[2][2];
					Phones_Data[RS485_BUF[2][0]*4+1+16].Error= RS485_BUF[2][3];
					Phones_Data[RS485_BUF[2][0]*4+2+16].Error = RS485_BUF[2][4];
					Phones_Data[RS485_BUF[2][0]*4+3+16].Error = RS485_BUF[2][5];
				}
				if(RS485_BUF[2][1]==0xee)	
					{	
					Phones_Data[RS485_BUF[2][0]*4+16].Error = RS485_BUF[2][2];
					Phones_Data[RS485_BUF[2][0]*4+1+16].Error = RS485_BUF[2][3];
					Phones_Data[RS485_BUF[2][0]*4+2+16].Error = RS485_BUF[2][4];
					Phones_Data[RS485_BUF[2][0]*4+3+16].Error = RS485_BUF[2][5];				
					Erro_flag=2;break;
					}

				}

			}
				
			if(Erro_flag == 1)
			{
							for(i=16;i<32;i++)
				{	
							if((Phones_Data[i].Error==0x02)||(Phones_Data[i].Error==0x01))
								Phones_Data[i].SW=0;
				}
			}				
			if(Erro_flag == 2)
			{
				Inquiry_Second = 0;
			TIM3->CR1&=~(1<<0);  //�رն�ʱ��3	
				Phase_cl(0xffffffff,Sendbuff,0);
						Send_APhase(Sendbuff);			//������λ����
			}

}



