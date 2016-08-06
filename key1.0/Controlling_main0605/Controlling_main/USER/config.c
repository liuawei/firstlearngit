#include "config.h"
#include "value_addr.h"
extern Phones_Typedef Phones_Data;
//节点检测
u8 Node_test()
{
	u8 i,lage;
	u8 Node_max;
	u8 Sendbuf[2];
	Sendbuf[0] = 0;
	Node_max=0xd0|(BASIC_CONF[Phase_num]/4); // 根据相位总数计算节点个数
	
		for(i=0xd0;i<Node_max;i++)
	{	
		Sendbuf[0] = 0xd0;
		RS485_Write_Data(USART3,Drive_addr,Sendbuf,2);
		RS485_CNT[2]=0;		//清零	
		RS485_BUF[2][0]=0;
		delay_ms(10);
		if(RS485_BUF[1][0]==i)
			lage+=1;
		
	}
	printf("Node_test---%d",lage);
		if (lage!=BASIC_CONF[Phase_num]/4)
			i=1;
		else i=0;
	return i;
			
}

u32 Car_no;						//人灯位置
//u8 STAGE_COMM[128];
u32 STAGE_32_CONF[32];			//阶段相位32位内容
u8 Countdown[10][3];					//倒计时数据
//读取配置数据到缓存
void Read_config()
{
	
	if(FramRead16Byte(LOG_ADDR-2)==0xffff)
	{
		printf("the first time\n");
		FramWrite16Byte(LOG_ADDR-2,LOG_ADDR);
	} 
	LOG_ADDR_P = FramRead16Byte(LOG_ADDR-2);					//读日志
	FramReadInduce(BASIC_ADDR,50,BASIC_CONF);		//基本配置数据
	Data_numbuf[0] = FramRead16Byte(BASIC_ADDR-5);
	
	FramReadInduce(LED_CAR_ADDR,50,LED_CAR_CONF);			//车灯序数据
	Data_numbuf[1] = FramRead16Byte(LED_CAR_ADDR-5);
	
	FramReadInduce(LED_MAN_ADDR,50,LED_MAN_CONF);			//人灯序数据
	Data_numbuf[2] = FramRead16Byte(LED_MAN_ADDR-5);
	
	FramReadInduce(PHASE_ADDR,50,PHASE_CONF);		//相位跟随倒数计时数据
	Data_numbuf[3] = FramRead16Byte(PHASE_ADDR-5);
	
	Data_numbuf[4] = FramRead16Byte(TIME_D_ADDR-5);
	
	FramReadInduce(TIME_D_ADDR,FramReadByte(TIME_D_ADDR-1),*TIME_D_CONF);	//时间调度数据
	
	Data_numbuf[5] = FramRead16Byte(PLAN_ADDR-5);
	
	FramReadInduce(STAGE_ADDR,FramReadByte(STAGE_ADDR-1),*STAGE_CONF);		//阶段相位数据
	Data_numbuf[6] = FramRead16Byte(STAGE_ADDR-5);
	
	FramReadInduce(PHASE_MARK_ADDR,40,Phase_Mark);		//相位标识
	Data_numbuf[7] = FramRead16Byte(PHASE_MARK_ADDR-5);
	Start_config();
}

