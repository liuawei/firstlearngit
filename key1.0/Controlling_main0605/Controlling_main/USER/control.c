#include "sys.h"


//u8 Phase_buff[32]={0,0,0,0,0,0,0};
Phones_Typedef Phones_Data;

//相位转换灯色
void Phase_cl(u32 Ph,u8 *Ph_cl,u8 cl)
{
	u8 i;
	for(i=0;i<BASIC_CONF[Phase_num];i++)
	{		//printf("Phones_Data--%x\n",Phones_Data[i].SW);
		if((Ph<<i)&0x80000000)
			Ph_cl[i]= cl&Phones_Data[i+16].SW;
		
	}
}
//阶段相位整合
u16 Buff_bit(u8 Stage_no)
{
	u16 Phbit=0;
	Phbit |= (Phbit|STAGE_CONF[Stage_no][0])<<8;
 	Phbit |= (Phbit|STAGE_CONF[Stage_no][1])<<0;
// 	Phbit = (Phbit<<8)||(*STAGE_CONF[Stage_no]+2);
// 	Phbit = (Phbit<<8)||(*STAGE_CONF[Stage_no]+3);
//	printf("Buff_bit--%x--%x--%x\n",*STAGE_CONF[Stage_no],STAGE_CONF[Stage_no][1],Phbit);
	return Phbit;
}

u8 Auto_hand=0x1a;
//相位运行
//返回阶段运行剩余时间
u8 Phase_run(u8 Run_num,u8 Stage_no,u8 R_G_Y)
{
	u8 st=0;
	u8 buff[2];
	buff[0] = Stage_no;
	buff[1] = Run_num;
//	Run_num =Run_num-Second;~Buff_bit(Stage_no)
	
	if(Second==1)
 	{
		
		Phase_cl(~STAGE_32_CONF[Stage_no],&Phones_Data->Color,0x01);   
		Phase_cl(STAGE_32_CONF[Stage_no],&Phones_Data->Color,0x03);
		Send_APhase(&Phones_Data->Color);			//发送相位设置	
		Bus1_Write_Data(Comm_addr,0x05,buff,2);
		delay_ms(10);
		Bus1_Write_Data(Comm_addr,0x05,buff,2);
//		Send_Phase_State(Phase_buff);
		 //printf("Phase_run--%d--%s\n",Run_num-Second,&Phones_Data->Color);	  
	}
	
	if((Run_num+1)==Second)
	{
//		printf("Phase_run--%d--%d\n",Second,Stage_no);
		st=1;
		Second=0;
		
		
	}
	
	
	return st;
}

u32 Phase_ch = 0x00000000;   //启动后的相位状态
/*************************************************
函数名：相位状态切换解析
参数：
Stage_no  阶段号
car_man   人灯位置
Pass_cl 获得通行权灯色
End_cl 失去通行权灯色
**************************************************/	 

void Status_Pass(u8 Stage_no,u32 car_man,u8 Pass_cl)
{
	u32 Pass;
	Pass = (~Phase_ch) & STAGE_32_CONF[Stage_no];
	Phase_cl(Pass&car_man,&Phones_Data->Color,Pass_cl);
//	printf("Pass---%x--%d\n",STAGE_32_CONF[Stage_no],Stage_no);
//	Send_APhase(&Phones_Data->Color);			//发送相位设置
//	Send_Phase_State(&Phones_Data->Color);	
	
}	
void Status_End(u8 Stage_no,u32 car_man,u8 End_cl)
{
	u32 End;
	End = (~STAGE_32_CONF[Stage_no]) & Phase_ch;
	Phase_cl(End&car_man,&Phones_Data->Color,End_cl);
//	printf("End---%x--%d\n",STAGE_32_CONF[Stage_no],Stage_no);
//	Send_APhase(&Phones_Data->Color);			//发送相位设置
//	Send_Phase_State(&Phones_Data->Color);	
	
}
/*************************************************
函数名：灯序解析
参数：
Stage_no  阶段号
car_man   人灯位置
Pass_cl 获得通行权灯色
End_cl 失去通行权灯色
**************************************************/	 

