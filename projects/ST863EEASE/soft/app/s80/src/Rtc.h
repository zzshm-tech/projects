
/*****************************************************
**	文件名称:
**	Time:
*****************************************************/

#ifndef RTC_H
#define RTC_H

#include "include.h"


#define  POWERWARN				0	         //外部电源断电报警
#define  POWERLOWWARN			1	         //外部电源电压低报警
#define  MCUSTATE				2            //MCU链接状态报警
#define  SHELLWARN				3            //外壳报警
#define  SPEEDWARN				4			 //行驶速度报警 
#define  LOCATIONWARN			5           //位置报警(保留)
#define  SLEEPWARN				6			//休眠报警

#define  SDWARN 				7        	//SD卡状态
#define  E2PROMWARN  			8			//E2PROM状态
#define  ANTCONWARN				9		    //天线开路报警
#define  ANTSHORTWARN			10           //天线多路报警
#define  CanWARN				11           //CAN断开报警




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


