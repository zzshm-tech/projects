
/*************************************************
**	FileName:
** 	Time:
*************************************************/
#ifndef GNSS_H
#define GNSS_H


#include "DataType.h"

#define   GPS_VALID   		1   					// 
#define   GPS_INVALID		0 						// 
	 
#define   NORTH_LATITUDE    1  						//
#define   SOUTH_LATITUDE	0	 					// 
	 
#define   EAST_LONGTITUDE   1  						// 
#define   WEST_LONGTITUDE	0  						// 



unsigned char GnssDataParse(void);
GNSSSTR *GetGnssDataSpace(void);
unsigned char GetDataStr(unsigned char n,unsigned char m,unsigned char *str,unsigned char *source,unsigned char str_len);
double StrToDouble(unsigned char *buf);
unsigned char GetComma(unsigned char num,unsigned char *str,unsigned char str_len);




#endif


/****************************File End***************************/



