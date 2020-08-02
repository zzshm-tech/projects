/****************************************
**	FileName:VersionNum.h
**	Time:2016.12.18
****************************************/


/******************************************
**
	
*******************************************/


#ifndef VERSIONNUM_H
#define VERSIONNUM_H



#define FIRMWARE_INF   "YX-Homer3b-3-20200310" 				//固件信息

#define Major_Number        1                               //主版本号
#define Minor_Number        8                               //次版本号
#define Revi_Number                                         //
#define Build_Number										//


unsigned char GetVerNum(void);
void GetFirmWareInf(unsigned char *s,unsigned char strLen);





#endif




/*************************File End*******************/




