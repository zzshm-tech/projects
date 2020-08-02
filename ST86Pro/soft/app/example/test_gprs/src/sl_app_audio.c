#include "sl_type.h"
#include "sl_debug.h"
#include "sl_error.h"
#include "sl_filesystem.h"
#include "sl_audio.h"

HANDLE FileHandle = 0;
/*****************AUDIO***************/

void SL_AppAudioPlayFinish(S32 slResult)
{
    SL_ApiPrint("SLAPP: SL_AppAudioPlayFinish ret[%d]!", slResult);
    SL_FileClose(FileHandle);
}

void SL_AppPlayAudio(U8* pucFileName)
{
    //S32 h = HNULL;
    S32 slFileLenth = 0;
    S32 slReadLenth = 0;
    S32 slRet = 0;
    void* pData = NULL;
        
    //SL_Sleep(2000);
    FileHandle = SL_FileOpen(pucFileName,SL_FS_RDONLY);
    if(FileHandle < 0)
    {
        SL_ApiPrint("SLPP: SL_AppTask3 file open fail[%d]\n", FileHandle);
        return;
    }
#if 1
    slFileLenth = SL_FileGetSize(FileHandle);
    pData = SL_GetMemory(slFileLenth);
    if(pData == NULL)
    {
        SL_ApiPrint("SLAPP: SL_GetMemory fail!");
        return;
    }
    slReadLenth = SL_FileRead(FileHandle, pData, slFileLenth);
    if(slReadLenth < 0)
    {
        SL_ApiPrint("SLAPP: SL_FileWrite fail!");
        slRet = SL_FileClose(FileHandle);
        SL_ApiPrint("SLAPP: SL_FileClose ret[%x]!", slRet);
        return;
    }
    
    SL_ApiPrint("SLAPP: SL_FileOpen ok handle[%d]!", FileHandle);
    SL_FileClose(FileHandle);
    SL_AudioSetPathVolume(7);
    //SL_AudioChangePath(SL_AUDIO_PATH_LOUD_SPEAKER);
    SL_AudioPlayStream(pData, slFileLenth/4, SL_AUDIO_STREAM_AMR_RING, 0, SL_AppAudioPlayFinish);
    SL_AudioChangePath(SL_AUDIO_PATH_LOUD_SPEAKER);
    //SL_AudioStopPlayStream();
    if(pData != NULL)
    {
        SL_FreeMemory(pData);
    }
#else
    SL_AudioSetPathVolume(7);
    SL_AudioStartPlayFile(FileHandle, SL_AUDIO_TYPE_AMR, SL_AUDIO_PATH_LOUD_SPEAKER, SL_AppAudioPlayFinish);   
    SL_Sleep(1000);
    SL_AudioPlayPause(1);
    SL_AudioChangePath(SL_AUDIO_PATH_RECEIVER);
    SL_Sleep(5000);
    SL_AudioPlayPause(0);

#endif

}

