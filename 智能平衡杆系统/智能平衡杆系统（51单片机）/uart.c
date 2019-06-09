#include <STC12C5A60S2.H>
#include "uart.h"


void init_uart()
{
    TMOD &= 0x0F;   //����T1�Ŀ���λ
    TMOD |= 0x20;   //����T1Ϊģʽ2			
		TH1=0xfd;				
		TL1=0xfd;				
		SCON=0x50;		
		TR1=1;
		TI=1;
}