
/*****************************************************

**	�ļ�����:
**	Time:
*****************************************************/

#ifndef RTC_H
#define RTC_H

#include "include.h"


#define  POWERWARN				0	         //�ⲿ��Դ�ϵ籨��
#define  POWERLOWWARN			1	         //�ⲿ��Դ��ѹ�ͱ���
#define  MCUSTATE				2            //MCU����״̬����
#define  SHELLWARN				3            //��Ǳ���
#define  SPEEDWARN				4			 //��ʻ�ٶȱ��� 
#define  LOCATIONWARN			5           //λ�ñ���(����)
#define  SLEEPWARN				6			//���߱���

#define  SDWARN 				7        	//SD��״̬
#define  E2PROMWARN  			8			//E2PROM״̬
#define  ANTCONWARN				9		    //���߿�·����
#define  ANTSHORTWARN			10           //���߶�·����
#define  CanWARN				11           //CAN�Ͽ�����




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
unsigned char GetSysTimerString(unsigned char *buf);
unsigned char GetSysRtcString(unsigned char *buf);
#endif






/*******************************File End**************************/


