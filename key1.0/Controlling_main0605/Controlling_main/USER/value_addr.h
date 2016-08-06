#ifndef __VALUE_ADDR_H
#define __VALUE_ADDR_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//����
//////////////////////////////////////////////////////////////////////////////////

// //�����ṹ��
// typedef struct 
// {
// 	u8 plan_no;
// 	u8 stage_num;
// 	u8 phase[][4];
// } Plan_data;

//���ݰ�LED_CAR_ADDR
extern u8 BASIC_CONF[50];				//������������
extern u8 LED_CAR_CONF[30];				//��������
extern u8 LED_MAN_CONF[30];				//��������
extern u8 PHASE_CONF[50];				//��λ���浹����ʱ����
extern u8 TIME_D_CONF[32][13];			//ʱ���������
extern u8 PLAN_CONF[20][20];				//����������
extern u8 STAGE_CONF[32][4];			//�׶�����
extern u32 STAGE_32_CONF[32];			//�׶���λ32λ����
extern u8 STAGE_COMM[32];			//�׶����ݽ���
extern u8 paln_plag;    				//�������ձ�־	
extern u8 Manual_Phase[8];				//�ֶ���������
extern u8 Phase_Mark[32];				//��λ��ʶ
extern u8 File_Name[60];				//������
extern u16 Data_numbuf[20];				//��������
extern u8 Countdown[10][3];					//����ʱ����
//��������
#define Tmier_dnum 		0				//ʱ���������
#define Logic_in 		1				//�߼�������
#define Jk_num 			2				//��ⴥ������	
#define Stage_l			3				//�׶�����
#define Stage_num		4				//�׶���
#define Phase_num		5				//��λ��
#define Timer_d			6				//ʱ��
#define RPI				7				//���ȼ�
#define Fault			15				//������Ӧ
#define Plan_num		16				//������
extern int Second;							//����
extern u8 Inquiry_Second;							//����

//��������
 #define  LED_CLOR(d)	(d>>4)			//��ɫ
 #define  LED_TIME(d)	(d&0x0f)				//ʱ��
// #define  Car			0				//����ɫ
// #define  Man			50				//�˵�ɫ
#define  Wait_clor		0				//������ɫ
#define  Hz_pwm			1				//����Ƶ�ʺ�ռ�ձ�
#define  Start_pass		2				//����ͨ��Ȩ
#define  End_pass		5				//����ͨ��Ȩ
#define  Sm_Spass		8				//�źŻ��������ͨ��Ȩ
#define  Sm_Epass		11				//�źŻ���������ͨ��Ȩ
#define  Ypass_wait		14				//��ͨ��Ȩ��������������
#define  Npass_wait		17				//��ͨ��Ȩ��������������
#define  Wait_Ypass		20				//�ɴ�����ʽ���ͨ��Ȩ
#define  Wait_Npass		23				//�ɴ�����ʽʧȥͨ��Ȩ



//��λ���浹����ʱ����


//ʱ���������
extern u32 Cus_date;    				//�Զ�������ʱ�������� ÿһλ����һ��
extern u8 Time_no;						//ʱ����Ⱥ� 






//��������
//extern u8 PLAN[66];				//����������
extern u8 Run_plan_data[20];
//�׶�����
extern u32 Car_no;						//�˵�λ��




//ģ���ַ
#define	Power_addr 0x0a
#define	Comm_addr 0x01
#define	Mian_addr 0x02	
#define	Drive_addr 0x03
#define	Coil_addr 0x07	
#define	LCD_addr 0x0e	
// #define		
// #define		


//������־
// 0 δ����  0x01 ��������  0x02  �������ģʽ  0x12 �Ѿ��������ģʽ  0x03 �˳�����ģʽ  0x13�Ѿ��˳�����ģʽ

extern u8 Start_flag;
extern u8 Auto_hand;
//���ϱ�־
extern u8 Erro_flag;

extern u16 LOG_ADDR_P;		//��־��ַָ��
extern u8 LOG_num_P;		//��־����
extern u8 Time_flag;

typedef struct
{
		u8 SW;							//��λ����
		u8 Color;						//��λ��ɫ
		u8 Mark;						//��λλ��	
	  u8 Current;					//��λ����
		u8 Error; 					//��λ����
}Phones_Typedef[32];


#endif

