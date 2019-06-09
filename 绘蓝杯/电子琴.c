/******************************
*	作品：1602简易时钟		  *
*   2015年12月2日15:41:10	  *
*	桂林电子科技大学	      *
*	程序员：风筝			  *
*	11月27日到12月2日		  *
*   历时6天	   ^_^			  *
******************************/

#include<reg52.h>
#define lcd P0

sbit rs=P2^6 ;
sbit rw=P2^5 ;
sbit e=P2^7	 ;
sbit buzz=P1^5;
sbit key1=P3^1;
sbit key2=P3^0;
sbit key3=P3^2;
sbit key4=P3^3;

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
unsigned char table[]="2016-02-28";
unsigned char table1[]="23-58-56";
unsigned char code time[]="0123456789" ;
unsigned char code clock[]="clock";
unsigned char clock1[]="02-28|23-59"; 
unsigned char code clock2[]="@"; 
unsigned char miao=56;
unsigned char fen=58;
unsigned char shi=23;
unsigned char ri=28;
unsigned char yue=2;
unsigned int nian=2016;
unsigned char fena=59;
unsigned char shia=23;
unsigned char yuea=2;
unsigned char ria=28;
unsigned char code T[49][2]={{0,0},
{0xF8,0x8B},{0xF8,0xF2},{0xF9,0x5B},{0xF9,0xB7},{0xFA,0x14},{0xFA,0x66},{0xFA,0xB9},{0xFB,0x03},{0xFB,0x4A},{0xFB,0x8F},{0xFB,0xCF},{0xFC,0x0B},
{0xFC,0x43},{0xFC,0x78},{0xFC,0xAB},{0xFC,0xDB},{0xFD,0x08},{0xFD,0x33},{0xFD,0x5B},{0xFD,0x81},{0xFD,0xA5},{0xFD,0xC7},{0xFD,0xE7},{0xFE,0x05},
{0xFE,0x21},{0xFE,0x3C},{0xFE,0x55},{0xFE,0x6D},{0xFE,0x84},{0xFE,0x99},{0xFE,0xAD},{0xFE,0xC0},{0xFE,0x02},{0xFE,0xE3},{0xFE,0xF3},{0xFF,0x02},
{0xFF,0x10},{0xFF,0x1D},{0xFF,0x2A},{0xFF,0x36},{0xFF,0x42},{0xFF,0x4C},{0xFF,0x56},{0xFF,0x60},{0xFF,0x69},{0xFF,0x71},{0xFF,0x79},{0xFF,0x81}
};
unsigned char music[35][2]={{0,4}} ;

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
  
