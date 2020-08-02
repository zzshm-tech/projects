/****************************************
**	FileName:VersionNum.h
**	Time:2016.12.18
****************************************/


/******************************************
版本说明:
1、版本分为主板号，次版本号，修正版本，编译版本，
目前只是用主版本加此版本。
2、修改时间:20170328,
	(1)、该版本程序适用老国三小麦机，新国三小麦机，老国二小麦机，
	P平台拖拉机、玉米机五中设备。
	(2)、除去老国二小麦机之外，该版本都已经添加故障码。
	(3)、删除一些无用的代码。
	
*******************************************/


#ifndef VERSIONNUM_H
#define VERSIONNUM_H



#define FIRMWARE_INF   "Homer3x_20170511" 					//固件信息

#define Major_Number        1                               //主版本号
#define Minor_Number        2                               //次版本号
#define Revi_Number                                         //修正版本
#define Build_Number										//编译版本


unsigned char GetVerNum(void);



#endif




/*************************File End*******************/