void Write_config()
{
	u8 a [50] ={1,0,0,1,4,16,0,1,2,3,4,5,6,7,8,1,2};
	u8 b [400]={0x7f,0x14,0x01,0x01,0x01,0x01,0x00,0x00,0x1e,0x00,0x01,0x00,0x00,
							0x0,0x14,0x01,0x01,0x01,0x01,0x00,0x12,0x08,0x00,0x05,0x00,0x00,
							0x0,0x14,0x01,0x01,0x01,0x01,0x00,0x12,0x09,0x00,0x06,0x00,0x00};
	u8 c [32][4]={
  					0xff,0x00,0x00,0x00,
  					0x00,0xff,0x00,0x00,
  					0x22,0xdd,0x00,0x00,
  					0xdd,0x22,0x00,0x00
		};
//							0x11,0x11,0xff,0xff,
//							0x22,0x22,0xff,0xff,
//							0x44,0x44,0xff,0xff,
//							0x88,0x88,0xff,0xff	
//								};
	u8 d [][20]={
				{0x00,0x04,0x0a,0x00,0x0a,0x01,0x0a,0x02,0x0a,0x03,0,0,0,0,0,0,0,0,0,0},
				{0x01,0x04,0x15,0x00,0x10,0x01,0x0e,0x02,0x25,0x03,0,0,0,0,0,0,0,0,0,0},
				{0x02,0x04,0x09,0x01,0x05,0x02,0x05,0x03,0x05,0x04,0,0,0,0,0,0,0,0,0,0},};
	u8 LED_CAR[50] = {0x07,0x11,
				0x16,0x63,0x00,0x33,0x83,0x73,   //灯色--时间
				0x13,0x74,0x83,0x13,0x64,0x73,
				0x13,0x74,0x83,0x13,0x64,0x73,
				0x13,0x74,0x83,0x13,0x64,0x73,};
	u8 LED_MAN[50] = {0x07,0x11,
				0x16,0x63,0x00,0x33,0x86,0x00,   //灯色--时间
				0x1a,0x75,0x08,0x1e,0x05,0x03,
				0x1a,0x75,0x08,0x1e,0x05,0x03,
				0x1a,0x75,0x08,0x1e,0x05,0x03};
	u8 m [32] = {0x01,0x03,0x04,0x00,0x11,0x13,0x14,0x10,0x21,0x23,0x24,0x20,0x31,0x33,0x34,0x30,};
	u8 cd[32] = {0x01,0x01,0x00,0x00,0x02,0x02,0x00,0x00,0x03,0x03,0x00,0x00,0x04,0x04,0x00,0x00};
	FramWriteInduce(BASIC_ADDR,50,a);				//基本配置数据
	FramWriteInduce(LED_CAR_ADDR,50,LED_CAR);				//车灯序数据
	FramWriteInduce(LED_MAN_ADDR,50,LED_MAN);				//人灯序数据
	FramWriteInduce(PHASE_ADDR,50,cd);		//相位跟随倒数计时数据
	FramWriteInduce(TIME_D_ADDR,400,b);				//时间调度数据
	FramWriteInduce(STAGE_ADDR,200,*c);				//阶段相位数据地址
	FramWriteInduce(PLAN_ADDR,400,*d);				//方案数据地址
	FramWriteInduce(PHASE_MARK_ADDR,40,m);			//相位标识数据地址
}
	
////配置数据并启动
//u32 Cus_date;    				//自定义日期时间调度序号 每一位代表一个
void Start_config()
{
	u8 i,j;
	memset(STAGE_32_CONF,0,128);
		for(i=0;i<16;i++)
	{
			for(j=0;j<4;j++)
		{
			STAGE_32_CONF[i]|=((0x00000000|STAGE_CONF[i][j])<<((3-j)*8));	
		}					
	}
//	printf("STAGE_32_CONF---%x--%x--%x--%x\n",STAGE_32_CONF[0],STAGE_32_CONF[1],STAGE_32_CONF[2],STAGE_32_CONF[3]);	
	for(i=0;i<32;i++)				//初始化相位结构体
	{
	Phones_Data[i].SW=0xff;
	Phones_Data[i].Color=0;
	Phones_Data[i].Mark=Phase_Mark[i];
	Car_no<<=1;
	if((Phase_Mark[i]&0x0f)!=0)
	Car_no |=0x01;						//车灯位置	
	
	}
	memset(STAGE_COMM,0,32);
	memset(Countdown,0xff,32);
		for(i=0;i<32;i++)				//初始化倒计时
	{
				switch (Phase_Mark[i]&0x0f)
				{
					case 0x01:		Countdown[PHASE_CONF[i]-1][0] = i;
						break;
					case 0x02:		Countdown[PHASE_CONF[i]-1][0] = i;
						break;
					case 0x03:		Countdown[PHASE_CONF[i]-1][1] = i;//printf("Phase_Mark---%x\n",PHASE_CONF[i]);
						break;
					case 0x04:		Countdown[PHASE_CONF[i]-1][2] = i;
						break;
					case 0x05:		Countdown[PHASE_CONF[i]-1][0] = i;
						break;
					case 0x06:		Countdown[PHASE_CONF[i]-1][0] = i;
						break;
					case 0x07:		Countdown[PHASE_CONF[i]-1][1] = i;
						break;
					default:
						break;
				}
	}
	
	printf("TIME_D_CONF---%x---%x\n",TIME_D_CONF[0][0],FramReadByte(TIME_D_ADDR-1));

}


