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
S32 SL_DtmfStart(SL_DTMF_DECTECT_CB cb, SL_TMF_DET_TYPE enSlTmfType);
S32 SL_DtmfThreshSet(U8 ucThresh1, U8 ucThresh2);
S32 SL_DtmfStop(void);
S32 SL_DtmfSendCode(U8 ucCodeNum, U8* pucCode, U32* pucSendTimeMs, U32* pucMuteTimeMs);

#endif
