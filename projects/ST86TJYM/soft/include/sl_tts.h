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
S32 SL_TtsPlay(U8* pucText, U32 ulSize, SL_TTS_PLAY_FINISH_CB stPlayCb);
S32 SL_TtsStop(void);
S32 SL_TtsQueryStatus(void);
S32 SL_TtsSetPlaySpeed(U8 ucTtsSpeed);

#endif
