//GPS�������ݽ������
#include "sys.h"
//GPS�ṹ��
typedef struct 
{
	u8 GPS_t[9];			 //UTCʱ��ʱ��8
	u8 GPS_time[9];          //UTCʱ��
	u8 GPS_wd[12];           //γ��
	u8 GPS_jd[13];           //����
	u8 GPS_warn;             //��λ����
	u8 GPS_quality[2];       //��λ���� ��������(0=û�ж�λ��1=ʵʱGPS��2=���GPS)
	u8 GPS_status;           //��λ״̬
	u8 GPS_alt[9];           //����
	u8 GPS_sv[3];            //ʹ������
	u8 GPS_speed[10];        //�ٶ�
	u8 GPS_date[9];          //UTC����
}_GPS_obj;					 
_GPS_obj GPS_Type;	//GPS�ṹ��

//GPS�ṹ��
typedef struct 
{
	u8 ReceivingF;
	u8 EndF;
	u8 RMCF;
	u8 GGAF;
	u8 VTGF;
	u8 GSAF;                
	u8 Segment;              //���ż���
	u8 Bytes_counter;
	u8 Command;
}_GPS_buffer;					 
_GPS_buffer GPS_buf={0,0,0,0,0,0,0,0,0};	//GPS�ṹ��


void GPS_display(void)
{
//	u8 g_timer[6];
	u8 t;
	u8 static temp = 0;
	t = (GPS_Type.GPS_sv[0]-0x30)*10 + GPS_Type.GPS_sv[1]-0x30;
	printf("GPS--%d\n",t);
	if(t>1&&t<20)
	{
		TimeValue.year = (GPS_Type.GPS_date[0]-0x30)*10+(GPS_Type.GPS_date[1]-0x30);//��
		TimeValue.month = (GPS_Type.GPS_date[3]-0x30)*10+(GPS_Type.GPS_date[4]-0x30);//��
		TimeValue.date = (GPS_Type.GPS_date[6]-0x30)*10+(GPS_Type.GPS_date[7]-0x30);//��
		TimeValue.hour = (GPS_Type.GPS_t[0]-0x30)*10+(GPS_Type.GPS_t[1]-0x30);		//ʱ
		TimeValue.minute = (GPS_Type.GPS_t[3]-0x30)*10+(GPS_Type.GPS_t[4]-0x30);		//��
		TimeValue.second = (GPS_Type.GPS_t[6]-0x30)*10+(GPS_Type.GPS_t[7]-0x30);		//��
		DS1307_ReadWrite_Time(0);
		printf("GPSУ׼ʱ��: %d��%d��-%d��-����%d-%dʱ-%d��-%d��\n",
		TimeValue.year,TimeValue.month,TimeValue.date,TimeValue.week,TimeValue.hour,TimeValue.minute,TimeValue.second);    //ʱ��

		//RTC_Set(2000+g_timer[0],g_timer[1],g_timer[2],g_timer[3],g_timer[4],g_timer[5]);  //����ʱ��
		if(temp<3)
		{
			temp++;
			delay_ms(300);
		}	
	}
}
/****************************************************/
//����:NMEA-183Э�����
//���:u8 tmp
//����:void
//ȫ�ֱ���޸�:GPS_*,Segment,command,Bytes_counter,NewByteF
/****************************************************/
void GPS(u8 tmp)
{
    if(tmp == '$')//��ʼ��־
    {
        GPS_buf.Command = 0;
        GPS_buf.ReceivingF = 1;
        GPS_buf.Bytes_counter = 0;
        GPS_buf.Segment = 0;               //������μ�����
        return;
    }
    if(GPS_buf.ReceivingF)
    {        
        if(tmp == ',')
        {
            ++GPS_buf.Segment;
            GPS_buf.Bytes_counter = 0;    //��ն��ֽڼ�����
            return;
        }
        if(tmp == '*')//�յ�������־
        {
            GPS_buf.ReceivingF = 0;
            GPS_buf.EndF = 1;
            return;
        }
        if(GPS_buf.Segment == 0)
        {
            if(GPS_buf.Bytes_counter == 3) //��0����������ж�
            switch(tmp)
            {
                case 'G':GPS_buf.Command = 1;//������� $GPGGA
                        GPS_buf.GGAF = 1;
                        break;
                case 'M':GPS_buf.Command = 2;//������� $GPRMC
                        GPS_buf.RMCF = 1;
                        break;
                case 'T':GPS_buf.Command = 3;//������� $GPVTG
                        GPS_buf.VTGF = 1;
                        break;
                case 'S':break;
                default:GPS_buf.Command = 0; //����Ч�������ͣ���ֹ��ǰ���ݽ���
                        GPS_buf.ReceivingF = 0;
                        break;
            }
            if(GPS_buf.Bytes_counter == 4)
                if(GPS_buf.Command==0 && tmp=='A')
                {
                    GPS_buf.Command = 4;//������� $GPGSA
                    GPS_buf.GSAF = 1;
                }
         }
         else
         {
             switch(GPS_buf.Command)
            {
                case 1:GPGGA(tmp);
                        break;
                case 2:GPRMC(tmp);
                        break;
                case 3:GPVTG(tmp);
                        break;
                case 4:GPGSA(tmp);
                        break;
                default:break;
            }
        }
         ++GPS_buf.Bytes_counter;     
    }
   // NewByteF = 0;
    return;
}

