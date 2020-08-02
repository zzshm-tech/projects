/*********************************************************************************
** File Name:     sl_audio.h                                                *
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
#ifndef _SL_AUDIO_H_
#define _SL_AUDIO_H_
#include "sl_type.h"
/******************************************
           type define
******************************************/
typedef void (*SL_AUDIO_PLAY_FINISH)(S32 slErrCode);
// =============================================================================
// AUDIO_PATH_T
// -----------------------------------------------------------------------------
/// List of audio path
// =============================================================================
typedef enum
{
    SL_AUDIO_PATH_RECEIVER,
    SL_AUDIO_PATH_HEADSET,
    SL_AUDIO_PATH_LOUD_SPEAKER,
    SL_AUDIO_PATH_INVALID
} SL_AUDIO_PATH_TYPE;
// ============================================================================
// SPK_LEVEL_T
// -----------------------------------------------------------------------------
/// This type describes the possible level value for the speaker volume.
// =============================================================================
typedef enum
{
    SL_SPK_MUTE                                = 0x00000000,
    SL_SPK_VOL_1                               = 0x00000001,
    SL_SPK_VOL_2                               = 0x00000002,
    SL_SPK_VOL_3                               = 0x00000003,
    SL_SPK_VOL_4                               = 0x00000004,
    SL_SPK_VOL_5                               = 0x00000005,
    SL_SPK_VOL_6                               = 0x00000006,
    SL_SPK_VOL_7                               = 0x00000007,
    SL_SPK_VOL_QTY                             = 0x00000008
} SL_SPK_LEVEL_TYPE;
// ============================================================================
// MIC_LEVEL_T
// -----------------------------------------------------------------------------
/// This type describes the possible level value for the mic: mute or enabled.
// =============================================================================
typedef enum
{
    SL_MIC_MUTE                                = 0x00000000,
    SL_MIC_ENABLE                              = 0x00000001,
    SL_MIC_VOL_QTY                             = 0x00000002
} SL_MIC_LEVEL_TYPE;
// ============================================================================
// SIDE_LEVEL_T
// -----------------------------------------------------------------------------
/// This type describes the possible level value for the side tone volume. 
// =============================================================================
typedef enum
{
    SL_AUDIO_SIDE_MUTE = 0, // mute
    SL_AUDIO_SIDE_m36dB, //  min
    SL_AUDIO_SIDE_m33dB,
    SL_AUDIO_SIDE_m30dB,
    SL_AUDIO_SIDE_m27dB,
    SL_AUDIO_SIDE_m24dB,
    SL_AUDIO_SIDE_m21dB,
    SL_AUDIO_SIDE_m18dB,
    SL_AUDIO_SIDE_m15dB,
    SL_AUDIO_SIDE_m12dB,
    SL_AUDIO_SIDE_m9dB,
    SL_AUDIO_SIDE_m6dB,
    SL_AUDIO_SIDE_m3dB,
    SL_AUDIO_SIDE_0dB,
    SL_AUDIO_SIDE_3dB,
    SL_AUDIO_SIDE_6dB, //  max
    SL_AUDIO_SIDE_GAIN_QTY
} SL_AUDIO_SIDE_LEVEL_TYPE;
// ============================================================================
//TONE_LEVEL_T
// -----------------------------------------------------------------------------
/// This type describes the possible level value for the tone volume. 
// =============================================================================
typedef enum
{
    SL_AUDIO_TONE_0dB = 0,
    SL_AUDIO_TONE_m3dB,
    SL_AUDIO_TONE_m9dB,
    SL_AUDIO_TONE_m15dB,
    SL_AUDIO_TONE_GAIN_QTY
} SL_AUDIO_TONE_LEVEL_TYPE;
typedef enum
{
    SL_SPK,
    SL_MIC,
    SL_SID,
    SL_TONE
}SL_VOLUME_TYPE;
typedef enum
{
    SL_AUDIO_TYPE_AMR = 3,
    SL_AUDIO_TYPE_PCM_8K = 7,
    SL_AUDIO_TYPE_PCM_16K = 8,
    SL_AUDIO_TYPE_WAV = 13,
    SL_AUDIO_TYPE_MP3 = 25,
    SL_AUDIO_TYPE_MIDI = 30
}SL_AUDIO_TYPE;
typedef enum
{
    SL_AUDIO_STREAM_AMR475,
    SL_AUDIO_STREAM_AMR515,
    SL_AUDIO_STREAM_AMR59,
    SL_AUDIO_STREAM_AMR67,
    SL_AUDIO_STREAM_AMR74,
    SL_AUDIO_STREAM_AMR795,
    SL_AUDIO_STREAM_AMR102,
    SL_AUDIO_STREAM_AMR122,
    SL_AUDIO_STREAM_FR,
    SL_AUDIO_STREAM_HR,
    SL_AUDIO_STREAM_EFR,
    SL_AUDIO_STREAM_PCM,
    // TODO Implement that mode
    SL_AUDIO_STREAM_AMR_RING,
    SL_AUDIO_STREAM_MP3,
    SL_AUDIO_STREAM_AAC,
    SL_AUDIO_STREAM_WAV,
    SL_AUDIO_STREAM_MID,
    SL_AUDIO_STREAM_PCM_TTS, //for TTS stream play
    SL_AUDIO_STREAM_QTY
}SL_AUDIO_STREAM_TYPE;
#include "sl_app.h"
#define SL_AudioStartPlayFile AA.SL_AudioStartPlayFile_p
#define SL_AudioStopPlayFile AA.SL_AudioStopPlayFile_p
#define SL_AudioPlayStream AA.SL_AudioPlayStream_p
#define SL_AudioStopPlayStream AA.SL_AudioStopPlayStream_p
#define SL_AudioPlayPause AA.SL_AudioPlayPause_p
#define SL_AudioSetVolume AA.SL_AudioSetVolume_p
#define SL_AudioSetPathVolume AA.SL_AudioSetPathVolume_p
#define SL_AudioChangePath AA.SL_AudioChangePath_p
#define SL_AudioSetMicGain AA.SL_AudioSetMicGain_p
#define SL_AudioSetSideToneGain AA.SL_AudioSetSideToneGain_p
#define SL_AudioGetMicGain AA.SL_AudioGetMicGain_p
#define SL_AudioGetSideToneGain AA.SL_AudioGetSideToneGain_p
#define SL_AudioVts AA.SL_AudioVts_p
#endif
