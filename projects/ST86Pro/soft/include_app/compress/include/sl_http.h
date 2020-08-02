/*********************************************************************************
 ** File Name:     sl_http.h                                                        *
 ** Author:         Haiqiang Feng                                                 *
 ** DATE:           2014/01/20                                                   *
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
 ** 2014/01/20        Haiqiang            Create
 *********************************************************************************/
#ifndef _SL_HTTP_H_
#define _SL_HTTP_H_
#include "sl_type.h"
/****************************************************
                                   macro define
****************************************************/
/****************************************************
                                   type define
****************************************************/
/*
    Web Client request types.
*/
typedef enum {
    SL_HTTP_Get,
    SL_HTTP_Post,
    SL_HTTP_Head
} SL_HTTP_RequestType;
typedef void (*SL_HTTP_SEND_DATA_SUCC)(void); 
typedef void (*SL_HTTP_RECV_DATA_SUCC)(char * pktPtr); //The input parameter pktPtr is the pointer of the received packet.
typedef void (*SL_HTTP_HANDLE_ERROR)(void); 
typedef struct{
    SL_HTTP_SEND_DATA_SUCC pstSlHttpSendDataSucc;
    SL_HTTP_RECV_DATA_SUCC pstSlHttpRecvDataSucc;
	SL_HTTP_HANDLE_ERROR pstSlHttpErrHandle;
}SL_HTTP_CALLBACK;
typedef enum {
	SL_HTTP_Html = 0, 
	SL_HTTP_TextPlain = 3,
	SL_HTTP_NormalForm = 8,
	SL_HTTP_MultiPartRelated = 22, 
	SL_HTTP_TextJson = 23 
}SL_HTTP_DataType;
/****************************************************
                                   function
****************************************************/
#include "sl_app.h"
#define SL_Http_GetCb AA.SL_Http_GetCb_p
#define SL_HTTP_Callback_Init AA.SL_HTTP_Callback_Init_p
/*****************************************************************************
* Name:            SL_Http_Init
* Description:    Start HTTP task, it need to be called  only one.
* Parameter:     SL_FTP_Connect_Info:   infomation of connection
* Return:           0:   OK;
*                       -1: Fail
******************************************************************************/
#define SL_Http_Init AA.SL_Http_Init_p
/*****************************************************************************
* Name:            SL_Http_Set_Para
* Description:    Set the parameters about HTTP. Now only support POST method.
* Parameter:     pUrl:  The server's URL, the format is http://server/path:tcpPort, the path and 
                                 the tcpPort is optional, the default tcpPort is 80;
                        pUserName: The user name for server's authentication. If the server don't 
                                 provide authentication, the pUserName can be set to NULL;
                        pPassword: The password for server's authentication. If the server don't 
                                 provide authentication, the pPassword can be set to NULL;         
                        pUserAgent: The user agent for the HTTP header send to server. It can be set
                                 to NULL. 
                        ReqType: The http request type. Look up the enum SL_HTTP_RequestType.
* Return:           0:   OK;
*                       -1: Fail
******************************************************************************/
#define SL_Http_Set_Para AA.SL_Http_Set_Para_p
/*****************************************************************************
* Name:            SL_Http_Send_Data
* Description:    Send the data.
* Parameter:     pData:  The data buffer's pointer. The data buffer will be copied in this function, so 
                                    the input buffer should be free by the caller.
* Return:           0:   OK;
*                       -1: Fail
******************************************************************************/
#define SL_Http_Send_Data AA.SL_Http_Send_Data_p
#endif
