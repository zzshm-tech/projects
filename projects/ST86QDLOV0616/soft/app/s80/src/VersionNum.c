/*******************************************
**	File Name:Version.c
**	Time:2017.05.23
*******************************************/

#include "include.h"

/******************************************
**	
******************************************/

unsigned char GetVerNum(void)
{
	unsigned char rv;
	
	rv = Major_Number * 10 + Minor_Number;
	
	return rv;
}



/********************File End*********************/



