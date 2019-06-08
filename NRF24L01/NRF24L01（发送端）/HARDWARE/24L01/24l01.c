/*****************************************
*		官方驱动，直接调用就好
*****************************************/

#include "24l01.h"
#include "stm32f10x.h"
#include "delay.h"
#include "led.h"

u8 RevTempDate[5];//最后一位用来存放结束标志  

u8  sta;   //状态标志
#define RX_DR	sta&0x40
#define TX_DS	sta&0x20
#define MAX_RT	sta&0x10


//初始化24L01的IO口
void NRF24L01_GPIOInit(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);	 //使能PB,G端口时钟
    	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	GPIO_SetBits(GPIOA,GPIO_Pin_6);						 	//PB.5 输出高

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 端口配置, 推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOA,GPIO_Pin_5); 						 //PE.5 输出高
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 //LED1-->PE.5 端口配置, 推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOA,GPIO_Pin_4); 						 //PE.5 输出高
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	    		 //LED1-->PE.5 端口配置, 推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOA,GPIO_Pin_7); 						 //PE.5 输出高
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 端口配置, 推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOC,GPIO_Pin_5); 						 //PE.5 输出高
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 //LED1-->PE.5 端口配置, 推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOC,GPIO_Pin_4); 						 //PE.5 输出高
	 		 	 
}


/*****************SPI时序函数******************************************/
u8 NRFSPI(u8 date)			//NRF24L01的SPI写时序
{
    u8 i;
   	for(i=0;i<8;i++)          // 循环8次
   	{
	  if(date&0x80)
	    MOSI=1;
	  else
	    MOSI=0;   // byte最高位输出到MOSI
   	  date<<=1;             // 低一位移位到最高位
   	  SCLK=1; 
	  if(READMISO)			//读取MOSO// 拉高SCK，nRF24L01从MOSI读入1位数据，同时从MISO输出1位数据
   	    date|=0x01;       	// 读MISO到byte最低位
   	  SCLK=0;            	// SCK置低
   	}
    return(date);           	// 返回读出的一字节
}

/**********************NRF24L01初始化函数*******************************/
void NRF24L01Int(void)
{
	delay_ms(2);//让系统什么都不干
	CE=0;
	CSN=1;  
	SCLK=0;
	IRQ=1; 
}

/*****************SPI读寄存器一字节函数*********************************/
u8 NRFReadReg(u8 RegAddr)
{
   u8 BackDate;
   CSN=0;//启动时序
   NRFSPI(RegAddr);//写寄存器地址
   BackDate=NRFSPI(0x00);//写入读寄存器指令  
   CSN=1;
   return(BackDate); //返回状态
}

/*****************SPI写寄存器一字节函数*********************************/
u8 NRFWriteReg(u8 RegAddr,u8 date)
{
   u8 BackDate;
   CSN=0;//启动时序
   BackDate=NRFSPI(RegAddr);//写入地址
   NRFSPI(date);//写入值
   CSN=1;  
   return(BackDate);
}	   

