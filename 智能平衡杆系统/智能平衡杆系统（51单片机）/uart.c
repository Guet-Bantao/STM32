#include <STC12C5A60S2.H>
#include "uart.h"


void init_uart()
{
    TMOD &= 0x0F;   //清零T1的控制位
    TMOD |= 0x20;   //配置T1为模式2			
		TH1=0xfd;				
		TL1=0xfd;				
		SCON=0x50;		
		TR1=1;
		TI=1;
}