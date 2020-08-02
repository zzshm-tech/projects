/*********************************************************************************
** File Name:      sl_system.h                                                *
** Author:         Wang Ya Feng                                                  *
** DATE:           2013/01/07                                                   *
** Copyright:      2013 by SimpLight Nanoelectronics,Ltd. All Rights Reserved.  *
*********************************************************************************

*********************************************************************************
** Description:    Implement for common function of API layer                   *
** Note:           None                                                         *
*********************************************************************************

*********************************************************************************
**                        Edit History                                          *
** -------------------------------------------------------------------------    *
** DATE           NAME             DESCRIPTION                                  *
** 2014/05/06    Wang Ya Feng       Create
*********************************************************************************/
#ifndef _SL_EPO_H_
#define _SL_EPO_H_

#include "sl_uart.h"

/****************************************************
                                  macro define

****************************************************/
/****************************************************
                                  type define

****************************************************/
typedef struct
{
	U16 usYear;    /*>2000*/
	U8 ucMonth;
	U8 ucDay;     /*1~31*/
	U8 ucHour;    /*0~23*/
	U8 ucMin;
	U8 ucSec;
}SL_MTK_PARAM_EPO_TIME_CFG;

typedef struct
{
    S32 slLat; //WGS84 geodetic Latidude, -90000000~90000000(1/1000000), Minus:South;Plus:North
    S32 slLon; //WGS84 geodetic Longgitude, -180000000~180000000(1/1000000), Minus:West;Plus:East
    S32 slAlt; //WGS84 ellipsoidal Altitude, 1/100
}SL_MTK_PARAM_EPO_LOC_CFG;

typedef struct
{
    S32 slLat; //WGS84 geodetic Latidude, -90000000~90000000(1/1000000), Minus:South;Plus:North
    S32 slLon; //WGS84 geodetic Longgitude, -180000000~180000000(1/1000000), Minus:West;Plus:East
	S32 slAlt; //WGS84 ellipsoidal Altitude, 1/100
	U16 usYear;
	U8 ucMonth;
	U8 ucDay;
	U8 ucHour;
	U8 ucMin;
	U8 ucSec;
}SL_MTK_PARAM_EPO_POS_CFG;

typedef struct
{
    U32 ulEpoWord[18];/*18 words[LSB first] of one EPO segment data [total 72 bytes]*/
    U8 ucSatId; /*1~32*/
}SL_MTK_PARAM_EPO_DATA_CFG;

typedef enum
{
    MTK_PARAM_CMD_CONFIG_EPO_TIME = 0,
    MTK_PARAM_CMD_CONFIG_EPO_POS,
    MTK_PARAM_CMD_CONFIG_EPO_DATA
}SL_MTK_PARAM_CMD_CONFIG_EPO_TYPE;

/****************************************************
                                  function

****************************************************/
void SL_EpoSetUartIdInUse(SL_UART_ID enUartId);
BOOL SL_IsGpsReadyMsg(U8* pucUartInputStr, U32 len);
BOOL SL_IsEpoDataReady(S32 slYear, S32 slMon, S32 slDay, S32 slHour);
S32 SL_EpoDataSave(U8* pucBuf, U32 ulLenth);
/*!
 * \fn SL_EpoSetDefaultLoc(SL_MTK_PARAM_EPO_LOC_CFG* pstSlEpoLoc)
 * \brief This function set default location info.
 * \param[in] pointer to SL_MTK_PARAM_EPO_LOC_CFG.
 * \return None
 */
void SL_EpoSetDefaultLoc(SL_MTK_PARAM_EPO_LOC_CFG* pstSlEpoLoc);
/*!
 * \fn SL_EpoLocUpdate(SL_MTK_PARAM_EPO_LOC_CFG* pstSlEpoLoc)
 * \brief This function set default location info.
 * \param[in] pointer to SL_MTK_PARAM_EPO_LOC_CFG.
 * \return 
 * \retval SL_RET_OK indicates success;other indicates failure.
 */
S32 SL_EpoLocUpdate(SL_MTK_PARAM_EPO_LOC_CFG* pstSlEpoLoc);
S32 SL_EpoDelData(void);
void SL_EpoProcess(void);

#endif

