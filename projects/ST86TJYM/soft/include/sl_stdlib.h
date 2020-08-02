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
char* SL_itoa(S32 value, char *string, S32 radix);
char* SL_ui64toa(U64 value, U8* string, S32 radix);
S32 SL_atoi(char *string);
U64 SL_atoui64(char *string);
char* SL_Strcpy(char* dest, const char* src);
char* SL_Strncpy(char* dest, const char* src, U16 size);
char* SL_Strcat(char* s1, const char* s2);
char* SL_Strncat(char* s1, const char* s2, U16 size);
U32 SL_Strlen(const char* str);
S32 SL_Strcmp(const char* s1, const char* s2);
S32 SL_Strncmp(const char* s1, const char* s2, U16 size);
void* SL_Memset(void* dest, U8 value, U32 size);
void* SL_Memcpy(void* dest, const void* src, U16 size);
S32 SL_Memcmp(const void* dest, const void* src, U16 size);
void* SL_Memmove(void* dest, const void* src, U16 size);
char* SL_Strstr(const char* s1, const char* s2);
char* SL_Strchr(const char* s1, U16 ch);
S32 SL_Sprintf(char *buf, const char *fmt,...);
volatile void SL_LongJmp (SL_JMP_BUF env,  S32 value);
DOUBLE SL_Sin(DOUBLE x);
DOUBLE SL_Cos(DOUBLE x);
DOUBLE SL_Asin(DOUBLE x);
DOUBLE SL_Acos(DOUBLE x);
DOUBLE SL_Tan(DOUBLE x);
DOUBLE SL_Cot(DOUBLE x);
DOUBLE SL_Atan(DOUBLE x);
DOUBLE SL_Sqrt(DOUBLE x);
DOUBLE SL_Atof(char *string);
DOUBLE SL_Fabs(DOUBLE x);
DOUBLE SL_Atan2(DOUBLE y, DOUBLE x);

#endif 
