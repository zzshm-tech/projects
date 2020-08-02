/*********************************************************************************
 ** File Name:     sl_sms.h                                                *
 ** Author:         Wang Ya Feng                                                  *
 ** DATE:           2013/01/11                                                   *
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
 ** 2013/01/11     Wang Ya Feng       Create
 *********************************************************************************/
#ifndef _SL_SMS_H_
#define _SL_SMS_H_

#include "sl_type.h"

/*************************
           type define
*************************/
typedef void (*SL_SMS_CB_CMGR)(S32 slResult, U8* pucbuf, U16 usLenth);
typedef void (*SL_SMS_CB_CMTI)(S32 slResult, U8 ucIndex, U8* pucbuf, U16 usLenth, U8* pucTelNumber);
typedef void (*SL_SMS_CB_CMGL)(S32 slResult, U8 ucIndex, U8* pucbuf, U16 usLenth);
typedef void (*SL_SMS_CB_CMST)(S32 slResult);
typedef void (*SL_SMS_CB_FULL)(S32 slResult);
typedef void (*SL_SMS_CB_CMGS)(S32 slResult);

typedef struct 
{ 
    U8* pDialNumber;
    U8 nDialNumberSize;
    U8 nType;
    U8 nClir;
    U8 padding[1];
} SL_DIALNUMBER;

typedef struct _SL_SMS_MO_INFO_ADD
{
	U32 hAo;             //save the register handle
	U8* pTmpData;       //For send long msg
	U8* pTmpPara;       //For send long msg
	U8* pTmpData_Addr;       //For send long msg
	U8* pTmpPara_Addr;       //For send long msg
	SL_DIALNUMBER  sNumber;         //User input data
	U8*  pData;           //User input data
	U16  nDataSize;       //User input data
	U16  nLocation;
	U16  nIndex;
	U16  nUTI_UserInput;   //User input
	U16  nUTI_Internal;     //set for invoking SIM api
	U16  i;    //for sth
	U8  nStatus;
	U8  nPath;
	U8  nFormat; //0:PDU; 1:Text
	U8  nMR;
	U32 nErrCode;
	U16  nListCount;
	U16  nListStartIndex;
	U32  nListAddr;
	U16  nListSize;
	U8   nListOption;
	U8   isLongerMsg;    // 1:a longer msg; 0:a normal msg or the last part of a longer msg;
	U16  nLongerMsgID;     // The Msgs which belongs to a same longer msg have a same ID
	U8   nIEIa;            // 0:8-bit reference number; 8: 16-bit reference number
	U8   nLongerMsgCount;  // How many Msgs comprise a longer msg.
	U8   nLongerMsgCurrent;// The current sequence number of a longer msg
	U8   nLongerLastUserDataSize;
	U8   padding[2];
	U8   nLongerEachPartSize;
	U8   nLongerIndexOffset;
	U8   nReadSmsStatus;
	U8   nDCS;
}SL_SMS_MO_INFO_ADD;

typedef struct{
    BOOL bLongMsg;  // 1: the msg received is a longer msg; 0: not
    U16 usLongMsgId; // The Msgs which belongs to a same longer msg have a same ID
    U8 ucMsgCount;  // How many Msgs comprise a longer msg.
    U8 ucMsgCurrent; // The current sequence number of a longer msg
}SL_SMS_LONG_MSG;

typedef struct{
    SL_SMS_CB_CMGR SL_SMS_CMGR;
    SL_SMS_CB_CMTI SL_SMS_CMTI;
    SL_SMS_CB_CMGL SL_SMS_CMGL;
    SL_SMS_CB_CMST SL_SMS_SMST;
    SL_SMS_CB_FULL SL_SMS_FULL;
    SL_SMS_CB_CMGS SL_SMS_CMGS;
}SL_SMS_CALLBACK;

typedef enum{
    SL_SMS_STORAGE_ME = 1,
    SL_SMS_STORAGE_SIM = 2,
    SL_SMS_STORAGE_INVALID
}SL_SMS_STORAGE_TYPE;

typedef enum{
    SL_SMS_LIST_UNREAD,
    SL_SMS_LIST_READ,    
    SL_SMS_LIST_UNSENT,
    SL_SMS_LIST_SENT,
    SL_SMS_LIST_ALL,
    SL_SMS_LIST_INVALID
}SL_SMS_LIST_STATE_TYPE;

typedef enum{
    SL_SMS_DEL_INDEX,
    SL_SMS_DEL_READ,
    SL_SMS_DEL_READ_SENT,
    SL_SMS_DEL_READ_SENT_UNSENT,
    SL_SMS_DEL_ALL
}SL_SMS_DEL_TYPE;

typedef enum{
    SL_SMS_FMT_PDU,
    SL_SMS_FMT_TXT,
    SL_SMS_FMT_INVALID
}SL_SMS_FMT_TYPE;

typedef enum{
    SL_SMS_7BIT = 0,
    SL_SMS_8BIT = 4,
    SL_SMS_UCS2 = 8
}SL_SMS_TXT_MODE_DATA_CODE_TYPE;

/*************************
                    function
*************************/
S32 SL_SmsSendTextSms(U8* pucDialNo, U8* msg);
S32 SL_SmsSendPDUSms(U16 usLenth, U8* msg);
S32 SL_SmsReadSms(U8 ucIndex, SL_SMS_CB_CMGR cb);
void SL_SmsSetNewSmsCallback(SL_SMS_CB_CMTI cb);
S32 SL_SmsSetSmsFormat(U8 ucFormat);
S32 SL_SmsGetSmsFormat(void);
S32 SL_SmsDeleteSms(U8 ucIndex, U8 ucFlag);
S32 SL_SmsSetStorage(SL_SMS_STORAGE_TYPE enMem1, SL_SMS_STORAGE_TYPE enMem2, SL_SMS_STORAGE_TYPE enMem3);
S32 SL_SmsReadSmsList(SL_SMS_LIST_STATE_TYPE ucSmsState, SL_SMS_CB_CMGL cb);
S32 SL_SmsSetInfoCenterNum (U8* pucNum, U8 ucLenth);
S32 SL_SmsGetInfoCenterNum(U8* pucInfoCenterNum);
void SL_SmsGetNetStatusCallback(SL_SMS_CB_CMST cb);
void SL_SmsSetSmsFullCallback(SL_SMS_CB_FULL cb);
void SL_SmsSetSendSmsCallback(SL_SMS_CB_CMGS cb);
U32 SL_SmsSetTxtModeDataCodSch(SL_SMS_TXT_MODE_DATA_CODE_TYPE enSlSmsCodeType);
U8* SL_SmsReadSmsTelNo(void);
S32 SL_SmsGetLongSmsPara(SL_SMS_LONG_MSG* pstSmsLongMsg);

#endif
