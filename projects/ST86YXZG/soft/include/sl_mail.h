/*********************************************************************************
 ** File Name:     sl_http.h                                                        *
 ** Author:         Haiqiang Feng                                                 *
 ** DATE:           2014/07/11                                                   *
 ** Copyright:      2014 by SimpLight Nanoelectronics,Ltd. All Rights Reserved.  *
 *********************************************************************************
 
 *********************************************************************************
 ** Description:    Implement for common function of API layer                   *
 ** Note:           None                                                         *
 *********************************************************************************

 *********************************************************************************
 **                        Edit History                                          *
 ** -------------------------------------------------------------------------    *
 ** DATE                   NAME             DESCRIPTION                                  *
 ** 2014/07/11        Haiqiang            Create
 *********************************************************************************/
#ifndef _SL_MAIL_H_
#define _SL_MAIL_H_

#include "sl_type.h"

/****************************************************
                                   macro define

****************************************************/
#define SL_MAIL_STRING_MAX_LENGTH	128

/****************************************************
                                   type define

****************************************************/

typedef void (*SL_POP3_CONNECT_SUCC)(void); 
typedef void (*SL_POP3_STATUS_SUCC)(U8 *pu8Data, U32 u32DataLen); 
typedef void (*SL_POP3_RECV_SUCC)(void); 
typedef void (*SL_POP3_DELE_SUCC)(void); 
typedef void (*SL_POP3_RSET_SUCC)(void); 
typedef void (*SL_POP3_LIST_SUCC)(U8 *pu8Data, U32 u32DataLen); 
typedef void (*SL_POP3_UIDL_SUCC)(U8 *pu8Data, U32 u32DataLen); 
typedef void (*SL_POP3_QUIT_SUCC)(void); 
typedef void (*SL_POP3_HANDLE_ERROR)(void); 

typedef void (*SL_SMTP_CONNECT_SUCC)(void); 
typedef void (*SL_SMTP_SEND_SUCC)(void); 
typedef void (*SL_SMTP_QUIT_SUCC)(void); 
typedef void (*SL_SMTP_HANDLE_ERROR)(void); 

typedef struct{
    SL_POP3_CONNECT_SUCC    pstSlPop3ConnetSucc;
    SL_POP3_STATUS_SUCC     pstSlPop3StatusSucc;
    SL_POP3_RECV_SUCC       pstSlPop3RecvSucc;
    SL_POP3_DELE_SUCC       pstSlPop3DeleteSucc;
    SL_POP3_RSET_SUCC       pstSlPop3RsetSucc;
    SL_POP3_LIST_SUCC       pstSlPop3ListSucc;
    SL_POP3_UIDL_SUCC       pstSlPop3UidlSucc;
    SL_POP3_QUIT_SUCC       pstSlPop3QuitSucc;
    SL_POP3_HANDLE_ERROR    pstSlPop3ErrHandle;
}SL_POP3_CALLBACK;

typedef struct{
    SL_SMTP_CONNECT_SUCC    pstSlSmtpConnetSucc;
    SL_SMTP_SEND_SUCC       pstSlSmtpSendSucc;
    SL_SMTP_QUIT_SUCC       pstSlSmtpQuitSucc;
    SL_SMTP_HANDLE_ERROR    pstSlSmtpErrHandle;
}SL_SMTP_CALLBACK;


typedef struct _sl_mail_list
{
    char data[SL_MAIL_STRING_MAX_LENGTH];
    struct _sl_mail_list *next;
} SL_MAIL_List_t;


/****************************************************
                                   function

****************************************************/

SL_POP3_CALLBACK* SL_POP3_GetCb(void);

/*****************************************************************************
* Name:            SL_POP3_Callback_Init
* Description:    Init callback function.
* Parameter:     cb:  callback function structure pointer;
* Return:           void
******************************************************************************/
void SL_POP3_Callback_Init(SL_POP3_CALLBACK *cb);


/*****************************************************************************
* Name:            SL_POP3_Set_Parameter
* Description:    Set the relevant parameters .
* Parameter:     host: The POP3 server's URL or IP address;
*                      port: The POP3 server's port
*                      username: The POP3 client's username for login the server
*                      password: The POP3 client's password for login the server
*                      tlsFlag: 0: Disable TLS, 1: Enable TLS
*                      timeout: The timeout between two commands, if timeout, the connectiong will
*                                   be closed.
* Return:           0:   OK;
*                       -1: Fail
******************************************************************************/
S32 SL_POP3_Set_Parameter(char *host, UINT32 port, char *username, char *password, UINT32 tlsFlag, UINT32 timeout);


/*****************************************************************************
* Name:            SL_POP3_Connect
* Description:    Connect to the POP3 server.
* Parameter:     void;
* Return:           0:   OK;
*                       -1: Fail
******************************************************************************/
S32 SL_POP3_Connect(void);

/*****************************************************************************
* Name:            SL_POP3_STATUS
* Description:    Get the status of server, including how many mail and how many bytes in the 
*                       server.
* Parameter:     void;
* Return:           0:   OK;
*                       -1: Fail
******************************************************************************/
S32 SL_POP3_STATUS(void);


/*****************************************************************************
* Name:            SL_POP3_RETR
* Description:    Receive the pointed mail.
* Parameter:     u32MailNum: The pointed mail number;
* Return:           0:   OK;
*                       -1: Fail
******************************************************************************/
S32 SL_POP3_RETR(UINT32 u32MailNum);

