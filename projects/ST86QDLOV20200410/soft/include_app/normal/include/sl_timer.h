/**********************************************************************************
 **                        Edit History                                                    *
 ** ------------------------------------------------------    *
 ** DATE           NAME             DESCRIPTION                                  *
 ** 2012/08/20     Fan Xiaoyue		  Create
 *********************************************************************************/
#ifndef _SL_TIMER_H_
#define _SL_TIMER_H_
#include "sl_type.h"
#include "sl_os.h"
typedef enum
{
	SL_TIMER_MODE_SINGLE = 0,		/*!< Produce an event after a given interval time  */
	SL_TIMER_MODE_PERIODIC,			/*!< Produce an event every time the given period is passed  */
}SL_TIMOPT;
typedef struct _SL_FILETIME { 
	UINT32 DateTime;
}SL_FILETIME, *SL_PFILETIME;
typedef struct _SL_SYSTEMTIME { 
    U16 uYear;
    U8  uMonth;
    U8  uDayOfWeek;
    U8  uDay;
    U8  uHour;
    U8  uMinute;
    U8  uSecond;
    U16 uMilliseconds;
} SL_SYSTEMTIME, *SL_PSYSTEMTIME;
#include "sl_app.h"
#define SL_StartTimer AA.SL_StartTimer_p
#define SL_StopTimer AA.SL_StopTimer_p
#define SL_SecondToTicks AA.SL_SecondToTicks_p
#define SL_MilliSecondToTicks AA.SL_MilliSecondToTicks_p
#define SL_GetLocalTime AA.SL_GetLocalTime_p
#define SL_SetLocalTime AA.SL_SetLocalTime_p
#define SL_TmGetTick AA.SL_TmGetTick_p
#define SL_SetTimeZone AA.SL_SetTimeZone_p
#define SL_GetTimeZone AA.SL_GetTimeZone_p
#define SL_GetTime AA.SL_GetTime_p
#define SL_FileTimeToSystemTime AA.SL_FileTimeToSystemTime_p
#endif
