/*********************************************************************************
** File Name:     sl_fm.h                                                *
** Author:         Wang Ya Feng                                                  *
** DATE:           2013/01/16                                                   *
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
** 2013/01/16     Wang Ya Feng       Create
*********************************************************************************/
#ifndef _SL_FM_H_
#define _SL_FM_H_

#include "sl_type.h"

/****************************************************
                                   type define

****************************************************/
typedef enum
{
    SL_FM_PATH_HP,
    SL_FM_PATH_LSP_HP    
}SL_FM_PATH_TYPE;

// =============================================================================
// SL_FM_ANA_LEVEL_TYPE
// -----------------------------------------------------------------------------
/// This type describes the possible level value for the analog output volume.
// =============================================================================

typedef enum
{
    SL_FM_ANA_MUTE = 0,
    SL_FM_ANA_VOL_1,
    SL_FM_ANA_VOL_2,
    SL_FM_ANA_VOL_3,
    SL_FM_ANA_VOL_4,
    SL_FM_ANA_VOL_5,
    SL_FM_ANA_VOL_6,
    SL_FM_ANA_VOL_7,
    SL_FM_ANA_VOL_QTY
} SL_FM_ANA_LEVEL_TYPE;

/****************************************************
                                   function

****************************************************/
S32 SL_FmOpen(VOID);
S32 SL_FmPowerOff(VOID);
S32 SL_FmSetFreq(U16 usCurfreqHundKHz);
S32 SL_FmGetSignalLevel(U16 usCurfreqHundKHz);
S32 SL_FmSetOutputDevice(U8 ucDevice);
S32 SL_FmSetVolume(U8 ucVolume, BOOL bBassBoost, BOOL bForceMono);
S32 SL_FmSeek(BOOL bSeekUp);
BOOL SL_FmValibStop(U16 ucFreqHundKHz);
S32 SL_FmGetFreq(U32 *pFreqKHz);
void SL_FmPlay(void);

#endif
