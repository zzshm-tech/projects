/*********************************************************************************
 ** File Name:     sl_pbk.h                                                *
 ** Author:         Wang Ya Feng                                                  *
 ** DATE:           2013/07/08                                                   *
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
 ** 2013/07/08     Wang Ya Feng       Create
 *********************************************************************************/
#ifndef _SL_PBK_H_
#define _SL_PBK_H_

#include "sl_type.h"

/************************************************
                                   type define 
                                   
************************************************/
typedef void (*SL_PBK_CB_CNUM)(S32 slResult, U8* pucName, U8* pucTelNo, U8 ucTelType); 
typedef void (*SL_PBK_CB_CPBR)(S32 slResult, U8 ucIndex, U8* pucTelNo, U8 ucTelType, U8* pucName);
typedef void (*SL_PBK_CB_CPBW)(S32 slResult);
typedef void (*SL_PBK_CB_CPBF)(S32 slResult, U8 ucIndex, U8* pucTelNo, U8 ucTelType, U8* pucName);
typedef void (*SL_PBK_CB_CPBS)(S32 slResult);
typedef void (*SL_PBK_CB_CPBD)(S32 slResult);

typedef enum{
    SL_PBK_SM = 0x1,
    SL_PBK_FD = 0x4,    
    SL_PBK_LD = 0x8,
    SL_PBK_ON = 0x20,
    SL_PBK_ERR = 0xff
}SL_PBK_STORAGE_TYPE;

typedef enum{
    SL_CS_GSM,
    SL_CS_HEX,
    SL_CS_UCS2,
    SL_CS_GBK, // equals to PCCP 936 char set
    SL_CS_COUNT,
    SL_CS_INVALID_ = 0x8F,
}SL_PBK_CHARSET;

typedef struct{
    SL_PBK_CB_CNUM pslCnumCb;
    SL_PBK_CB_CPBR pslCpbrCb;
    SL_PBK_CB_CPBW pslCpbwCb;
    SL_PBK_CB_CPBF pslCpbfCb;
    SL_PBK_CB_CPBS pslCpbsCb;
    SL_PBK_CB_CPBD pslCpbdCb;
}SL_PBK_CALLBACK;
/************************************************
                                   function 
                                   
************************************************/
S32 SL_PbkInit(SL_PBK_CALLBACK* cb);
S32 SL_PbkGetOwnNumber(void);
S32 SL_PbkSetStorage(SL_PBK_STORAGE_TYPE enStorageType);
S32 SL_PbkReadEntry(U8 ucStartIndex, U8 ucEndIndex);
S32 SL_PbkWriteEntry(U8 ucIndex, U8* pucTelNo, U8* pucName);
S32 SL_PbkFindEntry(U8* pucName);
S32 SL_PbkDeleteEntry(U8 ucIndex);
void SL_PbkSetCharset(SL_PBK_CHARSET enCharSet);
SL_PBK_CHARSET SL_PbkGetCharSet(void);

#endif