/*****************SPI读取RXFIFO寄存器的值********************************/
u8 NRFReadRxDate(u8 RegAddr,u8 *RxDate,u8 DateLen)
{  //寄存器地址//读取数据存放变量//读取数据长度//用于接收
    u8 BackDate,i;
	CSN=0;//启动时序
	BackDate=NRFSPI(RegAddr);//写入要读取的寄存器地址
	for(i=0;i<DateLen;i++) //读取数据
	  {
	     RxDate[i]=NRFSPI(0);
	  } 
    CSN=1;
   return(BackDate); 
}
/*****************SPI写入TXFIFO寄存器的值**********************************/
u8 NRFWriteTxDate(u8 RegAddr,u8 *TxDate,u8 DateLen)
{ //寄存器地址//写入数据存放变量//读取数据长度//用于发送
   u8 BackDate,i;
   CSN=0;
   BackDate=NRFSPI(RegAddr);//写入要写入寄存器的地址
   for(i=0;i<DateLen;i++)//写入数据
     {
	    NRFSPI(*TxDate++);
	 }   
   CSN=1;
   return(BackDate);
}
/*****************NRF设置为接收模式并接收数据******************************/
//接收模式
void NRFSetRXMode(u8 *Rx_addr)
{
    CE=0;
  	NRFWriteTxDate(W_REGISTER+RX_ADDR_P0,Rx_addr,5);  // 接收设备接收通道0使用和发送设备相同的发送地址
		NRFWriteReg(W_REGISTER+EN_AA,0x3f);               // 使能接收通道0自动应答
  	NRFWriteReg(W_REGISTER+EN_RXADDR,0x3f);           // 使能接收通道0
  	NRFWriteReg(W_REGISTER+RF_CH,0x40);                 // 选择射频通道0x40
		NRFWriteReg(W_REGISTER+RX_PW_P0,TX_DATA_WITDH);  // 接收通道0选择和发送通道相同有效数据宽度
  	NRFWriteReg(W_REGISTER+RF_SETUP,0x0f);            // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
	  NRFWriteReg(W_REGISTER+CONFIG,0x0f);             // CRC使能，16位CRC校验，上电，接收模式     
  	CE = 1; 
	delay_ms(5);    
}
/*****************NRF设置为发送模式并发送数据******************************/
void NRFSetTxMode(u8 *TxDate,u8 *TxAddr,u8 PX)
{//发送模式
    CE=0; 
   	NRFWriteTxDate(W_REGISTER+TX_ADDR,TxAddr,TX_ADDR_WITDH);//写寄存器指令+接收地址使能指令+接收地址+地址宽度
	NRFWriteTxDate(W_REGISTER+PX,TxAddr,TX_ADDR_WITDH);//为了应答接收设备，接收通道0地址和发送地址相同
	NRFWriteTxDate(W_TX_PAYLOAD,TxDate,TX_DATA_WITDH);//写入数据 
	/******下面有关寄 存器配置**************/
  	NRFWriteReg(W_REGISTER+EN_AA,0x3F);       //0x3f 使能接收通道自动应答
  	NRFWriteReg(W_REGISTER+EN_RXADDR,0x3F);   //0x3f 使能接收通道
    NRFWriteReg(W_REGISTER+SETUP_RETR,0x1A);  //0x1a 自动重发延时等待250us+86us，自动重发10次
	NRFWriteReg(W_REGISTER+RF_CH,0x40);         // 选择射频通道0x40
  	NRFWriteReg(W_REGISTER+RF_SETUP,0x0f);    // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
	NRFWriteReg(W_REGISTER+CONFIG,0x0e);      // CRC使能，16位CRC校验，上电  
	CE=1;
	delay_us(15);//保持10us秒以上
}

/****************************检测应答信号******************************/
u8 CheckACK(void)
{  //用于发射
	sta=NRFReadReg(R_REGISTER+STATUS);           // 返回状态寄存器
	if(TX_DS||MAX_RT) //发送完毕中断
	{//LED1=0;
	   NRFWriteReg(W_REGISTER+STATUS,0xff);  // 清除TX_DS或MAX_RT中断标志
	   CSN=0;
	   NRFSPI(FLUSH_TX);//用于清空FIFO ！！关键！！不然会出现意想不到的后果！！！大家记住！！  
       CSN=1; 
	   return(0);
	}
	else
	return(1);
}
 /*************************接收数据*********************************************
void GetDate(void)				 
{
    sta=NRFReadReg(R_REGISTER+STATUS);//发送数据后读取状态寄存器  
    if(RX_DR)				// 判断是否接收到数据
     { 
	   CE=0;//待机
	   NRFReadRxDate(R_RX_PAYLOAD,RevTempDate,RX_DATA_WITDH);// 从RXFIFO读取数据 接收4位即可，后一位位结束位
		if (RevTempDate[0]==0)
		{
			    //P2=0xf0;
		}
	   NRFWriteReg(W_REGISTER+STATUS,0xff); //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标
	   CSN=0;
	   NRFSPI(FLUSH_RX);//用于清空FIFO ！！关键！！不然会出现意想不到的后果！！！大家记住！！ 
	   CSN=1;
	   sta=0;
	}	 
}*/

