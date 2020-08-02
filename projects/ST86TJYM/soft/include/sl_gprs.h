/*********************************************************************************
** File Name:     sl_gprs.h                                                *
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
#ifndef _SL_GPRS_H_
#define _SL_GPRS_H_

#include "sl_type.h"

/****************************************************
                                   macro define

****************************************************/
#define SL_GPRS_APN_MAX_LEN      50
#define SL_GPRS_PDPADDR_MAX_LEN  50
#define SL_GPRS_USR_MAX_LEN      10
#define SL_GPRS_PAS_MAX_LEN      10
#define SL_PDPCID_MIN            1
#define SL_PDPCID_MAX            7


/****************************************************
                                   type define

****************************************************/
typedef void (*SL_GPRS_ATT_CB)(U8 ucState, S32 slResult);
typedef void (*SL_GPRS_ACT_CB)(U8 ucCid, U8 ucState, S32 slResult);

typedef struct{
    SL_GPRS_ATT_CB stSlGprsAtt;
    SL_GPRS_ACT_CB stSlGprsAct;
}SL_GPRS_CALLBACK;

typedef enum{
    SL_GPRS_PDP_TYPE_IP = 1
}SL_GPRS_PDP_TYPE;

typedef enum{
    SL_GPRS_DETACHED,
    SL_GPRS_ATTACHED
}SL_GPRS_ATTACH_STATE;

typedef enum{
    SL_GPRS_DEACTIVED,
    SL_GPRS_ACTIVED
}SL_GPRS_CID_STATE;

/***************************************************
                                     function
                                     
***************************************************/
S32 SL_GprsAtt(U8 ucState, SL_GPRS_ATT_CB cb);
S32 SL_GprsSetPdp(U8 ucCid, U8 ucPdpType, U8* pucApn);
S32 SL_GprsAct(U8 ucCid, U8 ucState, SL_GPRS_ACT_CB cb);
S32 SL_GprsGetAttState(U8* pucAttState);
S32 SL_GprsGetActState(U8 ucCid, U8* pucActState);
S32 SL_GprsSetPdpWithUsr(U8 ucCid, U8 ucPdpType, U8* pucApn, U8* pucUsr, U8* pucPasswrd);

#endif
