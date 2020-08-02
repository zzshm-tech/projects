
/*******************************************
**	File Name:
**	Time:
********************************************/

#ifndef SYSTICKTIME_H
#define SYSTICKTIME_H



/******************************************/
struct stopwatch16 
{					
	unsigned short int	 start_time;
};



struct stopwatch32 
{					
	unsigned int	 start_time;
};



/*******************************************/

void init_stopwatch16(struct stopwatch16 *sw);
void init_stopwatch32(struct stopwatch32 *sw);
unsigned short int read_stopwatch16_value(struct stopwatch16 *sw);
unsigned int read_stopwatch32_value(struct stopwatch32 *sw);
void TickAdd(void);





#endif



/***********************File End***********************/