void GPGGA(u8 tmp)
{   
    s8 Bhour; 
	switch(GPS_buf.Segment)
     {
            case 1:    if(GPS_buf.Bytes_counter == 2 || GPS_buf.Bytes_counter == 5)   //$GPGGA��1��UTCʱ�䣬hhmmss��ʱ���룩��ʽ,ȡǰ6λ ת��ΪHH:MM:SS��ʽ
                    {
                        GPS_Type.GPS_time[GPS_buf.Bytes_counter] = ':';
						GPS_Type.GPS_t[GPS_buf.Bytes_counter] = ':';
                        ++GPS_buf.Bytes_counter;
                    }
                    if(GPS_buf.Bytes_counter<8)
                    {
						GPS_Type.GPS_time[GPS_buf.Bytes_counter] = tmp;
						GPS_Type.GPS_t[GPS_buf.Bytes_counter] = tmp;
					}
					Bhour=((GPS_Type.GPS_time[0]-0x30)*10)+GPS_Type.GPS_time[1]-0x30+TIME_AREA;
					if(Bhour>=24){Bhour=Bhour-24;}
					else {if(Bhour<0)Bhour+=24;}
					GPS_Type.GPS_t[0]=Bhour/10%10+0x30;
					GPS_Type.GPS_t[1]=Bhour%10+0x30;
                    GPS_Type.GPS_time[8] = '\0';
                    break;    

            case 2:    if(GPS_buf.Bytes_counter == 3)      //$GPGGA ��2���� γ��ddmm.mmmm���ȷ֣���ʽ
                    {
                        GPS_Type.GPS_wd[GPS_buf.Bytes_counter] = '.'; //���յڶ����ֽں����'.'
                        ++GPS_buf.Bytes_counter;
                        GPS_Type.GPS_wd[11] = '\0';
                    }
                    if(GPS_buf.Bytes_counter == 0)
                        ++GPS_buf.Bytes_counter;
                    GPS_Type.GPS_wd[GPS_buf.Bytes_counter] = tmp;      
                    break;

            case 3:    GPS_Type.GPS_wd[0] = tmp;     //$GPGGA��3�δ��� γ�Ȱ���N�������򣩻�S���ϰ���
                    break;

            case 4:    if(GPS_buf.Bytes_counter == 4)    //$GPGGA ��4���� ����dddmm.mmmm���ȷ֣���ʽ
                    {
                        GPS_Type.GPS_jd[GPS_buf.Bytes_counter] = '.';  //���յ�3���ֽں����'.'
                        ++GPS_buf.Bytes_counter;
                        GPS_Type.GPS_jd[12] = '\0';
                    }
                    if(GPS_buf.Bytes_counter == 0)
                        ++GPS_buf.Bytes_counter;
                    GPS_Type.GPS_jd[GPS_buf.Bytes_counter] = tmp;      
                    break;
        

            case 5:    GPS_Type.GPS_jd[0] = tmp; //$GPGGA��5�δ��� ���Ȱ���E����������W��������
                    break;
                        
            case 6: GPS_Type.GPS_quality[0] = tmp;
					GPS_Type.GPS_quality[1] = '\0';
                    break;
                                                                    
            case 7:    if(GPS_buf.Bytes_counter<2)  //$GPGGA��7�δ���  ����ʹ�ý���λ�õ�����������00~12����ǰ���0Ҳ�������䣩
                        GPS_Type.GPS_sv[GPS_buf.Bytes_counter] = tmp;
                    GPS_Type.GPS_sv[2] = '\0';     
                      break;
                                            
            case 9:    if(GPS_buf.Bytes_counter<7)  //$GPGGA��9�δ��� ���θ߶ȣ�-9999.9~99999.9��
                    {
                        GPS_Type.GPS_alt[GPS_buf.Bytes_counter] = tmp;
						GPS_Type.GPS_alt[GPS_buf.Bytes_counter+1] = 'm';
                        GPS_Type.GPS_alt[GPS_buf.Bytes_counter+2] = '\0';
                    } 
                    break;

            default:break;
    }
}