u8 Phase_change(u8 Stage_no,u8 Start,u8 End,u32 Car_Man)
{
	
	u8 st=0;
	u8 num_time =LED_TIME(LED_CAR_CONF[Start])+LED_TIME(LED_CAR_CONF[Start+1])+LED_TIME(LED_CAR_CONF[Start+2])-1;//运行间隔时间
//	Auto_hand=0;
	//获得通行权
	if(Second==1)													//第一步车灯
		Status_Pass(Stage_no,Car_Man,LED_CLOR(LED_CAR_CONF[Start]));
	if(Second==(LED_TIME(LED_CAR_CONF[Start])))								//第二步车灯	
	Status_Pass(Stage_no,Car_Man,LED_CLOR(LED_CAR_CONF[Start+1]));
	if(Second==(LED_TIME(LED_CAR_CONF[Start])+LED_TIME(LED_CAR_CONF[Start+1])))		//第三步车灯
		Status_Pass(Stage_no,Car_Man,LED_CLOR(LED_CAR_CONF[Start+2]));	
	if(Second==1)													//第一步人灯
		Status_Pass(Stage_no,~Car_Man,LED_CLOR(LED_MAN_CONF[Start]));
	if(Second==(LED_TIME(LED_MAN_CONF[Start])))								//第二步人灯
	Status_Pass(Stage_no,~Car_Man,LED_CLOR(LED_MAN_CONF[Start+1]));
	if(Second==(LED_TIME(LED_MAN_CONF[Start])+LED_TIME(LED_MAN_CONF[Start+1])))		//第三步人灯
		Status_Pass(Stage_no,~Car_Man,LED_CLOR(LED_MAN_CONF[Start+2]));	
//失去通行权
	if(Second==1)													//第一步车灯
		Status_End(Stage_no,Car_Man,LED_CLOR(LED_CAR_CONF[End]));
	if(Second==(LED_TIME(LED_CAR_CONF[End])))								//第二步车灯
		Status_End(Stage_no,Car_Man,LED_CLOR(LED_CAR_CONF[End+1]));		
	if(Second==(LED_TIME(LED_CAR_CONF[End])+LED_TIME(LED_CAR_CONF[End+1])))			//第三步车灯	
	Status_End(Stage_no,Car_Man,LED_CLOR(LED_CAR_CONF[End+2]));
	if(Second==1)													//第一步人灯
		Status_End(Stage_no,~Car_Man,LED_CLOR(LED_MAN_CONF[End]));
	if(Second==(LED_TIME(LED_MAN_CONF[End])))								//第二步人灯
		Status_End(Stage_no,~Car_Man,LED_CLOR(LED_MAN_CONF[End+1]));		
	if(Second==(LED_TIME(LED_MAN_CONF[End])+LED_TIME(LED_MAN_CONF[End+1])))			//第三步人灯
	Status_End(Stage_no,~Car_Man,LED_CLOR(LED_MAN_CONF[End+2]));
	Send_APhase(&Phones_Data->Color);			//发送相位设置
//	printf("Phase_run--%d--%d\n",LED_TIME(LED_CAR_CONF[Start]),LED_TIME(LED_CAR_CONF[Start+1]));
	if(num_time == Second)
	{
		Phase_ch = STAGE_32_CONF[Stage_no];
		st=1;
		Second=0;
	}
	Inquiry_Second = 0;
	return st;

}
u8 comm_start=0;
u8 Start_flag = 0;
u8 Stage=0;
/*************************************************
函数名：方案运行
参数：时间调度号
**************************************************/	 
void Run_plan()
{
		u8 Manualbuff[16];
		IWDG_Feed();			//喂独立看门狗		
		Auto_hand=0x1a;
		if(Start_flag==0)			//开机运行
		{
			Phase_ch = ~STAGE_32_CONF[0];
			Start_flag = Phase_change(0,Sm_Spass,Sm_Epass,Car_no);
			comm_start = Start_flag;
		}
		if(Start_flag==1)	
		{

					
					if ((Stage)%2)			
					{	
						if(Run_plan_data[Stage+4]==Run_plan_data[1])
							Run_plan_data[Stage+4] = 0;
								Stage += Phase_change(Run_plan_data[Stage+4],Start_pass,End_pass,Car_no);		
						
						//运行切换灯序
					}
					else
					{			
								Stage += Phase_run(Run_plan_data[Stage+2],Run_plan_data[Stage+3],0);	
						
						//运行正常相位
					}		
					if(Stage/2>=Run_plan_data[1])						//判断是否一个周期运行完成
					{

						//	  重新计算时间调度						
						Time_ana_start();
						Stage = 0;				
						Second = 0;
						comm_start=1;		
					}		
					
					if(comm_start)	
						{																							
							while(Ccomm_board_data());					//给通信板发送配置
							comm_start=0;	
						}		
								
		}
		if(Start_flag==2)			
			{
					Phase_ch = ~STAGE_32_CONF[Run_plan_data[Stage+2]];	
				if(Phase_change(Run_plan_data[Stage+2],Ypass_wait,Npass_wait,Car_no))
				{		
						TIM3->CR1&=~(1<<0);  //关闭定时器3		
						Phase_cl(Car_no,Manualbuff,LED_CAR_CONF[0]);
						Phase_cl(~Car_no,Manualbuff,LED_MAN_CONF[0]);
						Send_APhase(Manualbuff);			//发送相位设置
						printf("进入待机模式");
				}
			
			}
			if(Start_flag==3)			
			{
					Stage=0;
					Phase_ch = ~STAGE_32_CONF[0];
				if(Phase_change(0,Wait_Ypass,Wait_Npass,Car_no))
				{
						Start_flag = 1;
						printf("退出待机模式");
				}
			}
}
u8 Stage_Hand_No = 0;
/*************************************************
函数名：手动控制
参数：控制字节
**************************************************/	 
void Manual_Mode(u8 *buff)
{
	u8 Manualbuff[16];
	u8 temp[2];
	Second = 0;
	TIM3->CR1|=0x01;  
	if((buff[6]>=0x11)&&(buff[6]<=0x18))
	{
		Stage_Hand_No = ((buff[6]&0x0f)|0x90);
		Phase_ch = STAGE_32_CONF[Run_plan_data[Stage+2]];		
		Stage = (Stage_Hand_No&0x0f)*2-1;	
	}
			switch (buff[6])
		{
			case 0x10:	//启动
//					if(Auto_hand==1)
					{	printf("启动--%x",Auto_hand);
						
						Ent_Write_Data(0x01,0xcf,&Auto_hand,1);
						
					}
				break;					
			case 0x19:	//手动控制
						//TIM3->CR1&=~(1<<0);  //关闭定时器3	
						printf("手动控制");
						Auto_hand=0x19;  //使能定时器3 
						Ent_Write_Data(0x01,0xcf,&Auto_hand,1);
							
				break;					
			case 0x1a:	//自动控制
						//TIM3->CR1|=0x01;  
						printf("自动控制");
						Auto_hand=0x1a;  //使能定时器3 
						Stage_Hand_No = 0;
						Ent_Write_Data(0x01,0xcf,&Auto_hand,1);
						
				break;					
			case 0x1b://全红
						Stage_Hand_No = 0xb1;
				break;										
			case 0x1c://黄闪
						Stage_Hand_No = 0xc1;	
				break;						
			case 0x1d://步进
						Stage_Hand_No = 0xd1;
				break;					
			case 0x1e://退出
						Stage_Hand_No = 0;
				break;				
			case 0xd0:		

				break;					
			case 0xf0:					

				break;				
			default:	
				break;
		}
		
		 
}

