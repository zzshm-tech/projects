/*********************************************************************************
 ** File Name:      sl_memory.h                                                *
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
 ** 2013/01/07    Wang Ya Feng       Create
 *********************************************************************************/
#ifndef _SL_MEMORY_H_
#define _SL_MEMORY_H_

#include "sl_type.h"

PVOID SL_GetMemory(U32 ulSize);
BOOL SL_FreeMemory(PVOID mem);
U32 SL_MemMaxCanAllocSize(void);
U32 SL_MemTotalLeftSize(void);

#endif