void GPRMC(u8 tmp)
{
    switch(GPS_buf.Segment)
    {
                                    
        case 9:    if(GPS_buf.Bytes_counter<2)   //$GPRMC��9�δ��� UTC���ڣ�ddmmyy�������꣩��ʽת��Ϊyy-mm-dd
                {
                    GPS_Type.GPS_date[6+GPS_buf.Bytes_counter] = tmp;
                }
                if(GPS_buf.Bytes_counter>1 && GPS_buf.Bytes_counter<4)//��
                {
                    GPS_Type.GPS_date[1+GPS_buf.Bytes_counter] = tmp;
                    GPS_Type.GPS_date[5] = '-';
                }
                if(GPS_buf.Bytes_counter>3 && GPS_buf.Bytes_counter<6)//��
                {
                    GPS_Type.GPS_date[GPS_buf.Bytes_counter-4] = tmp;
                    GPS_Type.GPS_date[2] = '-';
                    GPS_Type.GPS_date[8] = '\0';
                }
                break;
        default:break;
         }
}


void GPGSA(u8 tmp)
{
    switch(GPS_buf.Segment)
    {
        case 2:    GPS_Type.GPS_status = tmp;  //$GPGSA��2�δ��� ��λ���ͣ�1=û�ж�λ��2=2D��λ��3=3D��λ
                break;
        default:break;
    }
}

void GPVTG(u8 tmp)
{
    switch(GPS_buf.Segment)
    {
        case 7:if(GPS_buf.Bytes_counter < 7)//�������ʣ�0000.0~1851.8����/Сʱ��ǰ���0Ҳ�������䣩
               {
                    GPS_Type.GPS_speed[GPS_buf.Bytes_counter] = tmp;
                    GPS_Type.GPS_speed[GPS_buf.Bytes_counter+1] = 'K';//����4���ַ���Km/h
                    GPS_Type.GPS_speed[GPS_buf.Bytes_counter+2] = 'm';
                    GPS_Type.GPS_speed[GPS_buf.Bytes_counter+3] = '/';
                    GPS_Type.GPS_speed[GPS_buf.Bytes_counter+4] = 'h';
                    GPS_Type.GPS_speed[GPS_buf.Bytes_counter+5] = '\0';
                }
                break;
        default:break;
    }
}







