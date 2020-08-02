/*!
 *	\file sl_timer.h
 * 	\brief A set of OS level timer manipulation APIs in SL1360
 *
 * 	\details The file defines timer APIs. 
 *	\n
 * \remarks 
 *   These timers belongs to OS, and use OS ticks as timer scale.\n Now SL1300 have 1000/72 ticks in \b 1 second.
 *
 *	\version 		0.1
 *	\author 		SimpLight
 *	\date 			2012-08-20
 * 	\copyright
 *  Copyright (C) 2006-2012 by SimpLight Nanoelectronics, Ltd.\n
 *                 All rights reserved.\n
 * This material constitutes the trade secrets and confidential,\n
 * proprietary information of SimpLight.  This material is not to\n
 * be disclosed, reproduced, copied, or used in any manner  not\n
 * permitted under license from SimpLight Nanoelectronics Ltd.\n
 *
 *
 *	\example ex_timer.c
*/

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
/*! \enum 
*   \brief Define constants for use in timer parameters
*/
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

/*!
 * \fn SL_StartTimer(SL_TASK hTask, U8 timeid, U8 timemode, U32 ticks)
 * \brief Start a timer in timer.
 * \param[in] timeid  timer id you want to start
 * \param[in] timemode  Specifieds the mode for starting timer. This parameter can be one of the following value:SL_TIMER_MODE_SINGLE and SL_TIMER_MODE_PERIODIC
 * \param[in] ticks  time out interval in os ticks. (tick = 1/16384 second)
 * \return start operation success or not
 * \retval  SL_ERR   failed
 * \retval  SL_OK   success
 */
BOOL SL_StartTimer(SL_TASK hTask, U8 timeid, SL_TIMOPT timemode, U32 ticks);

/*!
 * \fn SL_StopTimer(SL_TASK task, U8 timeid)
 * \brief Stop a timer in timer.
 * \param[in] task Specify the task handle.
 * \param[in] timeid  timer id you want to start
 * \return stop operation success or not
 * \retval  SL_ERR   failed
 * \retval  SL_OK   success
 *  \see SL_StartTimer
 */
BOOL SL_StopTimer(SL_TASK hTask, U8 timeid);

U32 SL_SecondToTicks(U32 ulSec);

U32 SL_MilliSecondToTicks(U32 ulMilliSec);

BOOL SL_GetLocalTime(SL_SYSTEMTIME* pSystemTime);

BOOL SL_SetLocalTime(SL_SYSTEMTIME* pSystemTime);

U32 SL_TmGetTick(void);

/*!
 * \fn SL_SetTimeZone(S8 scTimeZone)
 * \brief Set time zone.
 * \param[in] time zone,  should [-12, 12].
 * \return set success or not
 * \retval  TRUE   success
 * \retval  FALS   failed
 */
BOOL SL_SetTimeZone(S8 scTimeZone);
/*!
 * \fn SL_GetTimeZone(void)
 * \brief Get time zone.
 * \param[in] null.
 * \return time zone
 */
S8 SL_GetTimeZone(void);

U32 SL_GetTime(void);

BOOL SL_FileTimeToSystemTime(SL_FILETIME nFileTime, SL_SYSTEMTIME* pSystemTime);

#endif

