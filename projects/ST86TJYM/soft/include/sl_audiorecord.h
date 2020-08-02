/*********************************************************************************
** File Name:     sl_audiorecord.h                                                *
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
#ifndef _SL_AUDIORECORD_H_
#define _SL_AUDIORECORD_H_

#include "sl_type.h"
#include "sl_audio.h"
/****************************************************
                                   macro define

****************************************************/

/****************************************************
                                   type define

****************************************************/
typedef enum {
	SL_AUDIOREC_FORMAT_AMR = 0
} SL_AUDIOREC_FORMAT;

typedef struct {
    SL_AUDIOREC_FORMAT  format;
    SL_AUDIO_STREAM_TYPE  bitrate;
	UINT32				bytelen;
	UINT32				seconds;
	UINT32				frames;
} SL_AUDIOREC_INFO;

typedef void (*SL_AUDIOREC_REC_FINISH_CB)(SL_AUDIOREC_INFO* pstAudRecInfo);
typedef void (*SL_AUDIOREC_PLAY_FINISH_CB)(void);
typedef void (*SL_AUDIOREC_PLAY_TO_OPPOSITE_FINISH_CB)(void);
//voice record fun 20131105
typedef void (*SL_VOICEREC_REC_FINISH_CB)(U8* p_data, U32 data_length);
//voice record fun 20131105
typedef struct {
    SL_AUDIOREC_REC_FINISH_CB slAudioRecRecFinishCb;
    SL_AUDIOREC_PLAY_FINISH_CB slAudioRecPlayFinishCb;
    SL_AUDIOREC_PLAY_TO_OPPOSITE_FINISH_CB slAudioRecPlayToOppositeFinishCb;    
//voice record fun 20131105
    SL_VOICEREC_REC_FINISH_CB slVoiceRecRecFinishCb;
//voice record fun 20131105
}SL_AUDIOREC_CALLBACK;

/***************************************************
                                     function
                                     
***************************************************/
S32 SL_AudioRecordInit(SL_AUDIOREC_CALLBACK* cb);
S32 SL_AudioRecordStart(U8* pucfilename);
S32 SL_AudioRecordStop(SL_AUDIOREC_INFO* pstSlAudRecInfo);
S32 SL_AudioRecordPlayStart(U8* pucfilename);
S32 SL_AudioRecordPlayStop(void);
S32 SL_AudioPlayToOpposite(U8* pucfilename, U8 ucLoopFlag);
S32 SL_AudioStopPlayToOpposite(void);
//voice record fun 20131105
S32 SL_VoiceRecordStart(U8 time);
S32 SL_VoiceRecordStop();
//voice record fun 20131105

//play tts to Opposite  20140303
//#if (SL_TTS == 1)
S32 SL_AudioPlayTTSToOpposite(U8* pucText, U32 ulSize);
S32 SL_AudioStopPlayTTSToOpposite(void);
//#endif
//play tts to Opposite  20140303

#endif
