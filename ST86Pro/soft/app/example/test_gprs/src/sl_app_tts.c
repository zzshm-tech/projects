#include "sl_type.h"
#include "sl_debug.h"
#include "sl_stdlib.h"
#include "sl_memory.h"
#include "sl_error.h"
#include "sl_tts.h"
#include "sl_audio.h"
#include "sl_app_tts.h"
/****************************************************
                                  variable

****************************************************/
static U8 tts_text[] = {"4F60597D002C6B228FCE4F7F75287B807EA67EB3005300380030005400540053529F80FD0021"};

/****************************************************
                                  function

****************************************************/


/*****************PLAY TTS***************/
void SL_AppPlayTtsCb(S32 slResult)
{
    SL_ApiPrint("SLAPP: SL_AppPlayTtsCb, result[%d]",slResult);
    //SL_TtsStop();
}

void SL_AppPlayTts(void)
{
    U8 ucTextLenth = 0;
    
    //SL_TtsSetPlaySpeed(30);
    ucTextLenth = SL_Strlen(tts_text);
    //SL_TtsPlay(tts_text, ucTextLenth, SL_AppPlayTtsCb);
}


