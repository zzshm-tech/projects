/*********************************************************************************
** File Name:     sl_assistgps.h                                                *
** Author:         Wang Ya Feng                                                  *
** DATE:           2013/03/18                                                   *
** Copyright:      2013 by SimpLight Nanoelectronics,Ltd. All Rights Reserved.  *
*********************************************************************************

*********************************************************************************
** Description:    Implement for common function of API layer                   *
** Note:           None                                                         *
*********************************************************************************

*********************************************************************************
**                        Edit History                                          *
** -------------------------------------------------------------------------    *
** DATE                   NAME             DESCRIPTION                                  *
** 2013/03/18     Wang Ya Feng       Create
*********************************************************************************/
#ifndef _SL_ASSISTGPS_H_
#define _SL_ASSISTGPS_H_

#include "sl_type.h"

/****************************************************
                                   type define
                                   
****************************************************/
/*!
 * \fn void (*SL_ASSISTGPS_GET_LOC_CB)(S32 slResult, U32* pulLongitude, U32* pulLatitude)
 * \brief get longitude and latitude callback
 * \param[in] result: get success or fail
 * \param[in] ulLongitude:  longitude
 * \param[in] ulLatitude:  latitude
 * \return None
 * \attention the precision of longitude and latitude is 1/1000000.
 */
typedef void (*SL_ASSISTGPS_GET_LOC_CB)(S32 slResult, U32 ulLongitude, U32 ulLatitude);
typedef void (*SL_ASSISTGPS_GET_ADDR_CB)(S32 slResult, U8* pucbuf, U16 usLenth);

typedef struct{
    SL_ASSISTGPS_GET_LOC_CB slAssistGpsGetLocCb;
    SL_ASSISTGPS_GET_ADDR_CB slAssistGpsGetAddrCb;
}SL_ASSISTGPS_CALLBACK;

typedef struct
{
	S32	year;
	S32	month;
	S32	day;
	S32	hour;
	S32	minute;
	S32	second;
	S32	valid;
}SL_ASSISTGPS_UTC_t;

/****************************************************
                                   function

****************************************************/
S32 SL_AssistGpsConfig(S8* pucServername, U16 usPort);
S32 SL_AssistGpsGetLoc(SL_ASSISTGPS_GET_LOC_CB cb);

/*!
 * \fn S32 SL_AssistGpsArpConfig(U32 uiServer,, U32 uiLanguage)
 * \brief get address according to longitude and latitude
 * \param[in] U32 uiServer: Arp server. Support sogou(0, China) and google(1, Global).
 * \param[in] uiLanguage:  English(0) or Chinese(1). Only support chinese address for sogou server. Google's charset is UTF-8.
 * \return S32
 */
S32 SL_AssistGpsArpConfig(U32 uiServer, U32 uiLanguage);

/*!
 * \fn S32 SL_AssistGpsGetAddr(U32 ulLongitude, U32 ulLatitude, SL_ASSISTGPS_GET_ADDR_CB cb)
 * \brief get address according to longitude and latitude
 * \param[in] ulLongitude: longitude
 * \param[in] ulLatitude:  latitude
 * \param[in] cb: get address callback
 * \return S32
 * \attention the precision of longitude and latitude is 1/1000000.
 */
S32 SL_AssistGpsGetAddr(U32 ulLongitude, U32 ulLatitude, SL_ASSISTGPS_GET_ADDR_CB cb);

/*!
 * \fn S32 SL_AssistGpsGetUtc(SL_ASSISTGPS_UTC_t *pUtcTime)
 * \brief get UTC time after agps position success. 
 * \param[in] SL_ASSISTGPS_UTC_t *pUtcTime: copy agps utc time to pointer pUtcTime
 * \return S32
 */
S32 SL_AssistGpsGetUtc(SL_ASSISTGPS_UTC_t *pUtcTime);

#endif
