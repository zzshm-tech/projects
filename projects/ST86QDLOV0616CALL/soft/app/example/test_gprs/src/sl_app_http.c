#include "sl_http.h"
#include "sl_gprs.h"
#include "sl_debug.h"
#include "sl_error.h"
#include "sl_tcpip.h"

void SL_APP_HttpSendDataCb(void)
{
    SL_Print("---> HTTP Send Data Done\n");
}

void SL_APP_HttpPostData(void)
{
    SL_Print("---> HTTP Send data ...\n");
    SL_Http_Set_Para("http://www.simplnano.com:1568", NULL, NULL, NULL, 0);
    //SL_Http_Set_Para("http://124.205.119.38:1568", NULL, NULL, NULL);
    SL_Http_Send_Data("SL HTTP POST TEST", 23);
}

void SL_AppHttpGprsActCb(U8 ucCid, U8 ucState, S32 slResult)
{
    SL_Print("---> SL http active call back ...\n");
    SL_APP_HttpPostData();
}

void SL_AppHttpGprsAttCb(U8 ucState, S32 slResult)
{
    SL_ApiPrint("SLAPP: SL_AppGprsAttCb ucState[%d], result[%d]", ucState, slResult);
    SL_Print("---> SL gprs attach call back\n");

    if((ucState == SL_GPRS_ATTACHED) && (slResult == 0))
    {
        SL_Print("---> SL do not active gprs and http send...\n");
    }

    if(ucState == SL_GPRS_DETACHED)
    {
        SL_Print("SLAPP: SL_AppGprsAttCb dettach, ucState[%d]", ucState);
    }

    SL_Print("---> SL App http send ...\n");
    SL_APP_HttpPostData();
}

void SL_APP_HttpRecvDataCb(char *pktPtr)
{
    SL_Print("---> SL Http recv: %s\n", pktPtr);
}

void SL_APP_HttpErrHandlerCb(void)
{
    SL_Print("---> [ERROR] SL Http Err handler\n");
}

void SL_APP_HttpInit(void)
{
    SL_HTTP_CALLBACK http_cb;
    S32 slRet = 0;

    http_cb.pstSlHttpSendDataSucc = SL_APP_HttpSendDataCb;
    http_cb.pstSlHttpRecvDataSucc = SL_APP_HttpRecvDataCb;
    http_cb.pstSlHttpErrHandle = SL_APP_HttpErrHandlerCb;
    SL_HTTP_Callback_Init(&http_cb);
    SL_Http_Init();

    slRet = SL_GprsAtt(SL_GPRS_ATTACHED, SL_AppHttpGprsAttCb);
    if(slRet != SL_RET_OK)
    {
        SL_ApiPrint("SLAPP: SL_AppGprsAttach attach fail[%d]", slRet);
    }
}

