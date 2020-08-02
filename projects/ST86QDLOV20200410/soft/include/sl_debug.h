/*********************************************************************************
 ** File Name:      sl_debug.h                                                *
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

#ifndef _SL_DEBUG_H_
#define _SL_DEBUG_H_

#include "sl_type.h"

/*! \fn SL_Print(const INT8 *fmt, ...)
 * \brief A printf-like funtion for information output through uart
 * \param[in] fmt format for print string. Please refer printf for reference
 * \return the print result
 * \example ex_kpd.c
 */
S32 SL_Print(const S8 *fmt, ...);
S32 SL_ApiPrint(const S8 *fmt, ...);
void SL_MEMBLOCK(U8* buffer, U16 len, U8 radix);
void SL_HstSendEvt(U32 x);

#endif
