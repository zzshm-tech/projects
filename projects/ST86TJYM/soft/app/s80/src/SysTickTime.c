
#include <include.h>

unsigned long int ticks = 0;


/*************************************************
**	函数名称:
**	功能描述:
*************************************************/


void init_stopwatch16(struct stopwatch16 *sw)
{
	sw->start_time = (unsigned short int)ticks;
}


/*************************************************
**	函数名称:
**	功能描述:
*************************************************/


void init_stopwatch32(struct stopwatch32 *sw)
{
	sw->start_time = (unsigned int )ticks;
}



/*************************************************
**	函数名称:
**	功能描述:
*************************************************/

unsigned short int read_stopwatch16_value(struct stopwatch16 *sw)
{
	unsigned short int rv;
	rv = (unsigned short int)ticks - sw->start_time;
	return rv;
}


/*************************************************
**	函数名称:
**	功能描述:
*************************************************/

unsigned int read_stopwatch32_value(struct stopwatch32 *sw)
{
	unsigned int rv;
	rv = ticks - sw->start_time;
	return rv;
}


/***********************************************
**	函数名称:
**	功能描述:
************************************************/
void TickAdd(void)

{

	ticks++;
}


/********************************File End********************************/