//手动选择阶段
void Stage_Hand_Mode()
{
	u8 Manualbuff[16];

	switch (Stage_Hand_No&0xf0)
		{
			case 0xb0:
				Phase_ch = ~STAGE_32_CONF[Run_plan_data[Stage+2]];	
				if(Phase_change(Run_plan_data[Stage+2],Ypass_wait,Npass_wait,Car_no))
				{			
					TIM3->CR1&=~(1<<0);  //关闭定时器3	
					Stage_Hand_No = 0x1b;
					
					Ent_Write_Data(0x01,0xcf,&Stage_Hand_No,1);
					Stage_Hand_No = 0;
					Phase_cl(0xffffffff,Manualbuff,0x01);
					Send_APhase(Manualbuff);			//发送相位设置
				//	printf("全红");
				
				}
				break;
			case 0xc0:
				Phase_ch = ~STAGE_32_CONF[Run_plan_data[Stage+2]];	
				if(Phase_change(Run_plan_data[Stage+2],Ypass_wait,Npass_wait,Car_no))
				{			
					TIM3->CR1&=~(1<<0);  //关闭定时器3	
					Stage_Hand_No = 0x1c;
					
					Ent_Write_Data(0x01,0xcf,&Stage_Hand_No,1);
					Stage_Hand_No = 0;
					Phase_cl(0xffffffff,Manualbuff,0x07);
					Send_APhase(Manualbuff);			//发送相位设置
				//	printf("黄闪");
				
				}
				break;
			case 0xd0:
				break;
			case 0x90:
				
					if (Phase_change(Run_plan_data[Stage+2],Start_pass,End_pass,Car_no)	)			
				{			Second = 1;
							Stage_Hand_No = (Stage_Hand_No&0x0f)|0x10;
							
							Ent_Write_Data(0x01,0xcf,&Stage_Hand_No,1);
							Stage_Hand_No = 0;
							Phase_run(255,Run_plan_data[Stage+2],0);	
						//	printf("Run_plan_data--%d--%d",Run_plan_data[Stage+2],Stage+2);
							
					//运行正常相位
				}		
				
				break;
			default:
				break;
		}


}








