#include "sl_type.h"
#include "sl_debug.h"
#include "sl_error.h"
#include "sl_stdlib.h"
#include "sl_system.h"
#include "sl_uart.h"
#include "sl_os.h"
#include "sl_app_event.h"

/****************************************************
                                  variable

****************************************************/
U8 gaucPin[] = {"5678"};
U8 gaucPuk[] = {"15968944"};

/****************************************************
                                  function

****************************************************/
void SL_AppSimEntPinPwdCb(S32 slResult)
{
    SL_ApiPrint("SLAPP: SL_AppSimEntPinPwdCb result[%d]", slResult);
    if(slResult == SL_RET_OK)
    {
        SL_AppSendMsg(SL_GetAppTaskHandle(), EVT_APP_READY, 0);
    }
    else
    {
        //SL_SimGetAuthInfo();
        SL_ApiPrint("SLAPP: SL_AppSimEntPinPwdCb pin code err.");
    }
}

void SL_AppSimSetPinPwdCb(S32 slResult)
{
    SL_ApiPrint("SLAPP: SL_AppSimSetPinPwdCb result[%d]", slResult);
    if(slResult == SL_RET_OK)
    {
        SL_AppSendMsg(SL_GetAppTaskHandle(), EVT_APP_READY, 0);
    }
}

void SL_AppSimChgPinPwdCb(S32 slResult)
{
    SL_ApiPrint("SLAPP: SL_AppSimChgPinPwdCb result[%d]", slResult);
}

void SL_AppSimClckCb(S32 slResult)
{
    SL_ApiPrint("SLAPP: SL_AppSimClckCb result[%d]", slResult);
}

void SL_AppSimCpinQueryCb(S32 slResult, U8 ucPinStatus)
{
    SL_ApiPrint("SLAPP: SL_AppSimCpinQueryCb result[%d], pin status[%d]", slResult, ucPinStatus);
    if(ucPinStatus == SL_SIM_AUTH_READY)
    {
        SL_AppSendMsg(SL_GetAppTaskHandle(), EVT_APP_READY, 0);
    }
    else if(ucPinStatus == SL_SIM_AUTH_SIMPIN)
    {
        SL_ApiPrint("SLAPP: SL_AppSimCpinQueryCb need PIN");
        SL_SimEnterPwd(gaucPin);
    }
    else if(ucPinStatus == SL_SIM_AUTH_SIMPUK)
    {
        SL_ApiPrint("SLAPP: SL_AppSimCpinQueryCb need PUK");
        SL_SimSetPwd(gaucPuk, gaucPin);
    }
}

void SL_AppSimClckQueryCb(S32 slResult, U8 ucLckStatus)
{
    SL_ApiPrint("SLAPP: SL_AppSimCpwdCb result[%d], lck status[%d]", slResult, ucLckStatus);
}

void SL_AppSimPinInit(void)
{
    SL_SIM_CALLBACK stSlSimCb;

    SL_Memset(&stSlSimCb, 0, sizeof(stSlSimCb));
    stSlSimCb.slSimEnterPinPwdCb = SL_AppSimEntPinPwdCb;
    stSlSimCb.slSimChangePinPwdCb = SL_AppSimChgPinPwdCb;
    stSlSimCb.slSimSetPinPwdCb = SL_AppSimSetPinPwdCb;
    stSlSimCb.slSimClckCb = SL_AppSimClckCb;
    stSlSimCb.slSimCpinQueryCb = SL_AppSimCpinQueryCb;
    stSlSimCb.slSimClckQueryCb = SL_AppSimClckQueryCb;

    SL_SimPinInit(&stSlSimCb);
    SL_ApiPrint("SLAPP: SL_AppSimPinInit OK!");
}

void SL_AppSimPinQuery(void)
{
    SL_ApiPrint("SLAPP: SL_AppSimPinQuery");
    SL_SimGetAuthInfo();
}

void SL_AppSimLock(U8 ucLock)
{
    SL_SimLock(0, ucLock, gaucPin);
}

