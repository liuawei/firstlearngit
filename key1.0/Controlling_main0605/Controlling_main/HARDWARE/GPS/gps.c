//GPS串口数据解码程序
#include "sys.h"
//GPS结构体
typedef struct 
{
	u8 GPS_t[9];			 //UTC时间时区8
	u8 GPS_time[9];          //UTC时间
	u8 GPS_wd[12];           //纬度
	u8 GPS_jd[13];           //经度
	u8 GPS_warn;             //定位警告
	u8 GPS_quality[2];       //定位质量 质量因子(0=没有定位，1=实时GPS，2=差分GPS)
	u8 GPS_status;           //定位状态
	u8 GPS_alt[9];           //海拔
	u8 GPS_sv[3];            //使用卫星
	u8 GPS_speed[10];        //速度
	u8 GPS_date[9];          //UTC日期
}_GPS_obj;					 
_GPS_obj GPS_Type;	//GPS结构体

//GPS结构体
typedef struct 
{
	u8 ReceivingF;
	u8 EndF;
	u8 RMCF;
	u8 GGAF;
	u8 VTGF;
	u8 GSAF;                
	u8 Segment;              //逗号计数
	u8 Bytes_counter;
	u8 Command;
}_GPS_buffer;					 
_GPS_buffer GPS_buf={0,0,0,0,0,0,0,0,0};	//GPS结构体


