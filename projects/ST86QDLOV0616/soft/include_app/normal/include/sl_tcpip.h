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
#include "sl_app.h"
#define SL_TcpipGprsNetInit AA.SL_TcpipGprsNetInit_p
#define SL_TcpipGprsApnSet AA.SL_TcpipGprsApnSet_p
#define SL_TcpipGprsApnGet AA.SL_TcpipGprsApnGet_p
#define SL_TcpipGprsNetActive AA.SL_TcpipGprsNetActive_p
#define SL_TcpipGprsNetDeactive AA.SL_TcpipGprsNetDeactive_p
#define SL_TcpipGprsNetGetState AA.SL_TcpipGprsNetGetState_p
#define SL_TcpipSocketCreate AA.SL_TcpipSocketCreate_p
#define SL_TcpipSocketClose AA.SL_TcpipSocketClose_p
#define SL_TcpipSocketConnect AA.SL_TcpipSocketConnect_p
#define SL_TcpipSocketSend AA.SL_TcpipSocketSend_p
#define SL_TcpipSocketRecv AA.SL_TcpipSocketRecv_p
#define SL_TcpipSocketSendTo AA.SL_TcpipSocketSendTo_p
#define SL_TcpipSocketRecvFrom AA.SL_TcpipSocketRecvFrom_p
#define SL_TcpipSocketBind AA.SL_TcpipSocketBind_p
#define SL_tcpip_socket_bind AA.SL_tcpip_socket_bind_p
#define SL_TcpipSocketListen AA.SL_TcpipSocketListen_p
#define SL_TcpipSocketAccept AA.SL_TcpipSocketAccept_p
#define SL_TcpipGetState AA.SL_TcpipGetState_p
#define SL_TcpipGetHostIpbyName AA.SL_TcpipGetHostIpbyName_p
#define SL_TcpipGetLocalIpAddr AA.SL_TcpipGetLocalIpAddr_p
#define SL_TcpipGetDnsServerAddr AA.SL_TcpipGetDnsServerAddr_p
#define SL_TcpipSetDnsServerAddr AA.SL_TcpipSetDnsServerAddr_p
#define SL_TcpipSocketCheckIp AA.SL_TcpipSocketCheckIp_p
#define SL_TcpipSocketHtonl AA.SL_TcpipSocketHtonl_p
#define SL_TcpipSocketHtons AA.SL_TcpipSocketHtons_p
#define SL_TcpipSetKeepAliveTime AA.SL_TcpipSetKeepAliveTime_p
#define SL_TcpipSetRetrTimes AA.SL_TcpipSetRetrTimes_p
#define SL_TcpipGetRetrTimes AA.SL_TcpipGetRetrTimes_p
#define SL_TcpipSetRetrTime AA.SL_TcpipSetRetrTime_p
#define SL_TcpipGetRetrTime AA.SL_TcpipGetRetrTime_p
#define SL_TcpipGetCid AA.SL_TcpipGetCid_p
#define SL_TcpipOverWifiCfg AA.SL_TcpipOverWifiCfg_p
#define SL_TcpipOverWifiClear AA.SL_TcpipOverWifiClear_p
#define SL_TcpipOverWifiStat AA.SL_TcpipOverWifiStat_p
#define SL_TcpipWifiLocalIpAddrCfg AA.SL_TcpipWifiLocalIpAddrCfg_p
#define SL_TcpipGetWifiLocalIpAddr AA.SL_TcpipGetWifiLocalIpAddr_p
#endif
