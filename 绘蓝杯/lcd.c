/*==========================================================
 * �������ƣ����ڶ�ʱ����1602����ʱ��	
 * ������Ա�����ֵ��ӿƼ���ѧ ����ѧԺ ����
 * ����������keil5
 * ����ʱ�䣺2015/12/02  15:41:10
 * �޸�ʱ�䣺
 * ����˵������ʱ6��
 *			RS->P1^1 RW->P1^2 E->P1^3
 *==========================================================*/

#include<reg52.h>
#define lcd P0

sbit rs=P1^1 ;
sbit rw=P1^2 ;
sbit e=P1^3	 ;
sbit buzz=P3^7;
sbit key1=P2^0;//���ܼ�
sbit key2=P2^1;//�� ��
sbit key3=P2^2;//�� ��
sbit key4=P2^3;//���Ӽ�

void writecmd(unsigned char cmd);
void set(unsigned char x,unsigned char y);
void key();
void initlcd();
void count();
void play();
void delayms(unsigned int xms);
void show(unsigned char x,unsigned char y,unsigned char *table);
void ling();

unsigned char b=0;
unsigned char c=0;
unsigned char cnt=0;
unsigned char k=0;
unsigned char z=0;
unsigned char di=0;
unsigned char table[]="2016-09-17";
unsigned char table1[]="19-40-56";
unsigned char code time[]="0123456789" ;
unsigned char code clock[]="clock";
unsigned char clock1[]="09-17|20-49"; 
unsigned char code clock2[]="@"; 
unsigned char miao=56;
unsigned char fen=40;
unsigned char shi=19;
unsigned char ri=17;
unsigned char yue=9;
unsigned int nian=2016;
unsigned char fena=49;
unsigned char shia=20;
unsigned char yuea=9;
unsigned char ria=17;

void main()
{  
			TMOD=0x01;
			EA=1;
			TH0=(65536-50000)/256;
			TL0=(65536-50000)%256;
			ET0=1;
			TR0=1;
			TH1 = 0xfb;
			TL1 = 0x90;
			ET1=1;
			initlcd();  
			while(1)
			{ 				
				ling();
				count();
				play();
				show(3,0,table);
				show(2,1,table1);
				key();                
			}
}

/*==========================================================   
 *�������ƣ�key()
 *��    �ܣ���������ʱ��
 *��ڲ�����
 *���ڲ�����
 *˵    ����
 *==========================================================*/
