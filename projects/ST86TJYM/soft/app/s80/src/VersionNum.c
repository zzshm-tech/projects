/*******************************************
**	File Name:Version.c
**	Time:2017.05.23
*******************************************/

#include "include.h"

/******************************************
**	函数名称:
**	功能描述:
******************************************/

unsigned char GetVerNum(void)
{
	unsigned char rv;
	
	rv = Major_Number * 10 + Minor_Number;
	//SL_Print("rv is %d\r\n",rv);
	return rv;
}



/*************************************
**	函数名称:
**	功能描述:
*************************************/

void GetFirmWareInf(unsigned char *s,unsigned char strLen)
{

}




/********************File End*********************/



