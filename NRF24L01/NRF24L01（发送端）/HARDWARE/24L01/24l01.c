/*****************************************
*		�ٷ�������ֱ�ӵ��þͺ�
*****************************************/

#include "24l01.h"
#include "stm32f10x.h"
#include "delay.h"
#include "led.h"

u8 RevTempDate[5];//���һλ������Ž�����־  

u8  sta;   //״̬��־
#define RX_DR	sta&0x40
#define TX_DS	sta&0x20
#define MAX_RT	sta&0x10


//��ʼ��24L01��IO��
void NRF24L01_GPIOInit(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PB,G�˿�ʱ��
    	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	GPIO_SetBits(GPIOA,GPIO_Pin_6);						 	//PB.5 �����

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 �˿�����, �������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOA,GPIO_Pin_5); 						 //PE.5 �����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 //LED1-->PE.5 �˿�����, �������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOA,GPIO_Pin_4); 						 //PE.5 �����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	    		 //LED1-->PE.5 �˿�����, �������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOA,GPIO_Pin_7); 						 //PE.5 �����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 �˿�����, �������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOC,GPIO_Pin_5); 						 //PE.5 �����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	    		 //LED1-->PE.5 �˿�����, �������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOC,GPIO_Pin_4); 						 //PE.5 �����
	 		 	 
}


/*****************SPIʱ����******************************************/
u8 NRFSPI(u8 date)			//NRF24L01��SPIдʱ��
{
    u8 i;
   	for(i=0;i<8;i++)          // ѭ��8��
   	{
	  if(date&0x80)
	    MOSI=1;
	  else
	    MOSI=0;   // byte���λ�����MOSI
   	  date<<=1;             // ��һλ��λ�����λ
   	  SCLK=1; 
	  if(READMISO)			//��ȡMOSO// ����SCK��nRF24L01��MOSI����1λ���ݣ�ͬʱ��MISO���1λ����
   	    date|=0x01;       	// ��MISO��byte���λ
   	  SCLK=0;            	// SCK�õ�
   	}
    return(date);           	// ���ض�����һ�ֽ�
}

/**********************NRF24L01��ʼ������*******************************/
void NRF24L01Int(void)
{
	delay_ms(2);//��ϵͳʲô������
	CE=0;
	CSN=1;  
	SCLK=0;
	IRQ=1; 
}

/*****************SPI���Ĵ���һ�ֽں���*********************************/
u8 NRFReadReg(u8 RegAddr)
{
   u8 BackDate;
   CSN=0;//����ʱ��
   NRFSPI(RegAddr);//д�Ĵ�����ַ
   BackDate=NRFSPI(0x00);//д����Ĵ���ָ��  
   CSN=1;
   return(BackDate); //����״̬
}

/*****************SPIд�Ĵ���һ�ֽں���*********************************/
u8 NRFWriteReg(u8 RegAddr,u8 date)
{
   u8 BackDate;
   CSN=0;//����ʱ��
   BackDate=NRFSPI(RegAddr);//д���ַ
   NRFSPI(date);//д��ֵ
   CSN=1;  
   return(BackDate);
}	   

