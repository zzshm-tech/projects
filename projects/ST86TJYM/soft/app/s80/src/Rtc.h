
/*****************************************************

**	FileName:
**	Time:
**  
*****************************************************/

#ifndef RTC_H
#define RTC_H

#include "include.h"


#define  POWERWARN				0	         //
#define  POWERLOWWARN			1	         //
#define  MCUSTATE				2            //
#define  SHELLWARN				3            //
#define  SPEEDWARN				4			 //
#define  LOCATIONWARN			5           //
#define  SLEEPWARN				6			//

#define  SDWARN 				7        	//
#define  E2PROMWARN  			8			//
#define  ANTCONWARN				9		    //
#define  ANTSHORTWARN			10           //
#define  CanWARN				11           //




SysTimeStr UnixToCalendar(unsigned int t);
unsigned int CalendarToUnix(SysTimeStr t);
void ProcessRtc(void);
SysTimeStr *GetSysTimeSpace(void);
unsigned char ReadSleepStatus(void);
void ProcessSysSleep(void);
void SetSysTime(unsigned int Time);
SysTimeStr *GetSetTimeSpace(void);
unsigned int GetWarningValue(void);
void WriteDeviceAlram(char bit,unsigned char n);
void WriteSleepStatus(unsigned char n);


#endif






/*******************************File End**************************/


