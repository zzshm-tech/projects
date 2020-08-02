/*********************************************************************************
 ** File Name:      sl_stdlib.h                                                *
 ** Author:         Wang Ya Feng                                                  *
 ** DATE:           2013/01/09                                                   *
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
 ** 2013/01/09    Wang Ya Feng       Create
 *********************************************************************************/
#ifndef _SL_STDLIB_H_
#define _SL_STDLIB_H_
#include "sl_type.h"
/****************************************************
                                   type define
****************************************************/
typedef U32 SL_JMP_BUF[12];
#define SL_SetJmp(env)  (setjmp(env))
/****************************************************
                                   function
****************************************************/
#include "sl_app.h"
#define SL_itoa AA.SL_itoa_p
#define SL_ui64toa AA.SL_ui64toa_p
#define SL_atoi AA.SL_atoi_p
#define SL_atoui64 AA.SL_atoui64_p
#define SL_Strcpy AA.SL_Strcpy_p
#define SL_Strncpy AA.SL_Strncpy_p
#define SL_Strcat AA.SL_Strcat_p
#define SL_Strncat AA.SL_Strncat_p
#define SL_Strlen AA.SL_Strlen_p
#define SL_Strcmp AA.SL_Strcmp_p
#define SL_Strncmp AA.SL_Strncmp_p
#define SL_Memset AA.SL_Memset_p
#define SL_Memcpy AA.SL_Memcpy_p
#define SL_Memcmp AA.SL_Memcmp_p
#define SL_Memmove AA.SL_Memmove_p
#define SL_Strstr AA.SL_Strstr_p
#define SL_Strchr AA.SL_Strchr_p
#define SL_Sprintf AA.SL_Sprintf_p
#define SL_LongJmp AA.SL_LongJmp_p
#define SL_Sin AA.SL_Sin_p
#define SL_Cos AA.SL_Cos_p
#define SL_Asin AA.SL_Asin_p
#define SL_Acos AA.SL_Acos_p
#define SL_Tan AA.SL_Tan_p
#define SL_Cot AA.SL_Cot_p
#define SL_Atan AA.SL_Atan_p
#define SL_Sqrt AA.SL_Sqrt_p
#define SL_Atof AA.SL_Atof_p
#define SL_Fabs AA.SL_Fabs_p
#define SL_Atan2 AA.SL_Atan2_p
#endif 