while(1){ 
        ling();
        count();
	    play();
		show(3,0,table);
        show(2,1,table1);
	    key();                
		}
}


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


		switch(k){
		case 1:	  set(9,1); writecmd(0x0f);break;
		case 2:	  set(6,1);                break;
		case 3:	  set(3,1);                break;
		case 4:   set(12,0);               break;
		case 5:	  set(9,0);                break;
		case 6:	  set(6,0);                break;
		case 7:   TR0=1; b=1;initlcd();show(11,0,clock);show(2,1,clock1);k=9;break;
		case 8:	  k=0;c=0;TR0=1;writecmd(0x0c);initlcd(); break;
		}
       
	   while(k==9)
	   {		//之前这个写if的，改了
	      if(key1==0){
		  delayms(10);

		    if(key1==0)
			{
		      if(b==4) 
		      k=8;
		
		         while(!key1);b++;	
				                    }
									   }
		 switch(b){	
	     case 1:	  set(12,1); writecmd(0x0f);break;
		 case 2:	  set(9,1);                 break;
		 case 3:	  set(6,1);                 break;
	     case 4:      set(3,1);                 break;
		  }

		if(key2==0)			//按键加
		{
			delayms(10);
				if(key2==0)
				{
					while(!key2);		  //松手检测
					
			
    switch(b){
		case 1:	  	fena++;
						if(fena>=60)
							fena=0;
							clock1[10]=time[fena%10];
                      	    clock1[9]=time[fena/10];  break;

		case 2:	  	shia++;
						if(shia>=24)
							shia=0;
							clock1[7]=time[shia%10];
                      	    clock1[6]=time[shia/10];  break;

		case 3:	  	ria++;
						if(ria>=32)
							ria=0;
							clock1[4]=time[ria%10];
                      	    clock1[3]=time[ria/10];  break;

		case 4:	  	yuea++;
						if(yuea>=13)
							yuea=0;
							clock1[1]=time[yuea%10];
                      	    clock1[0]=time[yuea/10];  break;
                                                              	}	
	                                                                }	   
	                                                                    }
	   
		if(key3==0)			//按键减
			{
				delayms(10);
				if(key3==0)
				{
					while(!key3);		  //松手检测
					
			
    switch(b){
		case 1:	  	fena--;
						if(fena==-1)
							fena=59;
							clock1[10]=time[fena%10];
                      	    clock1[9]=time[fena/10];  break;

		case 2:	  	shia--;
						if(shia==-1)
							shia=23;
							clock1[7]=time[shia%10];
                      	    clock1[6]=time[shia/10];  break;

		case 3:	  	ria--;
						if(ria==0)
							ria=31;
							clock1[4]=time[ria%10];
                      	    clock1[3]=time[ria/10];  break;

		case 4:	  	yuea--;
						if(yuea==0)
							yuea=12;
							clock1[1]=time[yuea%10];
                      	    clock1[0]=time[yuea/10];  break;
	                                                         }	
	                                                            }
	                                      	                      }	   
	                      	   	show(2,1,clock1);               	}		 
	   
	   	        
		if(k!=0)
		{
			if(key2==0)			//按键加
			{
				delayms(10);
				if(key2==0)
				{
					while(!key2);		  //松手检测
					
			
	switch(k){
		case 1:	  	miao++;
						if(miao==60)
							miao=0;
							table1[7]=time[miao%10];
                      	    table1[6]=time[miao/10];  break;
		case 2:	  	fen++;
						if(fen==60)
							fen=0;
							table1[4]=time[fen%10];
                        	table1[3]=time[fen/10];
							                          break;
		case 3:	 	shi++;
						if(shi==24)
							shi=0;
							table1[1]=time[shi%10];
                        	table1[0]=time[shi/10];   break;
		case 4:  ri++;
						if(ri==32)
							ri=0;
							table[9]=time[ri%10];
	                        table[8]=time[ri/10];     break;
		case 5:	 yue++;
						if(yue==12)
							yue=0;
					    	table[6]=time[yue%10];
	                        table[5]=time[yue/10];    break;
		case 6:	 nian++;
				    		table[3]=time[nian%10];
	                        table[2]=time[nian%100/10];
	                        table[1]=time[nian/100%10];
	                        table[0]=time[nian/1000];  break;

			 }		
				}
				   }

		else  if(key3==0)			//按键减
			{
				delayms(10);
				if(key3==0)
				{
					while(!key3);	   //松手检测
					
			
	switch(k){
		case 1:	  	miao--;
						if(miao==-1)
							miao=59;
							table1[7]=time[miao%10];
                      	    table1[6]=time[miao/10];  break;
		case 2:	  	fen--;
						if(fen==-1)
							fen=59;
							table1[4]=time[fen%10];
                        	table1[3]=time[fen/10];
							                          break;
		case 3:	 	shi--;
						if(shi==-1)
							shi=23;
							table1[1]=time[shi%10];
                        	table1[0]=time[shi/10];   break;
		case 4:  ri--;
						if(ri==0)
							ri=31;
							table[9]=time[ri%10];
	                        table[8]=time[ri/10];     break;
		case 5:	 yue--;
						if(yue==0)
							yue=12;
					    	table[6]=time[yue%10];
	                        table[5]=time[yue/10];    break;
		case 6:	 nian--;
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

void delayms(unsigned int xms)
{
        unsigned int i,j;
		for(i=xms;i>0;i--)
		    for(j=110;j>0;j--);
}

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

		    	if(ri>=29){	

				   if (!((nian%400==0)||((nian%100!=0)&&(nian%4==0)))&&(yue==2))	{
				
				      ri=1;
			       	  yue++;     
					                   }
				else if((ri>=30)&&(yue==2)){
				          
						  ri=1;
				          yue++;  
						                 }
			
				else if(ri>=31)
				{
					if(yue==4||yue==6||yue==9||yue==11)
					{
						yue++;ri=1;
					}
					else if(ri>=32)
					{
						yue++;ri=1;
					}
					if(yue>=13) {nian++;yue=1;}
				    }
				}  
			}	
		}
	}
}


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


void waitready()	  //检查lcd是否在忙
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


void writecmd(unsigned char cmd)			//输入指令
{ 
       waitready();
       rs=0;
       rw=0;
       lcd=cmd;
       e=1;
       e=0;  
}


void writedat(unsigned char dat)		 //输入数据
{
       waitready();
       rs=1;
       rw=0;
       lcd=dat;
       e=1;
       e=0;
}

void set(unsigned char x,unsigned char y)	   //确定显示在lcd的哪个位置
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


void initlcd()	   //初始化
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
   	 if(cnt>20){
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
   
