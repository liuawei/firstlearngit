/******************************************************************************
* @ File name --> ds1307.c
* @ Author    --> By@ Sam Chan
* @ Version   --> V1.0
* @ Date      --> 11 - 27 - 2013
* @ Brief     --> 时钟芯片DS1307驱动
* @           --> 本驱动函数兼容DS1307、DS1338、DS1338Z芯片
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
                                    定义变量
******************************************************************************/
extern Phones_Typedef Phones_Data;
//Time_Typedef TimeValue;  //定义时间缓存指针
u8 Time_Buffer[8];	//时间日历数据缓存
/******************************************************************************
* Function Name --> DS1307某寄存器写入一个字节数据
* Description   --> none
* Input         --> REG_ADD：要操作寄存器地址
*                   dat：要写入的数据
* Output        --> none
* Reaturn       --> none 
******************************************************************************/
void DS1307_Write_Byte(u8 REG_ADD,u8 dat)
{
	IIC_Start();
	if(!(IIC_Write_Byte(DS1307_Write))) //发送写命令并检查应答位
	{
		IIC_Write_Byte(REG_ADD);
		IIC_Write_Byte(dat);
	}
	IIC_Stop();
}
/******************************************************************************
* Function Name --> DS1307某寄存器读取一个字节数据
* Description   --> none
* Input         --> REG_ADD：要操作寄存器地址
* Output        --> none
* Reaturn       --> 读取到的数值
******************************************************************************/
u8 DS1307_Read_Byte(u8 REG_ADD)
{
	u8 rcv;

	IIC_Start();
	if(!(IIC_Write_Byte(DS1307_Write))) //发送写命令并检查应答位
	{
        IIC_Write_Byte(REG_ADD);    //发送要操作的寄存器地址
        IIC_Start();    //重启总线
        IIC_Write_Byte(DS1307_Read);    //发送读取命令
        rcv = IIC_Read_Byte();
        IIC_Ack(0x01); //发送非应答信号
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
* Function Name --> DS1307对时间日历寄存器操作，写入数据或者读取数据
* Description   --> 连续写入n字节或者连续读取n字节数据
* Input         --> REG_ADD：要操作寄存器起始地址
*                   *WBuff：写入数据缓存
*                   num：写入数据数量
*                   mode：操作模式。0：写入数据操作。1：读取数据操作
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void DS1307_Operate_Register(u8 REG_ADD,u8 *pBuff,u8 num,u8 mode)
{
	u8 i;
	if(mode)	//读取数据
	{
		IIC_Start();
		if(!(IIC_Write_Byte(DS1307_Write)))	//发送写命令并检查应答位
		{
			IIC_Write_Byte(REG_ADD);	//定位起始寄存器地址
			IIC_Start();	//重启总线
			IIC_Write_Byte(DS1307_Read);	//发送读取命令
			for(i = 0;i < num;i++)
			{
				*pBuff = bcd2bin(IIC_Read_Byte());	//读取数据
				if(i == (num - 1))	IIC_Ack(0x01);	//发送非应答信号
				else IIC_Ack(0x00);	//发送应答信号
				pBuff++;
			}
		}
		IIC_Stop();	
	}
	else	//写入数据
	{		 	
		IIC_Start();
		if(!(IIC_Write_Byte(DS1307_Write)))	//发送写命令并检查应答位
		{
			IIC_Write_Byte(REG_ADD);	//定位起始寄存器地址
			for(i = 0;i < num;i++)
			{
				IIC_Write_Byte(bin2bcd(*pBuff));	//写入数据
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
//		if(!(IIC_Write_Byte(DS1307_Write)))	//发送写命令并检查应答位
//		{
//			IIC_Write_Byte(add);	//定位起始寄存器地址

//				IIC_Write_Byte(bin2bcd(reg));	//写入数据

//		}
//		IIC_Stop();
			IIC_Start();
		if(!(IIC_Write_Byte(DS1307_Write)))	//发送写命令并检查应答位
		{
			IIC_Write_Byte(add);	//定位起始寄存器地址
			IIC_Start();	//重启总线
			IIC_Write_Byte(DS1307_Read);	//发送读取命令
			vluae = bcd2bin(IIC_Read_Byte());	//读取数据
				IIC_Ack(0x00);	//发送应答信号
		}
		IIC_Stop();	
	return vluae;

}
/******************************************************************************
* Function Name --> DS1307读取或者写入时间信息
* Description   --> 连续写入n字节或者连续读取n字节数据
* Input         --> *pBuff：写入数据缓存
*                   mode：操作模式。0：写入数据操作。1：读取数据操作
* Output        --> none
* Reaturn       --> none
******************************************************************************/
void DS1307_ReadWrite_Time(u8 mode)
{
	u8 Time_Register[8];	//定义时间缓存
	
	if(mode)	//读取时间信息
	{
		DS1307_Operate_Register(Address_second,Time_Register,7,1);	//从秒地址（0x00）开始读取时间日历数据
		
		/******将数据复制到时间结构体中，方便后面程序调用******/
		TimeValue.second = Time_Register[0] & Shield_secondBit;	//秒数据
		TimeValue.minute = Time_Register[1] & Shield_minuteBit;	//分钟数据
		TimeValue.hour = Time_Register[2] & Shield_hourBit;	//小时数据
		TimeValue.week = Time_Register[3] & Shield_weekBit;	//星期数据
		TimeValue.date = Time_Register[4] & Shield_dateBit;	//日数据
		TimeValue.month = Time_Register[5] & Shield_monthBit;	//月数据
		TimeValue.year = Time_Register[6];	//年数据
	}
	else
	{
		/******从时间结构体中复制数据进来******/
		Time_Register[0] = TimeValue.second | Control_Chip_Run;	//秒，启动芯片
		Time_Register[1] = TimeValue.minute;	//分钟
		Time_Register[2] = TimeValue.hour | Hour_Mode24;	//小时，24小时制
		Time_Register[3] = TimeValue.week;	//星期
		Time_Register[4] = TimeValue.date;	//日		
		Time_Register[5] = TimeValue.month;	//月
		Time_Register[6] = TimeValue.year;	//年
		
		DS1307_Operate_Register(Address_second,Time_Register,7,0);	//从秒地址（0x00）开始写入时间日历数据
	}
}
/******************************************************************************
* Function Name --> DS1307测试好坏
* Description   --> 在DS1307芯片的RAM的最后一个地址写入一个数据并读出来判断
*                   与上次写入的值相等，不是第一次上电，否则则初始化时间
* Input         --> none
* Output        --> none
* Reaturn       --> 0：设备正常并不是第一次上电
*                   1：设备错误或者已损坏
******************************************************************************/	
u8 DS1307_Check(void)
{
	if(DS1307_Read_Byte(RAM_Address55) == test_data)    return 0;    //设备正常，不是第一次上电
	else    return 1;
}
/******************************************************************************
* Function Name --> 时间日历初始化
* Description   --> none
* Input         --> *TimeVAL：RTC芯片寄存器值指针
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
		Log_Write(0);				//写日志
//			printf("时间: %d年%d月-%d日-星期%d-%d时-%d分-%d秒\n",
//			TimeValue.year,TimeValue.month,TimeValue.date,TimeValue.week,TimeValue.hour,TimeValue.minute,TimeValue.second);    //时间
//	
}


//计算从2000年开始到要计算时间的秒数char *slTime 
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
函数名：判断当前时间是否在时段内
参数：返回 1  是   0  否  
**************************************************/	 //

u8 daydiff( u8 *date1)
{
	u8 temp[3];
	u8 flg;
	DS1307_ReadWrite_Time(1);
	temp[0] = TimeValue.year;
	temp[1] = TimeValue.month;
	temp[2] = TimeValue.date;
	//格式化数据到字符串
// 	sprintf(temp,"20""%2d%1d%1d%1d%1d%1d%1d%1d%1d%1d%1d",
// 	TimeValue.year,TimeValue.month/10,TimeValue.month%10,TimeValue.date/10,TimeValue.date%10,
// 	TimeValue.hour/10,TimeValue.hour%10,TimeValue.minute/10,TimeValue.minute%10,TimeValue.second/10,TimeValue.second%10);

// 	printf("字符串%s\n",temp );
// 	printf("判断:%d\n",_mktime( date1 ));
// 	printf("判断:%d\n",_mktime( temp ));
// 	printf("判断:%d\n",_mktime( date2 ));
	
	if((_mktime( *(date1),*(date1+1),*(date1+2) )<_mktime(  temp[0],temp[1],temp[2] ))&&(_mktime(  temp[0],temp[1],temp[2] )<_mktime(  *(date1+3),*(date1+4),*(date1+5) )))
			flg = 1;
	else    flg = 0;
    return flg;
}
/*************************************************
函数名：与当前时间对比
参数：
比当前时间早  返回0	否则返回1
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
		if(TimeValue.minute != min)		//每分钟一次
		{
		
			min = TimeValue.minute;
			if(Start_flag==0||Start_flag==2)
				Time_ana_start();				//时段判断				
	
		printf("时间: %d年%d月-%d日-星期%d-%d时-%d分-%d秒\n",
		TimeValue.year,TimeValue.month,TimeValue.date,TimeValue.week,TimeValue.hour,TimeValue.minute,TimeValue.second);    //时间
		Power_board_data(0,Power_dat);
		printf("温度=%d.%d 度	电压=%d V\n",Power_dat[0],Power_dat[1],Power_dat[2]*100+Power_dat[3]); 
			GPS_display();			//每分钟校时一次
		}
		if(TimeValue.second != sec)
		{
			sec = TimeValue.second;
			
			IWDG_Feed();			//喂独立看门狗
			RUN_LED = ~RUN_LED;
//			Erro_Resolve();
		}
				if(TimeValue.hour != hour)
		{
			hour = TimeValue.hour;
			Log_Write(1);				//写日志
		}
			DS1307_ReadWrite_Time(1);

}
u8 STAGE_COMM[32];			//阶段内容接收
//故障解析
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
			TIM3->CR1&=~(1<<0);  //关闭定时器3	
				Phase_cl(0xffffffff,Sendbuff,0);
						Send_APhase(Sendbuff);			//发送相位设置
			}

}



