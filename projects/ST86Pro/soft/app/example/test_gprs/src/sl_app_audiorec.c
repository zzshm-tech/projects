#include "sl_type.h"
#include "sl_filesystem.h"
#include "sl_stdlib.h"
#include "sl_debug.h"
#include "sl_error.h"
#include "sl_audiorecord.h"


/****************************************************
                                  function

****************************************************/

void SL_AppAudRecFinishCb(SL_AUDIOREC_INFO* pstAudRecInfo)
{
    SL_ApiPrint("SLAPP:Format %d, Bitrate %d, Time %02d:%02d,Frame %d,Len %d\n", 
         pstAudRecInfo->format, pstAudRecInfo->bitrate,
        (pstAudRecInfo->seconds/60), (pstAudRecInfo->seconds%60), 
         pstAudRecInfo->frames,  pstAudRecInfo->bytelen);

    SL_AudioRecordPlayStart("2.amr");
}

void SL_AppAudRecPlayFinishCb(void)
{
    SL_ApiPrint("SLAPP: play finish ");
    SL_AudioRecordPlayStop();
}

void SL_AppAudRecPlayToOpFinishCb(void)
{
    SL_ApiPrint("SLAPP: play to opposite finish ");
    SL_AudioStopPlayToOpposite();
}

void SL_AppVoiceRecFinishCb(U8* p_data, U32 data_length)
{
    S32 fd = 0;
    S32 slRet = 0;

    SL_ApiPrint("SLAPP:data: %x, ,Len %d\n", p_data, data_length);
    //creat a file
    
    SL_FileDelete("3.pcm");
    fd = SL_FileCreate("3.pcm");
    if(fd < 0)
    {
        SL_ApiPrint("SLAPP: SL_FileDelete fail!");
        return;
    }
    if(SL_FileWrite(fd, p_data, data_length) != data_length)
    {
        SL_ApiPrint("SLAPP: SL_FileWrite fail!");
        return;    
    }
    slRet = SL_FileClose(fd);
    SL_ApiPrint("SLAPP: SL_FileClose ret[%x]!", slRet);
    return;
}

void SL_AppAudioRecInit()
{
    SL_AUDIOREC_CALLBACK stAudioCb;

    SL_Memset(&stAudioCb, 0, sizeof(stAudioCb));
    stAudioCb.slAudioRecRecFinishCb = SL_AppAudRecFinishCb;
    stAudioCb.slAudioRecPlayFinishCb = SL_AppAudRecPlayFinishCb;
    stAudioCb.slAudioRecPlayToOppositeFinishCb = SL_AppAudRecPlayToOpFinishCb;
    stAudioCb.slVoiceRecRecFinishCb = SL_AppVoiceRecFinishCb;
    SL_AudioRecordInit(&stAudioCb);
}

void SL_AppAudioRecStop()
{
    S32 slResult = 0;
    SL_AUDIOREC_INFO stSlAudRecInfo;
    
    slResult = SL_AudioRecordStop(&stSlAudRecInfo);
    if(slResult == SL_RET_OK)
    {        
        SL_ApiPrint("SLAPP:Format %d, Bitrate %d, Time %02d:%02d, Frame %d, Len %d\n", 
             stSlAudRecInfo.format, stSlAudRecInfo.bitrate,
            (stSlAudRecInfo.seconds/60), (stSlAudRecInfo.seconds%60), 
             stSlAudRecInfo.frames, stSlAudRecInfo.bytelen);
        
        SL_AudioRecordPlayStart("2.amr");
    }
}

void SL_AppVoiceRecStart()
{
    S32 slResult = 0;
    slResult = SL_VoiceRecordStart(10);
    SL_ApiPrint("SLAPP:SL_AppVoiceRecStart:%x",slResult); 
}

void SL_AppVoiceRecStop()
{
    S32 slResult = 0;
    slResult = SL_VoiceRecordStop();
    SL_ApiPrint("SLAPP:SL_AppVoiceRecStart:%x",slResult); 
}

void SL_AppVoicePlayStart()
{
    S32 slResult = 0;
    slResult = SL_AudioRecordPlayStart("3.pcm");
    SL_ApiPrint("SLAPP:SL_AppVoicePlayStart:%x",slResult); 
}