void key()
{
  if(key1==0)
		{
			delayms(10);
			if(key1==0)
			{
				k++; 
				c++;
				while(!key1);
				TR0=0;
				while (c==1)  
					{
						if(key1==0)
							delayms(10);	             
						if(key1==0)	 
							{ 
								k++ ;
								while(!key1);		
							}
						switch(k)
							{
								case 1:	  set(9,1); writecmd(0x0f);break;
								case 2:	  set(6,1);                break;
								case 3:	  set(3,1);                break;
								case 4:   set(12,0);               break;
								case 5:	  set(9,0);                break;
								case 6:	  set(6,0);                break;
								case 7:   TR0=1; b=1;initlcd();show(11,0,clock);
													show(2,1,clock1);k=9;break;
								case 8:	  k=0;c=0;TR0=1;writecmd(0x0c);initlcd(); break;
							}
						while(k==9)			
							{		//֮ǰ���дif�ģ�����
								if(key1==0)
									{
										delayms(10);
										if(key1==0)
											{
												if(b==4) 
													k=8;
												while(!key1);
												b++;	
				              }
									}
								switch(b)
									{	
										case 1:	  set(12,1); writecmd(0x0f);break;
										case 2:	  set(9,1);                 break;
										case 3:	  set(6,1);                 break;
										case 4:   set(3,1);                 break;
									}
								if(key2==0)			//������
									{
										delayms(10);
										if(key2==0)
											{
												while(!key2);		  //���ּ��
												switch(b)
													{
														case 1:	fena++;
																		if(fena>=60)
																			fena=0;
																		clock1[10]=time[fena%10];
																		clock1[9]=time[fena/10];  break;

														case 2:	shia++;
																		if(shia>=24)
																			shia=0;
																		clock1[7]=time[shia%10];
																		clock1[6]=time[shia/10];  break;
														case 3:	ria++;
																		if(ria>=32)
																			ria=0;
																		clock1[4]=time[ria%10];
																		clock1[3]=time[ria/10];  break;
														case 4:	yuea++;
																		if(yuea>=13)
																			yuea=0;
																		clock1[1]=time[yuea%10];
																		clock1[0]=time[yuea/10];  break;
                          }	
	                    }	   
	                }	   
								if(key3==0)			//������
									{
										delayms(10);
										if(key3==0)
											{
												while(!key3);		  //���ּ��		
												switch(b)
													{
														case 1: fena--;
																	  if(fena==-1)
																		  fena=59;
																	  clock1[10]=time[fena%10];
																	  clock1[9]=time[fena/10];  break;
														case 2: shia--;
																	  if(shia==-1)
																			shia=23;
																		clock1[7]=time[shia%10];
																		clock1[6]=time[shia/10];  break;
														case 3:	ria--;
																		if(ria==0)
																			ria=31;
																		clock1[4]=time[ria%10];
																		clock1[3]=time[ria/10];  break;
														case 4:	yuea--;
																		if(yuea==0)
																			yuea=12;
																		clock1[1]=time[yuea%10];
																		clock1[0]=time[yuea/10];  break;
	                        }	
	                    }
	                }	   
	              show(2,1,clock1);              
							}		 	      	        
						if(k!=0)
							{
								if(key2==0)			//������
									{
										delayms(10);
										if(key2==0)
											{
												while(!key2);		  //���ּ��						
												switch(k)
													{
														case 1:	miao++;
																		if(miao==60)
																			miao=0;
																		table1[7]=time[miao%10];
																		table1[6]=time[miao/10];  break;
														case 2:	fen++;
																		if(fen==60)
																			fen=0;
																		table1[4]=time[fen%10];
																		table1[3]=time[fen/10];	  break;
														case 3:	shi++;
																		if(shi==24)
																			shi=0;
																		table1[1]=time[shi%10];
																		table1[0]=time[shi/10];   break;
														case 4: ri++;
																		if(ri==32)
																			ri=0;
																		table[9]=time[ri%10];
																		table[8]=time[ri/10];     break;
														case 5:	yue++;
																		if(yue==12)
																			yue=0;
																		table[6]=time[yue%10];
																		table[5]=time[yue/10];    break;
														case 6:	nian++;
																		table[3]=time[nian%10];
																		table[2]=time[nian%100/10];
																		table[1]=time[nian/100%10];
																		table[0]=time[nian/1000];  break;

													}		
											}
									}
							 else if(key3==0)			//������
											{
												delayms(10);
												if(key3==0)
													{
														while(!key3);	   //���ּ��
														switch(k)
															{
																case 1:	miao--;
																				if(miao==-1)
																					miao=59;
																				table1[7]=time[miao%10];
																				table1[6]=time[miao/10];  break;
																case 2:	fen--;
																				if(fen==-1)
																					fen=59;
																				table1[4]=time[fen%10];
																				table1[3]=time[fen/10];
																																		break;
																case 3:	shi--;
																				if(shi==-1)
																					shi=23;
																				table1[1]=time[shi%10];
																				table1[0]=time[shi/10];   break;
																case 4: ri--;
																				if(ri==0)
																					ri=31;
																				table[9]=time[ri%10];
																				table[8]=time[ri/10];     break;
																case 5:	yue--;
																				if(yue==0)
																					yue=12;
																				table[6]=time[yue%10];
																				table[5]=time[yue/10];    break;
																case 6:	nian--;
																				table[3]=time[nian%10];
																				table[2]=time[nian%100/10];
																				table[1]=time[nian/100%10];
																				table[0]=time[nian/1000];  break;		        
                              }		
                         }
										}
	   				    }  
             show(3,0,table);
             show(2,1,table1);					  
					}						
			 }	
	 }     
}

/*==========================================================   
 *�������ƣ�ling()
 *��    �ܣ���������
 *��ڲ�����
 *���ڲ�����
 *˵    ��������4����
 *==========================================================*/