u8 Time_no;						//时间调度号 
u8 Run_plan_data[20];
/*************************************************
函数名：时间调度数解析
参数：
**************************************************/	 
 void Time_ana_start()
{
	
	u8 i;
	u8 Function = 0;
	u8 weekbit=0x01;
	u8 Manualbuff[16];
	TIM3->CR1&=~(1<<0);  //关闭定时器3
	IWDG_Feed();			//喂独立看门狗
	for(i=0;i<32;i++)
	{	
		if (TIME_D_CONF[i][0]&0x80)
			{			
							if(daydiff(&TIME_D_CONF[i][1]))
			//	if(hourdiff(TIME_D_CONF[i][7],TIME_D_CONF[i][8],TIME_D_CONF[i][9]))//是否到运行时间
							{
								IWDG_Feed();			//喂独立看门狗
								Time_no=i;
								FramReadInduce(PLAN_ADDR+(TIME_D_CONF[i][11]*20),20,Run_plan_data);
								Function = TIME_D_CONF[i][10];

								//Bus1_Write_Data(Comm_addr,0x01,Run_plan_data,Run_plan_data[1]*2+1);
								printf("Time_ana_start---2--%d--%d\n",i,Function);		
							}

			}
		else 		
		{
		if(TIME_D_CONF[i][0]&(weekbit<<(TimeValue.week-1)))				//判断当前星期是否设置
					//printf("Run_plan---%x\n",weekbit>>TimeValue.week);
					if(hourdiff(TIME_D_CONF[i][7],TIME_D_CONF[i][8],TIME_D_CONF[i][9]))//是否到运行时间
						{ 	
							IWDG_Feed();			//喂独立看门狗								
							Time_no=i;	
							FramReadInduce(PLAN_ADDR+(TIME_D_CONF[Time_no][11]*20),20,Run_plan_data);
							Function = TIME_D_CONF[i][10];					
							printf("Time_ana_start---1--%d--%d\n",i,Function);												

						}
						}
				

	}
	Function_analysis(Function);
	if(Function==0)
	{
	//没有任何时间调度 进入黄闪
	Phase_cl(0xffffffff,Manualbuff,0x07);
	Send_APhase(Manualbuff);			//发送相位设置
	}
}
//时间调度功能号解析
void Function_analysis(u8 mode)
{
		switch (mode)
		{
			case 0x00:
								 						 
				break;																
			case 0x01:		TIM3->CR1|=0x01;  //使能定时器	//定周期运行
				break;
			case 0x02:																	//无线揽协调
				break;
			case 0x03:																	//进入感应
				break;											
			case 0x04:																//结束感应
				break;										
			case 0x05:		TIM3->CR1|=0x01;  //使能定时器
											if(Start_flag==1)
										{
										Start_flag=2;			Second=0;				
										}							//进入待机
				break;
			case 0x06:		if(Start_flag==2)
										{
										Start_flag=3;			Second=0;	
										 				//结束待机
										}TIM3->CR1|=0x01;
										
				break;
			case 0x07:																	//打开可变标志
				break;
			case 0x08:																	//关闭可变标志			
				break;
			case 0x09:
				break;
			case 0x10:
				break;
			case 0x11:																	//待定
				break;
			case 0x12:
				break;																	
			case 0x13:																	//待定
				break;
			case 0x14:																//待定
				break;																	
			case 0x15:																//待定
				break;
			case 0x16:
				break;
			case 0x17:
				break;
			case 0x18:
				break;
			case 0x19:
				break;
			case 0x20:
				break;
			case 0x21:
				break;
			case 0x22:
				break;
			case 0x23:
				break;
			case 0x24:																	//暗灯
				break;
			case 0x25:																	//时钟同步
				break;
			default:
				break;
		}
		
	
}