/*****************************************************************************
* Name:            SL_POP3_DELE
* Description:    Mark the pointed mail as deleted. After call the SL_POP3_QUIT(), the pointed
*                      mail will be deleted really.
* Parameter:     u32MailNum: The pointed mail number;
* Return:           0:   OK;
*                       -1: Fail
******************************************************************************/
S32 SL_POP3_DELE(UINT32 u32MailNum);


/*****************************************************************************
* Name:            SL_POP3_LIST
* Description:    Get the bytes of the pointed mail.
* Parameter:     u32MailNum: The pointed mail number, if the number is 0xFFFFFFFF, will get
*                      the information of all the mails in the server.
* Return:           0:   OK;
*                       -1: Fail
******************************************************************************/
S32 SL_POP3_LIST(UINT32 u32MailNum);


/*****************************************************************************
* Name:            SL_POP3_RSET
* Description:    Reset the marked delete mail.
* Parameter:     void
* Return:           0:   OK;
*                       -1: Fail
******************************************************************************/
S32 SL_POP3_RSET(void);


/*****************************************************************************
* Name:            SL_POP3_UIDL
* Description:    Get the unique-id of the pointed mail.
* Parameter:     u32MailNum: The pointed mail number, if the number is 0xFFFFFFFF, will get
*                      the information of all the mails in the server.
* Return:           0:   OK;
*                       -1: Fail
******************************************************************************/
S32 SL_POP3_UIDL(UINT32 u32MailNum);


/*****************************************************************************
* Name:            SL_POP3_QUIT
* Description:    Close the connection to the POP3 Server.
* Parameter:     void
* Return:           0:   OK;
*                       -1: Fail
******************************************************************************/
S32 SL_POP3_QUIT(void);


/*****************************************************************************
* Name:            SL_POP3_GetSender
* Description:    Get the sender after receive one mail by calling SL_POP3_RETR().
* Parameter:     pBuff: The input buffer for store the sender string.
*                      buffLen: the length of pBuff.
* Return:           0:   OK;
*                       -1: Fail
******************************************************************************/
S32 SL_POP3_GetSender(UINT8 *pBuff, UINT32 buffLen );


/*****************************************************************************
* Name:            SL_POP3_GetSubject
* Description:    Get the subject after receive one mail by calling SL_POP3_RETR().
* Parameter:     pBuff: The input buffer for store the subject string.
*                      buffLen: the length of pBuff.
* Return:           0:   OK;
*                       -1: Fail
******************************************************************************/
S32 SL_POP3_GetSubject(UINT8 *pBuff, UINT32 buffLen);


/*****************************************************************************
* Name:            SL_POP3_GetContent
* Description:    Get the mail content after receive one mail by calling SL_POP3_RETR().
* Parameter:     pBuff: The input buffer for store the mail content.
*                      buffLen: the length of pBuff.
* Return:           0:   OK;
*                       -1: Fail
******************************************************************************/
S32 SL_POP3_GetContent(UINT8 *pBuff, UINT32 buffLen);


/*****************************************************************************
* Name:            SL_POP3_GetAttach
* Description:    Get the attachment after receive one mail by calling SL_POP3_RETR().
* Parameter:     pBuff: The input buffer for store the attachment.
*                      buffLen: the length of pBuff.
* Return:           0:   OK;
*                       -1: Fail
******************************************************************************/
S32 SL_POP3_GetAttach(UINT8 *pBuff, UINT32 buffLen);


SL_SMTP_CALLBACK* SL_SMTP_GetCb(void);


/*****************************************************************************
* Name:            SL_SMTP_Callback_Init
* Description:    Init callback function.
* Parameter:     cb:  callback function structure pointer;
* Return:           void
******************************************************************************/
void SL_SMTP_Callback_Init(SL_SMTP_CALLBACK *cb);


/*****************************************************************************
* Name:            SL_SMTP_Set_Parameter
* Description:    Set the relevant parameters .
* Parameter:     host: The SMTP server's URL or IP address;
*                      port: The SMTP server's port
*                      username: The SMTP client's username for login the server
*                      password: The SMTP client's password for login the server
*                      tlsFlag: 0: Disable TLS, 1: Enable TLS
* Return:           0:   OK;
*                       -1: Fail
******************************************************************************/
S32 SL_SMTP_Set_Parameter(char *host, UINT32 port, char *username, char *password, UINT32 tlsFlag);


/*****************************************************************************
* Name:            SL_SMTP_Connect
* Description:    Connect to SMTP server.
* Parameter:     void
* Return:           0:   OK;
*                       -1: Fail
******************************************************************************/
S32 SL_SMTP_Connect(void);


/*****************************************************************************
* Name:            SL_SMTP_SendMail
* Description:    Send the mail to SMTP server. Not support send attachment now.
* Parameter:     fromAddr: The sender's email address.
*                      toAddrList: The reveiver's email address list.
*                      subject: The mail's subject.
*                      mailContent: The mail's content for sending.
* Return:           0:   OK;
*                       -1: Fail
******************************************************************************/
S32 SL_SMTP_SendMail(char *fromAddr, SL_MAIL_List_t *toAddrList, char *subject, char *mailContent);


/*****************************************************************************
* Name:            SL_SMTP_Quit
* Description:    Close the connection to SMTP server.
* Parameter:     void
* Return:           0:   OK;
*                       -1: Fail
******************************************************************************/
S32 SL_SMTP_Quit(void);


#endif

