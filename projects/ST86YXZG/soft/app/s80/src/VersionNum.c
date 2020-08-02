/*******************************************
**	File Name:Version.c
**	Time:2017.05.23
*******************************************/

#include "include.h"

/******************************************
**	函数名称:unsigned char GetVerNum(void)
**	功能描述:
******************************************/

unsigned char GetVerNum(void)
{
	unsigned char rv;
	
	rv = Major_Number * 10 + Minor_Number;
	
	return rv;
}



/*************************************
**	获取相关版本信息
**	
*************************************/

void GetFirmWareInf(unsigned char *s,unsigned char strLen)
{
	
}




/********************File End*********************/



