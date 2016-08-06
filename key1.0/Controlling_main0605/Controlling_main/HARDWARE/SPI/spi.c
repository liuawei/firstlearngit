#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//SPI ��������	   
////////////////////////////////////////////////////////////////////////////////// 	  
 



//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������SD Card/W25X16/							  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
void SPIx_Init(void)
{	 
	
	
//	RCC->APB2ENR|=1<<2;       //PORTAʱ��ʹ��
//	RCC->APB2ENR|=1<<3;       //PORTAʱ��ʹ�� 	
//	RCC->APB2ENR|=1<<4;       //PORTAʱ��ʹ�� 	 
//	GPIOA->CRL&=0X0FFFFFFF; 
//	GPIOA->CRL|=0X30000000;
//// 	PA.7
// 	GPIOB->CRL&=0XFFFFFFF0; 
// 	GPIOB->CRL|=0X00000003;
////PB.0	
//	GPIOC->CRL&=0XFF00FFFF; 
//	GPIOC->CRL|=0X00830000;	
/////	MISO_IN(); Pc.4 pc.5
//	MOSI=1;
//	SCLK=1;
//	FRAM_CS=1;
 	RCC->APB2ENR|=1<<2;       //PORTAʱ��ʹ�� 	 
 	RCC->APB2ENR|=1<<12;      //SPI1ʱ��ʹ�� 
 		   
 	//����ֻ���SPI�ڳ�ʼ��
 	GPIOA->CRL&=0X0000FFFF; 
 	GPIOA->CRL|=0XBBB30000;//PA5.6.7���� 	    
 	GPIOA->ODR|=0Xf<<4;    //PA5.6.7����
 		
 	SPI1->CR1|=0<<10;//ȫ˫��ģʽ	
 	SPI1->CR1|=1<<9; //���nss����
 	SPI1->CR1|=1<<8;  

 	SPI1->CR1|=1<<2; //SPI����
 	SPI1->CR1|=0<<11;//8bit���ݸ�ʽ	
 	SPI1->CR1|=1<<1; //����ģʽ��SCKΪ1 CPOL=1
 	SPI1->CR1|=1<<0; //���ݲ����ӵڶ���ʱ����ؿ�ʼ,CPHA=1  
 	SPI1->CR1|=0<<3; //Fsck=Fcpu/256
 	SPI1->CR1|=0<<7; //MSBfirst
 	SPI1->CR1|=1<<6; //SPI�豸ʹ��
// 	SPIx_ReadWriteByte(0xff);//��������		 
	SPI1_SetSpeed(SPI_SPEED_4);	//����Ϊ18Mʱ��,����ģʽ
}   
//SPI1 �ٶ����ú���
//SpeedSet:0~7
//SPI�ٶ�=fAPB2/2^(SpeedSet+1)
//APB2ʱ��һ��Ϊ72Mhz
void SPI1_SetSpeed(u8 SpeedSet)
{
	SpeedSet&=0X07;			//���Ʒ�Χ
	SPI1->CR1&=0XFFC7; 
	SPI1->CR1|=SpeedSet<<3;	//����SPI1�ٶ�  
	SPI1->CR1|=1<<6; 		//SPI�豸ʹ�� 
} 
//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
 u8 SPIx_ReadWriteByte(u8 TxData)
 {		
 	u8 retry=0;				 
 	while((SPI1->SR&1<<1)==0)//�ȴ���������	
 	{
 		retry++;
 		if(retry>200)return 0;
 	}			  
 	SPI1->DR=TxData;	 	  //����һ��byte 
 	retry=0;
 	while((SPI1->SR&1<<0)==0) //�ȴ�������һ��byte  
 	{
 		retry++;
 		if(retry>200)return 0;
 	}	  						    
 	return SPI1->DR;          //�����յ�������				    
 }
//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
//u8 SPIx_ReadWriteByte(u8 TxData)
//{		
//	u16 retry=0;				 
//	while((SPI1->SR&1<<1)==0)//�ȴ���������	
//	{
//		retry++;
//		if(retry>0XFFFE)return 0;
//	}			  
//	SPI1->DR=TxData;	 	  //����һ��byte 
//	retry=0;
//	while((SPI1->SR&1<<0)==0) //�ȴ�������һ��byte  
//	{
//		retry++;
//		if(retry>0XFFFE)return 0;
//	}	  						    
//	return SPI1->DR;          //�����յ�������				    
//}



unsigned char FE_IN(void)
{
 unsigned char i;
 unsigned char TempBit  = 0;
 unsigned char TempData = 0;
  
 for(i=0;i<8;i++)
 {
  SCLK=0;	
  
  if (MISO == 1)
  {
      TempBit = 1;
  }
  else
  {
      TempBit = 0;
  }
  TempData = (TempData << 1) | TempBit;

  SCLK=1;
  delay_us(2); 
 }	
  
  return(TempData);
}

void FE_OUT(unsigned char WriteData)
{
 unsigned char i;
 SCLK=0;
 for(i=0;i<8;i++)
 {
  if (((WriteData >> 7) & 0x01) == 0x01)
    { 
	  SCLK=0;
	  delay_us(2); 
      MOSI=1;
      SCLK=1;
     
    }
    else
    { SCLK=0;
	  delay_us(2);	
      MOSI=0;
      SCLK=1;
    }
    WriteData = WriteData << 1;
 } 
}





