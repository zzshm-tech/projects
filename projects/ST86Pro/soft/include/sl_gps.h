/*********************************************************************************
** File Name:     sl_gps.h                                                         *
** Author:         likai                                                               *
** DATE:           2013/12/31                                                   *
** Copyright:      2013 by SimpLight Nanoelectronics,Ltd. All Rights Reserved.  *
*********************************************************************************

*********************************************************************************
** Description:    Implement for common function of API layer                   *
** Note:             None                                                         *
*********************************************************************************

*********************************************************************************
**                        Edit History                                          *
** -------------------------------------------------------------------------    *
** DATE                   NAME             DESCRIPTION                                  *
** 2013/12/31         likai               Create
*********************************************************************************/
#ifndef _SL_GPS_H_
#define _SL_GPS_H_
#include "sl_type.h"

/****************************************************
                                   type define
****************************************************/                                   
//callback function
typedef void (*SL_GPS_GET_CB)(CHAR* p_gps_info);

typedef struct{
    SL_GPS_GET_CB slGpsGetCb;
}SL_GPS_CALLBACK;

typedef struct{
	U8 *nmea_buf;
	U32 buf_len;
	U32 ret_len;
	S32 valid;
}SL_GPS_NMEA_BUF_t;


/****************************************************
                                   function

****************************************************/
//set mode: 0 power off 1 power on
S32 SL_GpsModeConfig(S8 mode);
//get GPS info
S32 SL_GpsGet(U32 timeout, SL_GPS_GET_CB cb);
//get GPS original nmea info. It can not be invoked with SL_GpsGet() at the same time.
S32 SL_GpsGetOriginalNmea(SL_GPS_NMEA_BUF_t *info);

#endif
