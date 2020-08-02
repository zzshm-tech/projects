/**********************************************************************************
 **                        Edit History                                          *
 ** -------------------------------------------------------------------------    *
 ** DATE           NAME             DESCRIPTION                                  *
 ** 2012/08/21		Fan Xiaoyue			Create
 *********************************************************************************/
#ifndef _SL_RTC_H_
#define _SL_RTC_H_
#include "sl_type.h"
/************************************************
                           macro define
************************************************/
#define SL_RTC_MAX_ALARMTEXTLEN		32
/************************************************
                           struc define
************************************************/
typedef struct
{
    U16 usYear;
    U8 ucMonth;
    U8 ucDayOfWeek;
    U8 ucDay;
    U8 ucHour;
    U8 ucMinute;
    U8 ucSecond;
    U16 usMilliseconds;
} SL_RTC_TIME;
typedef enum
{
    SL_EVERYDAY = 0x0,
    SL_ONCE = 0x1,
    SL_MONDAY = 0x2,
    SL_TUESDAY = 0x4,
    SL_WEDNESDAY = 0x8,
    SL_THURSDAY = 0x10,
    SL_FRIDAY = 0x20,
    SL_SATURDAY = 0x40,
    SL_SUNDAY = 0x80,
    SL_EXCEPTWEEKEND = 0x2E,
    SL_ALLWEEKDAY = 0xFE
}SL_RTC_WEEKDAY;
typedef enum
{
    SL_ALARM_VOICE = 0x1,	/* <! Make voice when alarm comes */
    SL_ALARM_VIBRATE = 0x2,		/* <! Make vibrate when alarm comes */
    SL_ALARM_SPLASH = 0x4,		/* <! Screen splash when alarm comes */
    SL_ALARM_SILENCE = 0x8,		/* <! No flag when alarm comes */
}SL_RTC_ALARM_TYPE;
typedef struct
{
    SL_RTC_TIME *pstAlarmTime;	/* <! utc time structure pointer */
    U8 *pucAlarmText;				/* <! Specify the string pointer value 
										without zero-terminated indicating 
										the text to be displayed when alarm time is reached. 
										This member maybe NULL if you don't specify 
										the display text for alarm. The maximum size is 32 Bytes*/
    U8 ucAlarmIdx;				/* <! Specify the index of the alarm, index starts with 1
  									and rang 1 to 15 */
    SL_RTC_ALARM_TYPE enAlarmType;		/* <! Specify the type of the alarm. */
    SL_RTC_WEEKDAY enAlarmWeekDay;	/* <! mask days in one week mask */
}SL_RTC_ALARM;
typedef void (*SL_ALM_CB)(U8 *pucAlarmText, U8 ucAlarmIdx, SL_RTC_ALARM_TYPE enAlarmType, SL_RTC_WEEKDAY enAlarmWeekDay);
#include "sl_app.h"
#define SL_RtcSet AA.SL_RtcSet_p
#define SL_RtcGet AA.SL_RtcGet_p
#define SL_RtcSetAlarm AA.SL_RtcSetAlarm_p
#define SL_RtcDelAlarm AA.SL_RtcDelAlarm_p
#define SL_RtcClrAllAlarm AA.SL_RtcClrAllAlarm_p
#define SL_RtcGetLastErr AA.SL_RtcGetLastErr_p
#define SL_RtcSetAlmCb AA.SL_RtcSetAlmCb_p
#endif
