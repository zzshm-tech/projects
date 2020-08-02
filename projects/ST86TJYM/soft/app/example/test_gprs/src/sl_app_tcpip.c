#include "sl_type.h"
#include "sl_debug.h"
#include "sl_error.h"
#include "sl_stdlib.h"
#include "sl_system.h"
#include "sl_uart.h"
#include "sl_tcpip.h"
#include "sl_os.h"
#include "sl_timer.h"
#include "sl_app_tcpip.h"

/****************************************************
                                  variable

****************************************************/
static unsigned char test_data[] = {"GPRSLOGN0100000000000086566200000000"};
static U8 aucApn[] = {"CMNET"};
static U8 aucUsrId[] = {"S80"};
static U8 aucPassword[] = {"S80"};
static U8 aucServerIp[] = {"124.205.119.38"};
static U16 usServerPort = 1560; //8000;
static U8 gucSocketNum = 0;

/****************************************************
                                 extern  variable

****************************************************/

/****************************************************
                                  function

****************************************************/
void SL_AppTcpipConn(S32 slSockId, U8 ucSocketType, U8* pucIpAddr, U16 usPort)
{
    S32 slRet = 0;

    slRet = SL_TcpipSocketCreate(slSockId, ucSocketType);
    if(slRet < 0)
    {
        SL_ApiPrint("SLAPP: socket create SockId[%d] fail[%d]", slSockId, slRet);
        return ;
    }
    gucSocketNum++;
    SL_ApiPrint("SL_TcpipSocketCreate OK, sock id[%d]", slRet);
    slRet = SL_TcpipSocketBind(slSockId);
    if(slRet != SL_RET_OK)
    {
        SL_ApiPrint("SLAPP: socket bind fail[%d]", slRet);
        return ;
    }
    SL_ApiPrint("******* SL_TcpipSocketBind OK*********\n");
    SL_Sleep(1000);
    if(ucSocketType == SL_TCPIP_SOCK_TCP)
    {
        slRet = SL_TcpipSocketConnect(slSockId, pucIpAddr, usPort);
        SL_ApiPrint("SLAPP: SL_TcpipSocketConnect ret[%d]", slRet);
    }
    if(ucSocketType == SL_TCPIP_SOCK_UDP)
    {
        //for(U8 i = 0; i < 10; i++)
        {
            slRet = SL_TcpipSocketSendTo(slSockId, test_data, sizeof(test_data), pucIpAddr, usPort);
            if(slRet <= 0)
            {
                SL_ApiPrint("SLAPP: socket send fail ret=%d", slRet);
            }
            SL_ApiPrint("SLAPP: socket send lenth=%d", slRet);
            SL_Sleep(3000);
        }
    }
}

void SL_AppGprsNetActRsp(U8 ucCidIndex, S32 slErrorCode)
{
    S32 slRet = 0;

    SL_ApiPrint("******* SL_TcpipGprsNetActive %d*********\n", slRet);
    if (slErrorCode == SL_RET_OK)
    {
        SL_AppTcpipConn(0, SL_TCPIP_SOCK_TCP, aucServerIp, usServerPort);
    }
}

void SL_AppTcpipConnRsp(U8 ucCidIndex, S32 slSockId, BOOL bResult, S32 slErrorCode)
{
    S32 slRet = 0;
    
    SL_ApiPrint("SLAPP: SL_AppTcpipConnRsp");
    if(bResult == FALSE)
    {
        SL_ApiPrint("SLAPP: socket connect fail[%d]", slErrorCode);
        return;
    }
    #if 0
    slRet = SL_TcpipSocketSend(slSockId, test_data, sizeof(test_data));
    if(slRet <= 0)
    {
        SL_ApiPrint("SLAPP: socket send fail ret=%d", slRet);
    }
    SL_ApiPrint("SLAPP: socket send lenth=%d", slRet);
    #else
    SL_AppTcpipConn(1, SL_TCPIP_SOCK_UDP, aucServerIp, usServerPort);

    #endif
}

void SL_AppTcpipSendRsp(U8 ucCidIndex, S32 slSockId, BOOL bResult, S32 slErrorCode)
{
    SL_TASK stSltask;

    SL_ApiPrint("SLAPP: SL_AppTcpipSendRsp socket[%d]", slSockId);
    if(bResult == FALSE)
    {
        SL_ApiPrint("SLAPP: socket connect fail[%d]", slErrorCode);
        return;
    }
    SL_ApiPrint("SLAPP: socket send OK");
    stSltask.element[0] = SL_GetAppTaskHandle();
    SL_StartTimer(stSltask, APP_TIMER_TCPIP_RESEND, SL_TIMER_MODE_PERIODIC, SL_SecondToTicks(300));
}

