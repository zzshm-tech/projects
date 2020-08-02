/*!
 *	\file sl_rtc_extr.h
 * 	\brief A set of RTC manipulation APIs in SL1360.
 *
 * 	\details The file defines RTC APIs. Developpers can use functions to set/get RTC value.
 *
 *	\n
 * 	<h2>RTC Operations</h2>
 *	You can use SL_RtcSet() to set RTC value through value get from GPS, and retrieve the RTC value
 *	by call function SL_RtcGet().
 *
 *	\version 		0.1
 *	\author 		SimpLight
 *	\date 		2012-08-21
 * 	\copyright
 *  Copyright (C) 2006-2012 by SimpLight Nanoelectronics, Ltd.\n
 *                 All rights reserved.\n
 * This material constitutes the trade secrets and confidential,\n
 * proprietary information of SimpLight.  This material is not to\n
 * be disclosed, reproduced, copied, or used in any manner  not\n
 * permitted under license from SimpLight Nanoelectronics Ltd.\n
 */

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

/*! \struct struct_utc_time
 *  \brief utc time structure, only UTC time is valid for this structure.
 */
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

/*! \enum enum_weekdays_mask
 *  \brief days in one week mask
 */
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

/*! \enum enum_alarm_t
 *  \brief alarm type mask
 */
typedef enum
{
    SL_ALARM_VOICE = 0x1,	/* <! Make voice when alarm comes */
    SL_ALARM_VIBRATE = 0x2,		/* <! Make vibrate when alarm comes */
    SL_ALARM_SPLASH = 0x4,		/* <! Screen splash when alarm comes */
    SL_ALARM_SILENCE = 0x8,		/* <! No flag when alarm comes */
}SL_RTC_ALARM_TYPE;

/*! \struct SL_Alarm_t
 *  \brief alarm structure.
 */
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
/*!
 * \fn VOID SL_RtcSet(struct_utc_time* utcTime)
 * \brief Sets RTC with the time specified by the utcTime structure \n
 *   Must transfer local time to UTC time according to time zone before calling SL_RtcSet
 * \param[in] utcTime  utc time structure pointer
 * \return Set UTC time result
 * \retval SL_OK UTC time setting succeed.
 * \retval SL_ERR UTC time setting failed.
 * \see SL_RtcGet
 */

S32 SL_RtcSet(SL_RTC_TIME* pstSlRtcTime);

/*!
 * \fn VOID SL_RtcGet(struct_utc_time* utcTime)
 * \brief Get RTC time with pointer utcTime\n
 * To get local time, time zone offset should be added.
 * \param[out] utcTime  utc time pointer
 * \return 
 * \retval	SL_OK  utcTime is valid
 * \retval  SL_ERR utcTime is invalid
 * \see SL_RtcSet
 */
S32 SL_RtcGet(SL_RTC_TIME* pstSlRtcTime);

/*!
 * \fn SL_RtcSetAlarm(SL_Alarm_t *alarm)
 * \brief Enable alarm function with RTC, alarm comes in a circular manner.
 * \param[in] alarm Specify alarm pointer
 * \return Alarm setting result
 * \retval SL_OK Alarm setting succeed.
 * \retval SL_ERR Alarm setting failed.
 */
S32 SL_RtcSetAlarm(SL_RTC_ALARM *pstSlRtcAlarm);

/*!
 * \fn SL_RtcDelAlarm(UINT8 idx)
 * \brief Disable alarm function with RTC, and delete it from alarm list.
 * \param[in] Alarm index.
 * \return Delete result
 * \retval SL_OK Alarm deletes succeed.
 * \retval SL_ERR Alarm deletes failed.
 */
S32 SL_RtcDelAlarm(U8 ucIdx);

/*!
 * \fn SL_RtcClrAllAlarm(VOID)
 * \brief Delete all alarms in the alarm list
 * \param[in] None
 * \return Result of service
 * \retval SL_OK Delete succeed.
 * \retval SL_ERR Delete failed.
 */
S32 SL_RtcClrAllAlarm(VOID);

/*!
 * \fn SL_RtcGetLastErr(VOID);
 * \brief Get last error code in RTC.
 * \param[in] None
 * \return Error code defined in sl_errcode_extr.h
 */
S32 SL_RtcGetLastErr(VOID);

/*!
 * \fn SL_RtcSetAlmCb(SL_ALM_CB cb)
 * \brief Register rtc callback function
 * \param[in] callback Specify rtc indicate function as SL_ALM_CB structure.
 * \return Register result
 * \retval SL_RET_OK Register succeed.
 */
S32 SL_RtcSetAlmCb(SL_ALM_CB cb);

#endif
