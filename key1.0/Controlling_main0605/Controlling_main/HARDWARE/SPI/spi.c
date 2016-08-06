#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//SPI 驱动函数	   
////////////////////////////////////////////////////////////////////////////////// 	  
 



//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25X16/							  
//SPI口初始化
//这里针是对SPI1的初始化
void SPIx_Init(void)
{	 
	
	
//	RCC->APB2ENR|=1<<2;       //PORTA时钟使能
//	RCC->APB2ENR|=1<<3;       //PORTA时钟使能 	
//	RCC->APB2ENR|=1<<4;       //PORTA时钟使能 	 
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
 	RCC->APB2ENR|=1<<2;       //PORTA时钟使能 	 
 	RCC->APB2ENR|=1<<12;      //SPI1时钟使能 
 		   
 	//这里只针对SPI口初始化
 	GPIOA->CRL&=0X0000FFFF; 
 	GPIOA->CRL|=0XBBB30000;//PA5.6.7复用 	    
 	GPIOA->ODR|=0Xf<<4;    //PA5.6.7上拉
 		
 	SPI1->CR1|=0<<10;//全双工模式	
 	SPI1->CR1|=1<<9; //软件nss管理
 	SPI1->CR1|=1<<8;  

 	SPI1->CR1|=1<<2; //SPI主机
 	SPI1->CR1|=0<<11;//8bit数据格式	
 	SPI1->CR1|=1<<1; //空闲模式下SCK为1 CPOL=1
 	SPI1->CR1|=1<<0; //数据采样从第二个时间边沿开始,CPHA=1  
 	SPI1->CR1|=0<<3; //Fsck=Fcpu/256
 	SPI1->CR1|=0<<7; //MSBfirst
 	SPI1->CR1|=1<<6; //SPI设备使能
// 	SPIx_ReadWriteByte(0xff);//启动传输		 
	SPI1_SetSpeed(SPI_SPEED_4);	//设置为18M时钟,高速模式
}   
//SPI1 速度设置函数
//SpeedSet:0~7
//SPI速度=fAPB2/2^(SpeedSet+1)
//APB2时钟一般为72Mhz
void SPI1_SetSpeed(u8 SpeedSet)
{
	SpeedSet&=0X07;			//限制范围
	SPI1->CR1&=0XFFC7; 
	SPI1->CR1|=SpeedSet<<3;	//设置SPI1速度  
	SPI1->CR1|=1<<6; 		//SPI设备使能 
} 
//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
 u8 SPIx_ReadWriteByte(u8 TxData)
 {		
 	u8 retry=0;				 
 	while((SPI1->SR&1<<1)==0)//等待发送区空	
 	{
 		retry++;
 		if(retry>200)return 0;
 	}			  
 	SPI1->DR=TxData;	 	  //发送一个byte 
 	retry=0;
 	while((SPI1->SR&1<<0)==0) //等待接收完一个byte  
 	{
 		retry++;
 		if(retry>200)return 0;
 	}	  						    
 	return SPI1->DR;          //返回收到的数据				    
 }
//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
//u8 SPIx_ReadWriteByte(u8 TxData)
//{		
//	u16 retry=0;				 
//	while((SPI1->SR&1<<1)==0)//等待发送区空	
//	{
//		retry++;
//		if(retry>0XFFFE)return 0;
//	}			  
//	SPI1->DR=TxData;	 	  //发送一个byte 
//	retry=0;
//	while((SPI1->SR&1<<0)==0) //等待接收完一个byte  
//	{
//		retry++;
//		if(retry>0XFFFE)return 0;
//	}	  						    
//	return SPI1->DR;          //返回收到的数据				    
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