void SL_AppTcpipRcvRsp(U8 ucCidIndex, S32 slSockId, BOOL bResult, S32 slErrorCode)
{
    S32 slRet = 0;
    U8 aucData[1000] = {0};
    
    SL_ApiPrint("SLAPP: SL_AppTcpipRcvRsp");
    if(bResult == FALSE)
    {
        SL_ApiPrint("SLAPP: socket receive fail[%d]", slErrorCode);
        return;
    }

    slRet = SL_TcpipSocketRecv(slSockId, aucData, sizeof(aucData));
    if(slRet >= 0)
    {
        SL_ApiPrint("SLAPP: socket[%d] receive OK, lenth[%d], data[%s]", slSockId, slRet, aucData);
        SL_MEMBLOCK(aucData, slRet, 16);
        //SL_UartSendData(SL_UART_1, aucData, sizeof(aucData));        
    }
    else
    {
        SL_ApiPrint("SLAPP: socket receive fail, ret[%d]", slRet);
    }

}

void SL_AppTcpipCloseRsp(U8 ucCidIndex, S32 slSockId, BOOL bResult, S32 slErrorCode)
{
    S32 slRet = 0;
    S32 slState = 0;
    
    SL_ApiPrint("SLAPP: SL_AppTcpipCloseRsp, err[%d]", slErrorCode);
    if(bResult == FALSE)
    {
        SL_ApiPrint("SLAPP: socket Close fail[%d]", slErrorCode);
        return;
    }
    SL_ApiPrint("SLAPP: socket[%d] Close OK", slSockId);
    
    slState = SL_TcpipGetState(slSockId);
    SL_ApiPrint("SLAPP: SL_AppTcpipCloseRsp socket state[%d]", slState);
    
    gucSocketNum--;
    if((!gucSocketNum) || (slErrorCode == SL_TCPIP_SOCKET_ABORT))
    {
        slRet = SL_TcpipGprsNetDeactive();
    }
    if(slRet != SL_RET_OK)
    {
        SL_ApiPrint("SLAPP: gprs net deact fail[0x%x]", slRet);
    }
}

void SL_AppGprsNetDeactRsp(U8 ucCidIndex, S32 slErrorCode)
{
    if(slErrorCode != SL_RET_OK)
    {
        SL_ApiPrint("SLAPP: gprs net deact rsp fail[%d]", slErrorCode);
        return;
    }
    SL_ApiPrint("SLAPP: gprs net deact rsp OK");
    //SL_AppStartTcpip();
}

void SL_AppInitTcpip()
{
    SL_TCPIP_CALLBACK stSlTcpipCb;

    SL_Memset(&stSlTcpipCb, 0, sizeof(stSlTcpipCb));
    stSlTcpipCb.pstSlnetAct = SL_AppGprsNetActRsp;
    stSlTcpipCb.pstSlnetDea = SL_AppGprsNetDeactRsp;
    stSlTcpipCb.pstSlsockConn = SL_AppTcpipConnRsp;
    stSlTcpipCb.pstSlsockSend = SL_AppTcpipSendRsp;
    stSlTcpipCb.pstSlsockRecv = SL_AppTcpipRcvRsp;
    stSlTcpipCb.pstSlsockClose = SL_AppTcpipCloseRsp;
    SL_TcpipSetRetrTime(0x10);
    SL_TcpipGprsNetInit(0, &stSlTcpipCb);
    SL_ApiPrint("SLAPP: SL_AppInitTcpip OK!");
}

S32 SL_AppStartTcpip()
{
    S32 slRet = 0;
    
    SL_TcpipGprsApnSet(aucApn, aucUsrId, aucPassword);

    SL_ApiPrint("******* SL_TcpipGprsNetActive *********\n");
    slRet = SL_TcpipGprsNetActive();
    if(slRet != SL_RET_OK)
    {
        SL_ApiPrint("SLAPP: gprs net act fail[%d]", slRet);
    }
    return slRet;
}

void SL_AppTcpipRsnd(S32 slSockId)
{
    S32 slState = 0;
    S32 slRet = 0;

    slState = SL_TcpipGetState(slSockId);
    SL_ApiPrint("SLAPP: SL_AppTcpipRsnd socket state[%d]", slState);
    if(slState == SL_TCPIP_STATE_CONNECTOK)
    {
        slRet = SL_TcpipSocketSend(slSockId, test_data, sizeof(test_data));
        if(slRet <= 0)
        {
            SL_ApiPrint("SLAPP: SL_AppTcpipRsnd socket send fail ret=%d", slRet);
        }
        SL_ApiPrint("SLAPP: SL_AppTcpipRsnd socket send lenth=%d", slRet);
    }
}

