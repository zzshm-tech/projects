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
#include "sl_app.h"
#define SL_AssistGpsConfig AA.SL_AssistGpsConfig_p
#define SL_AssistGpsGetLoc AA.SL_AssistGpsGetLoc_p
#define SL_AssistGpsArpConfig AA.SL_AssistGpsArpConfig_p
#define SL_AssistGpsGetAddr AA.SL_AssistGpsGetAddr_p
#define SL_AssistGpsGetUtc AA.SL_AssistGpsGetUtc_p
#endif
