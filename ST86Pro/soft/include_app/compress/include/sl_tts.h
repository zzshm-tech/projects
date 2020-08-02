/*********************************************************************************
 ** File Name:     sl_tts.h                                                *
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
#ifndef _SL_TTS_H_
#define _SL_TTS_H_
/****************************************************
                                   type define
****************************************************/
typedef void (*SL_TTS_PLAY_FINISH_CB)(S32 slResult);
typedef enum _SL_TTS_PLAYER_STATUS
{
    SL_TTS_PLAYER_NOT_INIT,
    SL_TTS_PLAYER_IDLE,
    SL_TTS_PLAYER_SYNTHESIZING,
    SL_TTS_PLAYER_PLAYING,
    SL_TTS_PLAYER_PAUSE
}SL_TTS_PLAY_STATUS;
/****************************************************
                                  function
****************************************************/
#include "sl_app.h"
#define SL_TtsPlay AA.SL_TtsPlay_p
#define SL_TtsStop AA.SL_TtsStop_p
#define SL_TtsQueryStatus AA.SL_TtsQueryStatus_p
#define SL_TtsSetPlaySpeed AA.SL_TtsSetPlaySpeed_p
#endif
