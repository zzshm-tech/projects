#include "sl_os.h"
#include "sl_stdlib.h"
#include "sl_type.h"
#include "sl_debug.h"
#include "sl_error.h"
#include "sl_timer.h"
#include "sl_call.h"
#include "sl_app_call.h"

/****************************************************
                                  function

****************************************************/
    
void SL_AppCallDialCb(S32 slResult)
{
    SL_CALL_ACTIVE_STATE enSlCallState = 0;

    SL_ApiPrint("SLAPP: call dial rsp result[%d]", slResult);
    
    enSlCallState = SL_CallGetActiveStatus();
    SL_ApiPrint("SLAPP: SL_AppCallDialCb state[%d]", enSlCallState);
}

void SL_AppCallHangupCb(S32 slResult)
{
    SL_CALL_ACTIVE_STATE enSlCallState = 0;

    SL_ApiPrint("SLAPP: call hangup rsp result[0x%x]", slResult);
    
    enSlCallState = SL_CallGetActiveStatus();
    SL_ApiPrint("SLAPP: SL_AppCallHangupCb state[%d]", enSlCallState);
}

void SL_AppCallRingCb(U8* pucComingNum)
{
    SL_TASK stSltask;
    SL_CALL_ACTIVE_STATE enSlCallState = 0;
    
    SL_ApiPrint("SLAPP: call ring ind callno[%s]", pucComingNum);
    #if 1
    stSltask.element[0] = SL_GetAppTaskHandle();
    SL_StartTimer(stSltask, APP_TIMER_CALL_RING, SL_TIMER_MODE_SINGLE, SL_SecondToTicks(10));
    enSlCallState = SL_CallGetActiveStatus();
    SL_ApiPrint("SLAPP: SL_AppCallRingCb state[%d]", enSlCallState);
    #endif
    //SL_AppAnswer();
}

void SL_AppAnswer(void)
{
    SL_TASK stSltask;
    //if(!SL_Strcmp(pucComingNum, pucCallNo))
    {
        SL_CallAnswer();
        stSltask.element[0] = SL_GetAppTaskHandle();
        SL_StartTimer(stSltask, APP_TIMER_CALL_HANGUP, SL_TIMER_MODE_SINGLE, SL_SecondToTicks(10));
    }
}
void SL_AppHangUp()
{
    SL_CallHangup();
}
void SL_AppCallRleaseCb(S32 slResult)
{
    SL_CALL_ACTIVE_STATE enSlCallState = 0;

    SL_ApiPrint("SLAPP: call release rsp result[%d]", slResult);
    
    enSlCallState = SL_CallGetActiveStatus();
    SL_ApiPrint("SLAPP: SL_AppCallRleaseCb state[%d]", enSlCallState);
}

void SL_AppCallAcceptCb(S32 slResult)
{
    SL_CALL_ACTIVE_STATE enSlCallState = 0;

    SL_ApiPrint("SLAPP: call accept rsp result[%d]", slResult);

    enSlCallState = SL_CallGetActiveStatus();
    SL_ApiPrint("SLAPP: SL_AppCallAcceptCb state[%d]", enSlCallState);

}

void SL_AppInitCall()
{
    SL_CALL_CALLBACK stCallCb;

    SL_Memset(&stCallCb, 0, sizeof(stCallCb));
    stCallCb.pstSlCallDial = SL_AppCallDialCb;
    stCallCb.pstSlCallHangup = SL_AppCallHangupCb;
    stCallCb.pstSlCallRing = SL_AppCallRingCb;
    stCallCb.pstSlCallRelease = SL_AppCallRleaseCb;
    stCallCb.pstSlCallAccept = SL_AppCallAcceptCb;
    SL_CallInit(&stCallCb);
    SL_ApiPrint("SLAPP: SL_AppInitCall OK!");
}
void SL_AppDialCall(char* CallNo)
{
    SL_CallDial(SL_CALL_VOICECALL, CallNo);
    SL_ApiPrint("SLAPP: SL_AppStartCall OK!");
}

