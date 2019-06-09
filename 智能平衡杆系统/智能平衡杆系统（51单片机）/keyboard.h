#ifndef __keyboard_H__
#define __keyboard_H__
#include "STC12C5A60S2.H"

#define KEY  				P0 		//���󰴼���P0��
#define NO_KEY     0xFF

unsigned char Obtain_key(void);
unsigned char Scan_key(void);
unsigned char Get_key(void);

#endif
