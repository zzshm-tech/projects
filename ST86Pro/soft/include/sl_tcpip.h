/*********************************************************************************
 ** File Name:     sl_tcpip.h                                                *
 ** Author:         Wang Ya Feng                                                  *
 ** DATE:           2013/01/14                                                   *
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
 ** 2013/01/14     Wang Ya Feng       Create
 *********************************************************************************/
#ifndef _SL_TCPIP_H_
#define _SL_TCPIP_H_

#include "sl_type.h"
#include "sl_gprs.h"
/****************************************************
                                   macro define

****************************************************/
#define SL_MUXSOCKET_NUM     8
#define SL_PDPCID_TCPIP_INDEX 7

/****************************************************
                                   type define

****************************************************/
typedef void (*SL_TCPIP_NET_ACT)(U8 ucCidIndex, S32 slErrorCode); 
typedef void (*SL_TCPIP_NET_DEA)(U8 ucCidIndex, S32 slErrorCode);
typedef void (*SL_TCPIP_SOCK_CONN)(U8 ucCidIndex, S32 slSocketId, BOOL bResult, S32 slErrorCode);
typedef void (*SL_TCPIP_SOCK_CLOSE)(U8 ucCidIndex, S32 slSocketId, BOOL bResult, S32 slErrorCode);
typedef void (*SL_TCPIP_SOCK_ACPT)(U8 ucCidIndex, S32 slSocketId, BOOL bResult, S32 slErrorCode);
typedef void (*SL_TCPIP_SOCK_RECV)(U8 ucCidIndex, S32 slSocketId, BOOL bResult, S32 slErrorCode);
typedef void (*SL_TCPIP_SOCK_SEND)(U8 ucCidIndex, S32 slSocketId, BOOL bResult, S32 slErrorCode);
typedef void (*SL_TCPIP_GET_HOSTIP_BY_NAME)(U8 ucCidIndex, S32 slErrorCode, U8* pucIpAddr);
typedef struct{
    SL_TCPIP_NET_ACT pstSlnetAct;
    SL_TCPIP_NET_DEA pstSlnetDea;
    SL_TCPIP_SOCK_CONN pstSlsockConn;
    SL_TCPIP_SOCK_CLOSE pstSlsockClose;
    SL_TCPIP_SOCK_ACPT pstSlsockAcpt;
    SL_TCPIP_SOCK_RECV pstSlsockRecv;
    SL_TCPIP_SOCK_SEND pstSlsockSend;
    SL_TCPIP_GET_HOSTIP_BY_NAME pstSlGetHostName;
}SL_TCPIP_CALLBACK;

typedef struct{
    U8 uCid;
    U8 uStateStr[15];
    U8 uState;
    U8 nPdpType;
    U8 nDComp;
    U8 nHComp;
    U8 pApn[SL_GPRS_APN_MAX_LEN];
    U8 pPdpAddr[SL_GPRS_PDPADDR_MAX_LEN];
    U8 nApnSize;
    U8 nPdpAddrSize;
    U8 uaUsername[SL_GPRS_USR_MAX_LEN];
    U8 nUsernameSize;
    U8 uaPassword[SL_GPRS_PAS_MAX_LEN];
    U8 nPasswordSize;
} SL_GPRS_CIDINFO;

typedef enum{
    SL_TCPIP_SOCK_TCP = 1,
    SL_TCPIP_SOCK_UDP = 2,    
    SL_TCPIP_SOCK_RAW = 3   
}SL_TCPIP_SOCKET_TYPE;

typedef enum{
    SL_TCPIP_STATE_IPINITIAL,      //need all the steps
    SL_TCPIP_STATE_IPSTART,       // need begin from pdp act
    SL_TCPIP_STATE_GPRSACT,       // need begin from create socket      
    SL_TCPIP_STATE_CONNECTING,    // temporary state
    SL_TCPIP_STATE_CLOSE,         
    SL_TCPIP_STATE_CONNECTOK     
}SL_TCPIP_SOCKET_STATE;

/****************************************************
                                   function

****************************************************/
S32 SL_TcpipGprsNetInit(U8 ucCidIndex, SL_TCPIP_CALLBACK *cb);
S32 SL_TcpipGprsApnSet(U8* pucApn, U8* pucUsrId, U8* pucPassword);
S32 SL_TcpipGprsApnGet(U8* pucApn, U8* pucUsrId, U8* pucPassword);
S32 SL_TcpipGprsNetActive(void);
S32 SL_TcpipGprsNetDeactive(void);
S32 SL_TcpipGprsNetGetState(U8* pucNetState, U8* pucGprsRegState);
S32 SL_TcpipSocketCreate(S32 slSocketId, U8 ucSocketType);
S32 SL_TcpipSocketClose(S32 slSocketId);
S32 SL_TcpipSocketConnect(S32 slSocketId, U8* pucIpAddr, U16 usPort);
S32 SL_TcpipSocketSend(S32 slSocketId, U8* pucData, U16 usDatalenth);
S32 SL_TcpipSocketRecv(S32 slSocketId, U8* pucData, U16 usDatalenth);
S32 SL_TcpipSocketSendTo(S32 slSocketId, U8* pucData, U16 usDatalenth, U8* pucIpAddr, U16 usPort);
S32 SL_TcpipSocketRecvFrom(S32 slSocketId, U8* pucData, U16 usDatalenth, U8* pucIpAddr, U16 usPort);
S32 SL_TcpipSocketBind(S32 slSocketId);
S32 SL_TcpipSocketListen(S32 slSocketId);
S32 SL_TcpipSocketAccept(S32 slSocketId, U8* pucIpAddr, U16* pusPort);
S32 SL_TcpipGetState(S32 slSocketId);
S32 SL_TcpipGetHostIpbyName(U8 ucCid, U8* pucHostname, SL_TCPIP_GET_HOSTIP_BY_NAME cb);
S32 SL_TcpipGetLocalIpAddr(U8 ucCid, U8* pucIpAddr);
S32 SL_TcpipGetDnsServerAddr(U8 ucCid, U8* pucPrimaryAddr, U8* pucSecondAddr);
S32 SL_TcpipSetDnsServerAddr(U8 ucCid, BOOL bPrimarySet, U8* pucPrimayAddr, BOOL bSecondarySet, U8* pucSecondaryAddr);
S32 SL_TcpipSocketCheckIp(U8* pucIpAddrString, U32* pulIpAddr);
U32 SL_TcpipSocketHtonl(U32 n);
U16 SL_TcpipSocketHtons(U16 n);
S32 SL_TcpipSetKeepAliveTime(U32 ulTime, U32 ulEnableFlag);
U8 SL_TcpipSetRetrTimes(U8 ucRetrTimes);
U8 SL_TcpipGetRetrTimes(void);
U8 SL_TcpipSetRetrTime(U8 ucRetrTime);
U8 SL_TcpipGetRetrTime(void);
U8 SL_TcpipGetCid(void);
S32 SL_TcpipOverWifiCfg(S32 slSocketId);
S32 SL_TcpipOverWifiClear(S32 slSocketId);
S32 SL_TcpipOverWifiStat(S32 slSocketId, U8* pucStat);
S32 SL_TcpipWifiLocalIpAddrCfg(U8* pucIpAddr);
S32 SL_TcpipGetWifiLocalIpAddr(U8* pucIpAddr);

#endif
