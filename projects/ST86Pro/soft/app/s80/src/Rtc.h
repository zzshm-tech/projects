
/*****************************************************
**	ÎÄ¼þÃû³Æ:
**	Time:
*****************************************************/

#ifndef RTC_H
#define RTC_H

#include "include.h"



SysTimeStr UnixToCalendar(unsigned int t);
unsigned int CalendarToUnix(SysTimeStr t);
void ProcessRtc(void);
SysTimeStr *GetSysTimeSpace(void);
unsigned char ReadSleepStatus(void);
void ProcessSysSleep(void);
void SetSysTime(unsigned int Time);
SysTimeStr *GetSetTimeSpace(void);


#endif






/*******************************File End**************************/