/*****************SPI��ȡRXFIFO�Ĵ�����ֵ********************************/
u8 NRFReadRxDate(u8 RegAddr,u8 *RxDate,u8 DateLen)
{  //�Ĵ�����ַ//��ȡ���ݴ�ű���//��ȡ���ݳ���//���ڽ���
    u8 BackDate,i;
	CSN=0;//����ʱ��
	BackDate=NRFSPI(RegAddr);//д��Ҫ��ȡ�ļĴ�����ַ
	for(i=0;i<DateLen;i++) //��ȡ����
	  {
	     RxDate[i]=NRFSPI(0);
	  } 
    CSN=1;
   return(BackDate); 
}
/*****************SPIд��TXFIFO�Ĵ�����ֵ**********************************/
u8 NRFWriteTxDate(u8 RegAddr,u8 *TxDate,u8 DateLen)
{ //�Ĵ�����ַ//д�����ݴ�ű���//��ȡ���ݳ���//���ڷ���
   u8 BackDate,i;
   CSN=0;
   BackDate=NRFSPI(RegAddr);//д��Ҫд��Ĵ����ĵ�ַ
   for(i=0;i<DateLen;i++)//д������
     {
	    NRFSPI(*TxDate++);
	 }   
   CSN=1;
   return(BackDate);
}
/*****************NRF����Ϊ����ģʽ����������******************************/
//����ģʽ
void NRFSetRXMode(u8 *Rx_addr)
{
    CE=0;
  	NRFWriteTxDate(W_REGISTER+RX_ADDR_P0,Rx_addr,5);  // �����豸����ͨ��0ʹ�úͷ����豸��ͬ�ķ��͵�ַ
		NRFWriteReg(W_REGISTER+EN_AA,0x3f);               // ʹ�ܽ���ͨ��0�Զ�Ӧ��
  	NRFWriteReg(W_REGISTER+EN_RXADDR,0x3f);           // ʹ�ܽ���ͨ��0
  	NRFWriteReg(W_REGISTER+RF_CH,0x40);                 // ѡ����Ƶͨ��0x40
		NRFWriteReg(W_REGISTER+RX_PW_P0,TX_DATA_WITDH);  // ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
  	NRFWriteReg(W_REGISTER+RF_SETUP,0x0f);            // ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������
	  NRFWriteReg(W_REGISTER+CONFIG,0x0f);             // CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ     
  	CE = 1; 
	delay_ms(5);    
}
/*****************NRF����Ϊ����ģʽ����������******************************/
void NRFSetTxMode(u8 *TxDate,u8 *TxAddr,u8 PX)
{//����ģʽ
    CE=0; 
   	NRFWriteTxDate(W_REGISTER+TX_ADDR,TxAddr,TX_ADDR_WITDH);//д�Ĵ���ָ��+���յ�ַʹ��ָ��+���յ�ַ+��ַ���
	NRFWriteTxDate(W_REGISTER+PX,TxAddr,TX_ADDR_WITDH);//Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
	NRFWriteTxDate(W_TX_PAYLOAD,TxDate,TX_DATA_WITDH);//д������ 
	/******�����йؼ� ��������**************/
  	NRFWriteReg(W_REGISTER+EN_AA,0x3F);       //0x3f ʹ�ܽ���ͨ���Զ�Ӧ��
  	NRFWriteReg(W_REGISTER+EN_RXADDR,0x3F);   //0x3f ʹ�ܽ���ͨ��
    NRFWriteReg(W_REGISTER+SETUP_RETR,0x1A);  //0x1a �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�10��
	NRFWriteReg(W_REGISTER+RF_CH,0x40);         // ѡ����Ƶͨ��0x40
  	NRFWriteReg(W_REGISTER+RF_SETUP,0x0f);    // ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������
	NRFWriteReg(W_REGISTER+CONFIG,0x0e);      // CRCʹ�ܣ�16λCRCУ�飬�ϵ�  
	CE=1;
	delay_us(15);//����10us������
}

/****************************���Ӧ���ź�******************************/
u8 CheckACK(void)
{  //���ڷ���
	sta=NRFReadReg(R_REGISTER+STATUS);           // ����״̬�Ĵ���
	if(TX_DS||MAX_RT) //��������ж�
	{//LED1=0;
	   NRFWriteReg(W_REGISTER+STATUS,0xff);  // ���TX_DS��MAX_RT�жϱ�־
	   CSN=0;
	   NRFSPI(FLUSH_TX);//�������FIFO �����ؼ�������Ȼ��������벻���ĺ����������Ҽ�ס����  
       CSN=1; 
	   return(0);
	}
	else
	return(1);
}
 /*************************��������*********************************************
void GetDate(void)				 
{
    sta=NRFReadReg(R_REGISTER+STATUS);//�������ݺ��ȡ״̬�Ĵ���  
    if(RX_DR)				// �ж��Ƿ���յ�����
     { 
	   CE=0;//����
	   NRFReadRxDate(R_RX_PAYLOAD,RevTempDate,RX_DATA_WITDH);// ��RXFIFO��ȡ���� ����4λ���ɣ���һλλ����λ
		if (RevTempDate[0]==0)
		{
			    //P2=0xf0;
		}
	   NRFWriteReg(W_REGISTER+STATUS,0xff); //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�
	   CSN=0;
	   NRFSPI(FLUSH_RX);//�������FIFO �����ؼ�������Ȼ��������벻���ĺ����������Ҽ�ס���� 
	   CSN=1;
	   sta=0;
	}	 
}*/

