#include "sl_type.h"
#include "sl_debug.h"
#include "sl_error.h"
#include "sl_uart.h"
#include "sl_gprs.h"
#include "sl_assistgps.h"

/****************************************************
                                  internal function

****************************************************/
void SL_AppGprsActCb(U8 ucCid, U8 ucState, S32 slResult);
void SL_AppGprsAttCb(U8 ucState, S32 slResult);
void SL_AppGprsAttach(void);

/****************************************************
                                  function

****************************************************/

void SL_AppStartAgps(void)
{
    //SL_AssistGpsConfig("supl.nokia.com", 7275);   //change default(google) agps server
    SL_AppGprsAttach();
}

void SL_AppAgpsGetAddrCb(S32 slResult, U8* pucbuf, U16 usLenth)
{
    SL_ApiPrint("SLAPP: SL_AppAgpsGetAddrCb result[%d]", slResult);
    //if(pucbuf != NULL)
    //{
    //    SL_UartSendData(SL_UART_1, pucbuf, usLenth);
    //    //SL_SmsSendTextSms("13810109423", pucbuf);
    //}
}

void SL_AppAgpsGetLocCb(S32 slResult, U32 ulLonggitude, U32 ulLatidude)
{
    S32 slRet;
    U8 auc[50] = {0};
    U8 auc1[50] = {0};
    SL_ApiPrint("SLAPP: SL_AppAgpsGetLocCb result[%d], longi[%d.%6d], lati[%d.%6d]", 
        slResult, ulLonggitude/1000000, ulLonggitude%1000000, ulLatidude/1000000, ulLatidude%1000000);
    if ((0 == ulLonggitude) || (0 == ulLatidude))
    {
        SL_Print("AGPS loc fail this time for lat/lon is 0\n");
        return;
    }

    slRet = SL_AssistGpsGetAddr(ulLonggitude, ulLatidude, SL_AppAgpsGetAddrCb);
    if (slRet != SL_RET_OK)
    {               
        SL_Print("AGPS arp error, no attached/actived gprs, exit\n");
        return;
    }
#if 0
    SL_itoa(ulLonggitude, auc, 10);
    SL_itoa(ulLatidude, auc1, 10);
    SL_Strcat(auc,",");
    SL_Strcat(auc,auc1);
    SL_SmsSendTextSms("13810109423", auc);
#endif
}

void SL_AppGetHostIpByNameCb(U8 ucCidIndex, S32 slErrorCode, U8* pucIpAddr)
{
    SL_ApiPrint("SLAPP: SL_AppGetHostIpByNameCb cid[%d], result[%d]", ucCidIndex, slErrorCode);
    SL_ApiPrint("SLAPP: IP[%s]", pucIpAddr);
}

void SL_AppGprsActCb(U8 ucCid, U8 ucState, S32 slResult)
{
    U8 aucLocalIp[20] = {0};
    U8 aucPriDns[20] = {0};
    U8 aucSecDns[20] = {0};
    S32 slRet = 0;

    SL_ApiPrint("SLAPP: SL_AppGprsActCb cid[%d], state[%d], result[%d]", ucCid, ucState, slResult);

    if(ucState == SL_GPRS_ACTIVED)
    {
#if 1 //get agps location
        slRet = SL_AssistGpsGetLoc(SL_AppAgpsGetLocCb);
        if (slRet != SL_RET_OK)
        {
            SL_Print("AGPS loc error, no attached/actived gprs, exit\n");
            return;
        }

#else  //get ip address
        slRet = SL_TcpipGetLocalIpAddr(1, aucLocalIp);
        SL_ApiPrint("SLAPP: SL_AppGprsActCb get local ip addr[%s],ret[%d]", aucLocalIp, slRet);
        slRet = SL_TcpipGetDnsServerAddr(1, aucPriDns, aucSecDns);
        SL_ApiPrint("SLAPP: SL_AppGprsActCb get prim DNS addr[%s],sec DNS addr[%s], ret[%d]", aucPriDns, aucSecDns, slRet);
        SL_TcpipGetHostIpbyName(1, "www.sohu.com", SL_AppGetHostIpByNameCb);

#endif
    }
    else if(ucState == SL_GPRS_DEACTIVED)
    {
        SL_ApiPrint("SLAPP: SL_AppGprsActCb cid[%d], deactive result[%d]", ucCid, slResult);
    }
    else
    {
        SL_ApiPrint("SLAPP: SL_AppGprsActCb cid[%d], state[%d] err", ucCid, ucState);
    }
}

void SL_AppGprsAttCb(U8 ucState, S32 slResult)
{
    SL_ApiPrint("SLAPP: SL_AppGprsAttCb ucState[%d], result[%d]", ucState, slResult);

    if((ucState == SL_GPRS_ATTACHED) && (slResult == 0))
    {
        SL_GprsSetPdp(1, SL_GPRS_PDP_TYPE_IP, "CMNET");

        SL_GprsAct(1, SL_GPRS_ACTIVED, SL_AppGprsActCb);
    }
    if(ucState == SL_GPRS_DETACHED)
    {
        SL_ApiPrint("SLAPP: SL_AppGprsAttCb dettach, ucState[%d]", ucState);
    }
}

void SL_AppGprsAttach(void)
{
    S32 slRet = 0;
    U8 attach_st = 0;
    U8 active_st = 0;

    slRet = SL_GprsGetAttState(&attach_st);
    if (slRet != SL_RET_OK)
    {
        SL_Print("AGPS get gprs attach stat error, exit!\n");
        return;
    }

    if (SL_GPRS_ATTACHED == attach_st)
    {
        SL_Print("AGPS get gprs attach state attached, check active gprs state first\n");
        slRet = SL_GprsGetActState(1, &active_st);
        if (slRet != SL_RET_OK)
        {
            SL_Print("AGPS get gprs active stat error, exit!\n");
            return;
        }

        if (SL_GPRS_ACTIVED == active_st)
        {
            SL_Print("AGPS gprs already actived, start agps location...\n");
            slRet = SL_AssistGpsGetLoc(SL_AppAgpsGetLocCb);
            if (slRet != SL_RET_OK)
            {
                SL_Print("AGPS get location error for no attached/actived gprs, exit\n");
                return;
            }
        } 
        else
        {
            SL_Print("AGPS get gprs active stat deactived, active gprs...\n");
            SL_GprsSetPdp(1, SL_GPRS_PDP_TYPE_IP, "CMNET");
            SL_GprsAct(1, SL_GPRS_ACTIVED, SL_AppGprsActCb);
        }
    }
    else
    {
        SL_Print("AGPS get gprs attach state deattached, attach gprs...\n");
        slRet = SL_GprsAtt(SL_GPRS_ATTACHED, SL_AppGprsAttCb);
        if(slRet != SL_RET_OK)
        {
            SL_ApiPrint("SLAPP: SL_AppGprsAttach attach fail[%d]", slRet);
            return;
        }
    }
}

