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
#include "sl_app.h"
#define SL_GprsAtt AA.SL_GprsAtt_p
#define SL_GprsSetPdp AA.SL_GprsSetPdp_p
#define SL_GprsAct AA.SL_GprsAct_p
#define SL_GprsGetAttState AA.SL_GprsGetAttState_p
#define SL_GprsGetActState AA.SL_GprsGetActState_p
#define SL_GprsSetPdpWithUsr AA.SL_GprsSetPdpWithUsr_p
#endif
