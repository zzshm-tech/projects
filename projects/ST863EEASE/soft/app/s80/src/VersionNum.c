/*******************************************
**	File Name:Version.c
**	Time:2017.05.23
*******************************************/

#include "include.h"

/******************************************
**	函数名称:unsigned char GetVerNum(void)
**	功能描述:返回系统版本号
******************************************/

unsigned char GetVerNum(void)
{
	unsigned char rv;
	
	rv = Major_Number * 10 + Minor_Number;
	
	return rv;
}



/********************File End*********************/