void ling()
{ 	
    if(key4==0)
		{				   
		  delayms(10);
		  if(key4==0)
			{  
		    z=~z ;
		    di++;		
		    if(di==1)
					show(15,0,clock2);
		    else if(di==2)
		    {
		      di=0;	
		      initlcd();  
		    }
		    while(!key4);
			}
		}
		
	  if((fen==fena)&&(shi==shia))
		if((ri==ria)&&(yue==yuea))
	   	{
				if(z>=1)
				{		
					TR1 = 1;
    	    delayms(100);
					TR1 = 0;
					delayms(100); 
	      }
      }
		else
			TR1=0;	   
}

/*==========================================================   
 *�������ƣ�delayms(unsigned int xms)
 *��    �ܣ���ʱ
 *��ڲ�����
 *���ڲ�����
 *˵    ����
 *==========================================================*/
void delayms(unsigned int xms)
{
    unsigned int i,j;
		for(i=xms;i>0;i--)
			for(j=110;j>0;j--);
}

/*==========================================================   
 *�������ƣ�count()
 *��    �ܣ�����ʱ��
 *��ڲ�����
 *���ڲ�����
 *˵    ���������60����0��һ�֣��ִ���60����0��һʱ......
 *==========================================================*/
void count()
{
	if(miao>=60)
	{
		fen++;
		miao=0;
		if(fen>=60)
		{
			shi++;
			fen=0;
			if(shi>=24)
			{
				ri++;
			  shi=0;
		    if(ri>=29)
				{	
				  if (!((nian%400==0)||((nian%100!=0)&&(nian%4==0)))&&(yue==2))	
					{
						ri=1;
			      yue++;     
					}
					else if((ri>=30)&&(yue==2))
					{    
						ri=1;
				    yue++;  
					}			
					else if(ri>=31)
					{
						if(yue==4||yue==6||yue==9||yue==11)
						{
							yue++;
							ri=1;
						}
						else if(ri>=32)
						{
							yue++;
							ri=1;
						}
						if(yue>=13) 
						{
							nian++;
							yue=1;
						}
				  }
				}  
			}	
		}
	}
}

/*==========================================================   
 *�������ƣ�play()
 *��    �ܣ�����1602Ҫ��ʾ��ʱ��
 *��ڲ�����
 *���ڲ�����
 *˵    ����
 *==========================================================*/
void play()
{
	table1[7]=time[miao%10];
	table1[6]=time[miao/10];
	table1[4]=time[fen%10];
	table1[3]=time[fen/10];
	table1[1]=time[shi%10];
	table1[0]=time[shi/10];
	table[9]=time[ri%10];
	table[8]=time[ri/10];
	table[6]=time[yue%10];
	table[5]=time[yue/10];
	table[3]=time[nian%10];
	table[2]=time[nian%100/10];
	table[1]=time[nian/100%10];
	table[0]=time[nian/1000];

}


void waitready()	  //���lcd�Ƿ���æ
{       
		unsigned char sta;
		lcd= 0xff;  
		rs=0;
		rw=1;
	  do
    {
		e=1;
		sta=lcd;
		e=0;
		}
		while (sta&0x80);  
}


void writecmd(unsigned char cmd)			//����ָ��
{ 
		waitready();
		rs=0;
		rw=0;
		lcd=cmd;
		e=1;
		e=0;  
}


void writedat(unsigned char dat)		 //��������
{
		waitready();
		rs=1;
		rw=0;
		lcd=dat;
		e=1;
		e=0;
}

void set(unsigned char x,unsigned char y)	   //ȷ����ʾ��lcd���ĸ�λ��
{
		unsigned char addr;
		if(y==0)   
			addr=0x00+x;  
		else 		   
			addr=0x40+x;	
			writecmd(addr|0x80);
}

void show(unsigned char x,unsigned char y,unsigned char *table)
{
    set(x,y);
    while(*table!='\0')
		{
     writedat(*table++);             
		}	
}


void initlcd()	   //��ʼ��
{
     writecmd(0x38);
     writecmd(0x0c);
     writecmd(0x06);
     writecmd(0x01);
}	 

 void InterruptTimer() interrupt 1
{  
   cnt++;
   TH0=(65536-50000)/256;
   TL0=(65536-50000)%256;
   if(cnt>20)
	{
	 miao++;
	 cnt=0;
  }
}

void buzzIntr() interrupt 3
{
	TH1 = 0xfb;
	TL1 = 0x90;
	buzz = ~buzz;
}
   
