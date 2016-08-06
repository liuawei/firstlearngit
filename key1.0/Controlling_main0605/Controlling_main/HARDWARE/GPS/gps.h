#ifndef __GPS_H
#define __GPS_H
#include "sys.h"

#define  TIME_AREA   8	 //Ê±Çø

void GPRMC(u8);
void GPGGA(u8);
void GPVTG(u8);
void GPGSA(u8);
void GPS(u8);
void GPS_display(void);


#endif
