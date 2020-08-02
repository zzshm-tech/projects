/*********************************************************************************
** File Name:     sl_dtmf.h                                                *
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
#ifndef _SL_DTMF_H_
#define _SL_DTMF_H_
#include "sl_type.h"
/****************************************************
                                   type define
****************************************************/
typedef void (*SL_DTMF_DECTECT_CB)(U8* pucValue);
typedef enum
{
    SL_DTMF = 1,
    SL_STMF = 2,
    SL_BOTH = 3,
}SL_TMF_DET_TYPE;
/***************************************************
                                     function
***************************************************/
#include "sl_app.h"
#define SL_DtmfStart AA.SL_DtmfStart_p
#define SL_DtmfThreshSet AA.SL_DtmfThreshSet_p
#define SL_DtmfStop AA.SL_DtmfStop_p
#define SL_DtmfSendCode AA.SL_DtmfSendCode_p
#endif
