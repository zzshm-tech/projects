/*********************************************************************************
 ** File Name:     sl_ftp.h                                                *
 ** Author:         Xu Hao                                                 *
 ** DATE:           2013/06/25                                                   *
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
 ** 2013/06/25       Xu Hao            Create
 *********************************************************************************/
#ifndef _SL_FTP_H_
#define _SL_FTP_H_
#include "sl_type.h"
/****************************************************
                                   macro define
****************************************************/
#define		SL_FTP_STRING_MAX_LENGTH			128 
/****************************************************
                                   type define
****************************************************/
typedef enum {
    SL_FTP_FILE_TYPE_ASCII = 0,
    SL_FTP_FILE_TYPE_BINARY = 1
}SL_FTP_File_TYPE;
typedef struct _stSL_FTP_Open_Paras
{
    U16 u16DestPort;
    U8  uaUrl[SL_FTP_STRING_MAX_LENGTH];
    U8  uaUsername[SL_FTP_STRING_MAX_LENGTH];
    U8  uaPassword[SL_FTP_STRING_MAX_LENGTH];
}SL_FTP_Connect_Info;
typedef struct
{
    UINT32    min;      /* [0,59]     */
    UINT32    hour;     /* [0,23]     */
    UINT32    day;      /* [1,31]     */
    UINT32    month;    /* [1,12]     */
}SL_FTP_TIME_DATE;
typedef struct _stAT_Ftp_List_Info
{
    U8  u8Valid;  /*1: exist; 0: not exist*/
    U8  u8FileName[SL_FTP_STRING_MAX_LENGTH];
    S32 s32Chmod;
    U32 u32Length;
    SL_FTP_TIME_DATE strData;
}SL_Ftp_List_Info;
typedef void (*SL_FTP_LOGIN)(S32 s32ErrorCode); 
typedef void (*SL_FTP_DOWNLOAD)(U8 *pu8Data, U32 u32DataLen, S32 s32ErrorCode); 
typedef void (*SL_FTP_UPLOAD)(S32 slErrorCode); 
typedef void (*SL_FTP_PWD)(U8 *pu8RemoteDir, S32 slErrorCode); 
typedef void (*SL_FTP_CWD)(S32 slErrorCode); 
typedef void (*SL_FTP_CDUP)(S32 slErrorCode); 
typedef void (*SL_FTP_MKD)(S32 slErrorCode); 
typedef void (*SL_FTP_RMD)(S32 slErrorCode); 
typedef void (*SL_FTP_DEL)(S32 slErrorCode); 
typedef void (*SL_FTP_REN)(S32 slErrorCode); 
typedef void (*SL_FTP_LIST)(SL_Ftp_List_Info *pstListInfo, S32 slErrorCode); 
typedef void (*SL_FTP_CLOSE)(S32 slErrorCode); 
typedef struct{
    SL_FTP_LOGIN pstSlFtpLogin;
    SL_FTP_DOWNLOAD pstSlFtpDownload;
    SL_FTP_UPLOAD pstSlFtpUpload;
    SL_FTP_PWD pstSlFtpPwd;
    SL_FTP_CWD pstSlFtpCwd;
    SL_FTP_CDUP pstSlFtpCdup;
    SL_FTP_MKD pstSlFtpMkd;
    SL_FTP_RMD pstSlFtpRmd;
    SL_FTP_DEL pstSlFtpDel;    
    SL_FTP_REN pstSlFtpRen;
    SL_FTP_LIST pstSlFtpList;
    SL_FTP_CLOSE pstSlFtpClose;
}SL_FTP_CALLBACK;
/****************************************************
                                   function
****************************************************/
#include "sl_app.h"
#define SL_Ftp_GetCb AA.SL_Ftp_GetCb_p
#define SL_Ftp_Callback_Init AA.SL_Ftp_Callback_Init_p
/*****************************************************************************
* Name:             SL_Ftp_Open
* Description:    Establish a ftp connnection
* Parameter:     SL_FTP_Connect_Info:   infomation of connection
* Return:           0:   OK;
*                        -1: Fail
******************************************************************************/
#define SL_Ftp_Open AA.SL_Ftp_Open_p
/*****************************************************************************
* Name:             SL_Ftp_Close
* Description:    Close a ftp connnection
* Parameter:     NULL
* Return:           0:   OK;
*                        -1: Fail
******************************************************************************/
#define SL_Ftp_Close AA.SL_Ftp_Close_p
/*****************************************************************************
* Name:             SL_Ftp_Download
* Description:    Download a file from ftp server
* Parameter:     pu8RemoteFile:   the file to be download
*                        emFileType          upload file type(0:ascii;1:binary)
* Return:           0:   OK;
*                        -1: Fail
******************************************************************************/
#define SL_Ftp_Download AA.SL_Ftp_Download_p
/*****************************************************************************
* Name:             SL_Ftp_Upload
* Description:    Upload a file to ftp server
* Parameter:     pu8FileName:   the file to be Upload
*                        pFileData:           upload data address
*                        u32FileLen:         upload data length
*                        emFileType          upload file type(0:ascii;1:binary)
* Return:           0:   OK;
*                        -1: Fail
******************************************************************************/
#define SL_Ftp_Upload AA.SL_Ftp_Upload_p
/*****************************************************************************
* Name:             SL_Ftp_PWD
* Description:    get current dirctory of ftp server
* Return:           0:   OK;
*                        -1: Fail
******************************************************************************/
#define SL_Ftp_PWD AA.SL_Ftp_PWD_p
/*****************************************************************************
* Name:             SL_Ftp_CWD
* Description:    change current dirctory of ftp server
* Parameter:     path: dirctory of changing to
* Return:           0:   OK;
*                        -1: Fail
******************************************************************************/
#define SL_Ftp_CWD AA.SL_Ftp_CWD_p
/*****************************************************************************
* Name:             SL_Ftp_CDUP
* Description:    change current dirctory of ftp server to parent dirctoty
* Parameter:     NULL
* Return:           0:   OK;
*                        -1: Fail
******************************************************************************/
#define SL_Ftp_CDUP AA.SL_Ftp_CDUP_p
/*****************************************************************************
* Name:             SL_Ftp_MKD
* Description:    create dirctory 
* Parameter:     dirName: dirctory name 
* Return:           0:   OK;
*                        -1: Fail
******************************************************************************/
#define SL_Ftp_MKD AA.SL_Ftp_MKD_p
/*****************************************************************************
* Name:             SL_Ftp_RMD
* Description:    delete dirctory 
* Parameter:     dirName: dirctory to delete 
* Return:           0:   OK;
*                        -1: Fail
******************************************************************************/
#define SL_Ftp_RMD AA.SL_Ftp_RMD_p
/*****************************************************************************
* Name:             SL_Ftp_DEL
* Description:    delete file 
* Parameter:     fileName: file to delete 
* Return:           0:   OK;
*                        -1: Fail
******************************************************************************/
#define SL_Ftp_DEL AA.SL_Ftp_DEL_p
/*****************************************************************************
* Name:             SL_Ftp_REN
* Description:    change file name 
* Parameter:     NewName: the new name 
*                        OldName: the file to be change name
* Return:           0:   OK;
*                        -1: Fail
******************************************************************************/
#define SL_Ftp_REN AA.SL_Ftp_REN_p
/*****************************************************************************
* Name:             SL_Ftp_LIST
* Description:    Get file infomation 
* Parameter:     fileName: file name 
* Return:           0:   OK;
*                        -1: Fail
******************************************************************************/
#define SL_Ftp_LIST AA.SL_Ftp_LIST_p
#endif
