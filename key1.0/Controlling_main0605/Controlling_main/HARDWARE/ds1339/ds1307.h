/******************************************************************************
* @ File name --> ds1307.h
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

#ifndef _ds1307_h_ 
#define _ds1307_h_

/******************************************************************************
                              �ⲿ����ͷ�ļ�
                    Ӧ�õ���ͬ������ͷ�ļ����������޸ļ���                        
******************************************************************************/
#include "sys.h"
//#include "config.h"

/******************************************************************************
                        DS1307 / DS1338оƬѡ��궨��
******************************************************************************/

#define Chip_Type           0   //ѡ��оƬ���ͣ����޸����Ｔ��
                                //1: ������оƬ�ͺ���DS1307
                                //0: ������оƬ�ͺ���DS1338 or DS1338Z

/******************************************************************************
                               DS1307�Ĵ����ṹ����                          
******************************************************************************/

typedef struct
{  
    u8 year;    //��
		u8 month;   //��
		u8 date;    //��
		u8 week;    //����
		u8 hour;    //Сʱ
		u8 minute;  //��
		u8 second;  //��
}Time_Typedef;

extern Time_Typedef TimeValue;  //����ʱ�仺��ָ��
extern Time_Typedef Timeinit;
/******************************************************************************
                                    �����궨��                       
******************************************************************************/

#define DS1307_Write			0xd0	//д����
#define DS1307_Read			0xd1	//����������ã�DS1307_Write + 1��

#define Clear_Register			0x00	//����Ĵ���ֵ�������

#define test_data               0x55    //�������������DS1307�Ƿ�������������ã����Ը�������ֵ

/******************************************************************************
                                  ��ַ�����궨��
******************************************************************************/

#define Address_second          0x00    //��Ĵ�����ַ
#define Address_minute          0x01    //���ӼĴ�����ַ
#define Address_hour            0x02    //Сʱ�Ĵ�����ַ
#define Address_week            0x03    //���ڼĴ�����ַ
#define Address_date            0x04    //�ռĴ�����ַ
#define Address_month           0x05    //�¼Ĵ�����ַ
#define Address_year            0x06    //��Ĵ�����ַ

#define Address_SQWE            0x0f    //Ƶ��������üĴ�����ַ

//RAM��ַ

#define RAM_Address0            0x08
#define RAM_Address1            0x09
#define RAM_Address2            0x0a
#define RAM_Address3            0x0b
#define RAM_Address4            0x0c
#define RAM_Address5            0x0d
#define RAM_Address6            0x0e
#define RAM_Address7            0x0f

#define RAM_Address8            0x10
#define RAM_Address9            0x11
#define RAM_Address10            0x12
#define RAM_Address11            0x13
#define RAM_Address12            0x14
#define RAM_Address13            0x15
#define RAM_Address14            0x16
#define RAM_Address15            0x17
#define RAM_Address16            0x18
#define RAM_Address17            0x19
#define RAM_Address18            0x1a
#define RAM_Address19            0x1b
#define RAM_Address20            0x1c
#define RAM_Address21            0x1d
#define RAM_Address22            0x1e
#define RAM_Address23            0x1f

#define RAM_Address24            0x20
#define RAM_Address25            0x21
#define RAM_Address26            0x22
#define RAM_Address27            0x23
#define RAM_Address28            0x24
#define RAM_Address29            0x25
#define RAM_Address30            0x26
#define RAM_Address31            0x27
#define RAM_Address32            0x28
#define RAM_Address33            0x29
#define RAM_Address34            0x2a
#define RAM_Address35            0x2b
#define RAM_Address36            0x2c
#define RAM_Address37            0x2d
#define RAM_Address38            0x2e
#define RAM_Address39            0x2f

#define RAM_Address40            0x30
#define RAM_Address41            0x31
#define RAM_Address42            0x32
#define RAM_Address43            0x33
#define RAM_Address44            0x34
#define RAM_Address45            0x35
#define RAM_Address46            0x36
#define RAM_Address47            0x37
#define RAM_Address48            0x38
#define RAM_Address49            0x39
#define RAM_Address50            0x3a
#define RAM_Address51            0x3b
#define RAM_Address52            0x3c
#define RAM_Address53            0x3d
#define RAM_Address54            0x3e
#define RAM_Address55            0x3f

/******************************************************************************
                            ʱ�����������Чλ�궨��                 
******************************************************************************/

#define Shield_secondBit			0x7f
#define Shield_minuteBit			0x7f
#define Shield_hourBit				0x3f
#define Shield_weekBit				0x07
#define Shield_dateBit				0x3f
#define Shield_monthBit				0x1f	

/******************************************************************************
                                  ���������                      
******************************************************************************/

#define Control_Chip_Run            (0<<7)
#define Control_Chip_Stop           (1<<7)  //����Ҳֹͣ����

#define Hour_Mode12                 (1<<6)  //12Сʱģʽ
#define Hour_Mode24                 (0<<6)  //24Сʱģʽ

#define Hour_AM                     (0<<5)  //����
#define Hour_PM                     (1<<5)  //����

#define SQWE_OUT_Hight              (1<<7)  //OUTλ���ã�Ƶ�������ʱ������1��SQW/OUT pin���1���������0
#define SQWE_OUT_Low                (0<<7)

#define SQWE_Disable                (0<<4)  //�ر�Ƶ�����
#define SQWE_Enable                 (1<<4)  //��Ƶ�����

#define CLKOUT_f1                   0x00	//���1Hz
#define CLKOUT_f4096                0x01	//���4.096KHz
#define CLKOUT_f8192                0x02	//���8.192KHz
#define CLKOUT_f32768               0x03	//���32.768KHz

//DS1338 and DS1338ZоƬ��Ƶ������Ĵ�������λ

#if Chip_Type==0	//û������ʹ�õ���DS1338����DS1338ZоƬ

	#define OSF_Disable                 (1<<5)  //ֹͣ����
	#define OSF_Enable                  (0<<5)  //��������

#endif

/******************************************************************************
                                �ⲿ���ܺ���
******************************************************************************/

void DS1307_Time_Init(Time_Typedef *TimeVAL);  //ʱ��������ʼ��

void DS1307_Write_Byte(u8 REG_ADD,u8 dat);  //DS1307ĳ�Ĵ���д��һ���ֽ�����

u8 DS1307_Read_Byte(u8 REG_ADD);    //DS1307ĳ�Ĵ�����ȡһ���ֽ�����

void DS1307_Operate_Register(u8 REG_ADD,u8 *pBuff,u8 num,u8 mode);  //DS1307��ʱ�������Ĵ���������д�����ݻ��߶�ȡ����

void DS1307_ReadWrite_Time(u8 mode);    //DS1307��ȡ����д��ʱ����Ϣ

u8 DS1307_Check(void);  //DS1307���Ժû�

void DS1307_RAM_Write_Data(u8* pBuff,u8 WRadd,u8 num);  //DS1307���õ�RAMд���ݲ���

void DS1307_RAM_Read_Data(u8* pBuff,u8 WRadd,u8 num);   //DS1307���õ�RAM�����ݲ���
void Calibration_time(void);
 int _mktime(int year,u8 mon,u8 day);
u8 daydiff( u8 *date1 ); /** yyyy-mm-dd **/
u8 hourdiff(u8 hour,u8 min,u8 sec);
void Erro_Resolve(void);
#endif  /* end ds1307.h */