void GPS_display(void)
{
//	u8 g_timer[6];
	u8 t;
	u8 static temp = 0;
	t = (GPS_Type.GPS_sv[0]-0x30)*10 + GPS_Type.GPS_sv[1]-0x30;
	printf("GPS--%d\n",t);
	if(t>1&&t<20)
	{
		TimeValue.year = (GPS_Type.GPS_date[0]-0x30)*10+(GPS_Type.GPS_date[1]-0x30);//年
		TimeValue.month = (GPS_Type.GPS_date[3]-0x30)*10+(GPS_Type.GPS_date[4]-0x30);//月
		TimeValue.date = (GPS_Type.GPS_date[6]-0x30)*10+(GPS_Type.GPS_date[7]-0x30);//日
		TimeValue.hour = (GPS_Type.GPS_t[0]-0x30)*10+(GPS_Type.GPS_t[1]-0x30);		//时
		TimeValue.minute = (GPS_Type.GPS_t[3]-0x30)*10+(GPS_Type.GPS_t[4]-0x30);		//分
		TimeValue.second = (GPS_Type.GPS_t[6]-0x30)*10+(GPS_Type.GPS_t[7]-0x30);		//秒
		DS1307_ReadWrite_Time(0);
		printf("GPS校准时间: %d年%d月-%d日-星期%d-%d时-%d分-%d秒\n",
		TimeValue.year,TimeValue.month,TimeValue.date,TimeValue.week,TimeValue.hour,TimeValue.minute,TimeValue.second);    //时间

		//RTC_Set(2000+g_timer[0],g_timer[1],g_timer[2],g_timer[3],g_timer[4],g_timer[5]);  //设置时间
		if(temp<3)
		{
			temp++;
			delay_ms(300);
		}	
	}
}
/****************************************************/
//功能:NMEA-183协议解析
//入口:u8 tmp
//返回:void
//全局变更修改:GPS_*,Segment,command,Bytes_counter,NewByteF
/****************************************************/
void GPS(u8 tmp)
{
    if(tmp == '$')//起始标志
    {
        GPS_buf.Command = 0;
        GPS_buf.ReceivingF = 1;
        GPS_buf.Bytes_counter = 0;
        GPS_buf.Segment = 0;               //清空语句段计数器
        return;
    }
    if(GPS_buf.ReceivingF)
    {        
        if(tmp == ',')
        {
            ++GPS_buf.Segment;
            GPS_buf.Bytes_counter = 0;    //清空段字节计数器
            return;
        }
        if(tmp == '*')//收到结束标志
        {
            GPS_buf.ReceivingF = 0;
            GPS_buf.EndF = 1;
            return;
        }
        if(GPS_buf.Segment == 0)
        {
            if(GPS_buf.Bytes_counter == 3) //段0，语句类型判断
            switch(tmp)
            {
                case 'G':GPS_buf.Command = 1;//语句类型 $GPGGA
                        GPS_buf.GGAF = 1;
                        break;
                case 'M':GPS_buf.Command = 2;//语句类型 $GPRMC
                        GPS_buf.RMCF = 1;
                        break;
                case 'T':GPS_buf.Command = 3;//语句类型 $GPVTG
                        GPS_buf.VTGF = 1;
                        break;
                case 'S':break;
                default:GPS_buf.Command = 0; //非有效数据类型，终止当前数据接收
                        GPS_buf.ReceivingF = 0;
                        break;
            }
            if(GPS_buf.Bytes_counter == 4)
                if(GPS_buf.Command==0 && tmp=='A')
                {
                    GPS_buf.Command = 4;//语句类型 $GPGSA
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
            case 1:    if(GPS_buf.Bytes_counter == 2 || GPS_buf.Bytes_counter == 5)   //$GPGGA段1，UTC时间，hhmmss（时分秒）格式,取前6位 转换为HH:MM:SS格式
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

            case 2:    if(GPS_buf.Bytes_counter == 3)      //$GPGGA 段2处理 纬度ddmm.mmmm（度分）格式
                    {
                        GPS_Type.GPS_wd[GPS_buf.Bytes_counter] = '.'; //接收第二个字节后插入'.'
                        ++GPS_buf.Bytes_counter;
                        GPS_Type.GPS_wd[11] = '\0';
                    }
                    if(GPS_buf.Bytes_counter == 0)
                        ++GPS_buf.Bytes_counter;
                    GPS_Type.GPS_wd[GPS_buf.Bytes_counter] = tmp;      
                    break;

            case 3:    GPS_Type.GPS_wd[0] = tmp;     //$GPGGA第3段处理 纬度半球N（北半球）或S（南半球）
                    break;

            case 4:    if(GPS_buf.Bytes_counter == 4)    //$GPGGA 段4处理 经度dddmm.mmmm（度分）格式
                    {
                        GPS_Type.GPS_jd[GPS_buf.Bytes_counter] = '.';  //接收第3个字节后插入'.'
                        ++GPS_buf.Bytes_counter;
                        GPS_Type.GPS_jd[12] = '\0';
                    }
                    if(GPS_buf.Bytes_counter == 0)
                        ++GPS_buf.Bytes_counter;
                    GPS_Type.GPS_jd[GPS_buf.Bytes_counter] = tmp;      
                    break;
        

            case 5:    GPS_Type.GPS_jd[0] = tmp; //$GPGGA第5段处理 经度半球E（东经）或W（西经）
                    break;
                        
            case 6: GPS_Type.GPS_quality[0] = tmp;
					GPS_Type.GPS_quality[1] = '\0';
                    break;
                                                                    
            case 7:    if(GPS_buf.Bytes_counter<2)  //$GPGGA第7段处理  正在使用解算位置的卫星数量（00~12）（前面的0也将被传输）
                        GPS_Type.GPS_sv[GPS_buf.Bytes_counter] = tmp;
                    GPS_Type.GPS_sv[2] = '\0';     
                      break;
                                            
            case 9:    if(GPS_buf.Bytes_counter<7)  //$GPGGA第9段处理 海拔高度（-9999.9~99999.9）
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
                                    
        case 9:    if(GPS_buf.Bytes_counter<2)   //$GPRMC第9段处理 UTC日期，ddmmyy（日月年）格式转换为yy-mm-dd
                {
                    GPS_Type.GPS_date[6+GPS_buf.Bytes_counter] = tmp;
                }
                if(GPS_buf.Bytes_counter>1 && GPS_buf.Bytes_counter<4)//月
                {
                    GPS_Type.GPS_date[1+GPS_buf.Bytes_counter] = tmp;
                    GPS_Type.GPS_date[5] = '-';
                }
                if(GPS_buf.Bytes_counter>3 && GPS_buf.Bytes_counter<6)//年
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
        case 2:    GPS_Type.GPS_status = tmp;  //$GPGSA第2段处理 定位类型，1=没有定位，2=2D定位，3=3D定位
                break;
        default:break;
    }
}

void GPVTG(u8 tmp)
{
    switch(GPS_buf.Segment)
    {
        case 7:if(GPS_buf.Bytes_counter < 7)//地面速率（0000.0~1851.8公里/小时，前面的0也将被传输）
               {
                    GPS_Type.GPS_speed[GPS_buf.Bytes_counter] = tmp;
                    GPS_Type.GPS_speed[GPS_buf.Bytes_counter+1] = 'K';//这里4个字符是Km/h
                    GPS_Type.GPS_speed[GPS_buf.Bytes_counter+2] = 'm';
                    GPS_Type.GPS_speed[GPS_buf.Bytes_counter+3] = '/';
                    GPS_Type.GPS_speed[GPS_buf.Bytes_counter+4] = 'h';
                    GPS_Type.GPS_speed[GPS_buf.Bytes_counter+5] = '\0';
                }
                break;
        default:break;
    }
}







