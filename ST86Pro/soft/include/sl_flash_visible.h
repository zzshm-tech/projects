/*********************************************************************************
** File Name:      sl_flash_visible.h                                                *
** Author:         Wang Ya Feng                                                  *
** DATE:           2013/01/07                                                   *
** Copyright:      2013 by SimpLight Nanoelectronics,Ltd. All Rights Reserved.  *
*********************************************************************************

*********************************************************************************
** Description:    Implement for common function of API layer                   *
** Note:           None                                                         *
*********************************************************************************

*********************************************************************************
**                        Edit History                                          *
** -------------------------------------------------------------------------    *
** DATE           NAME             DESCRIPTION                                  *
** 2015/09/16    Wang Ya Feng       Create
*********************************************************************************/
#ifndef _SL_FLASH_VISIBLE_H_
#define _SL_FLASH_VISIBLE_H_

#include "sl_type.h"

/****************************************************
                                  macro define

****************************************************/
/****************************************************
                                  type define

****************************************************/
typedef struct _SL_FLASH_DEV_INFO {
  U32 ulSectorSize;
  U32 ulTotalSize;
  U8* pucBaseAddr;
}SL_FLASH_DEV_INFO_STRUCT; 


/****************************************************
                                  function

****************************************************/
S32 SL_FlashGetInfo(SL_FLASH_DEV_INFO_STRUCT* pstFlashInfo);
S32 SL_FlashEraseSector(U32 ulAddrOffset);
S32 SL_FlashWrite(U32 ulAddrOffset,
                      CONST U8* pBuffer, U32 nNumberOfBytesToWrite, U32* pNumberOfBytesWritten);
S32 SL_FlashRead(U32 ulAddrOffset, U8* pBuffer, U32 nNumberOfBytesToRead, U32* pNumberOfBytesRead);

#endif

