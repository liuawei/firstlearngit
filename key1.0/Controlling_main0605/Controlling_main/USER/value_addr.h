#ifndef __VALUE_ADDR_H
#define __VALUE_ADDR_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//变量
//////////////////////////////////////////////////////////////////////////////////

// //方案结构体
// typedef struct 
// {
// 	u8 plan_no;
// 	u8 stage_num;
// 	u8 phase[][4];
// } Plan_data;

//数据包LED_CAR_ADDR
extern u8 BASIC_CONF[50];				//基本配置数据
extern u8 LED_CAR_CONF[30];				//灯序数据
extern u8 LED_MAN_CONF[30];				//灯序数据
extern u8 PHASE_CONF[50];				//相位跟随倒数计时数据
extern u8 TIME_D_CONF[32][13];			//时间调度数据
extern u8 PLAN_CONF[20][20];				//方案总数据
extern u8 STAGE_CONF[32][4];			//阶段内容
extern u32 STAGE_32_CONF[32];			//阶段相位32位内容
extern u8 STAGE_COMM[32];			//阶段内容接收
extern u8 paln_plag;    				//方案接收标志	
extern u8 Manual_Phase[8];				//手动控制数据
extern u8 Phase_Mark[32];				//相位标识
extern u8 File_Name[60];				//方案名
extern u16 Data_numbuf[20];				//数据总数
extern u8 Countdown[10][3];					//倒计时数据
//基本配置
#define Tmier_dnum 		0				//时间调度总数
#define Logic_in 		1				//逻辑输入数
#define Jk_num 			2				//检测触发器数	
#define Stage_l			3				//阶段流数
#define Stage_num		4				//阶段数
#define Phase_num		5				//相位数
#define Timer_d			6				//时差
#define RPI				7				//优先级
#define Fault			15				//故障响应
#define Plan_num		16				//方案数
extern int Second;							//秒数
extern u8 Inquiry_Second;							//秒数

//灯序数据
 #define  LED_CLOR(d)	(d>>4)			//灯色
 #define  LED_TIME(d)	(d&0x0f)				//时长
// #define  Car			0				//车灯色
// #define  Man			50				//人灯色
#define  Wait_clor		0				//待机灯色
#define  Hz_pwm			1				//灯闪频率和占空比
#define  Start_pass		2				//给出通行权
#define  End_pass		5				//结束通行权
#define  Sm_Spass		8				//信号机启动获得通行权
#define  Sm_Epass		11				//信号机启动结束通行权
#define  Ypass_wait		14				//有通行权的由正常到待机
#define  Npass_wait		17				//无通行权的由正常到待机
#define  Wait_Ypass		20				//由待机方式获得通行权
#define  Wait_Npass		23				//由待机方式失去通行权



//相位跟随倒数计时数据


//时间调度数据
extern u32 Cus_date;    				//自定义日期时间调度序号 每一位代表一个
extern u8 Time_no;						//时间调度号 






//方案数据
//extern u8 PLAN[66];				//方案总数据
extern u8 Run_plan_data[20];
//阶段内容
extern u32 Car_no;						//人灯位置




//模块地址
#define	Power_addr 0x0a
#define	Comm_addr 0x01
#define	Mian_addr 0x02	
#define	Drive_addr 0x03
#define	Coil_addr 0x07	
#define	LCD_addr 0x0e	
// #define		
// #define		


//启动标志
// 0 未启动  0x01 正常运行  0x02  进入待机模式  0x12 已经进入待机模式  0x03 退出待机模式  0x13已经退出待机模式

extern u8 Start_flag;
extern u8 Auto_hand;
//故障标志
extern u8 Erro_flag;

extern u16 LOG_ADDR_P;		//日志地址指针
extern u8 LOG_num_P;		//日志总数
extern u8 Time_flag;

typedef struct
{
		u8 SW;							//相位开关
		u8 Color;						//相位颜色
		u8 Mark;						//相位位置	
	  u8 Current;					//相位电流
		u8 Error; 					//相位故障
}Phones_Typedef[32];


#endif

