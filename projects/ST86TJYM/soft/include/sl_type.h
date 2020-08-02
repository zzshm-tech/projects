/*!
 *	\file sl_type.h
 *	\brief Basic data types in SL1360 platform.
 *
 *	\version 		0.1
 *	\author 		SimpLight
 *	\date 			2013-01-07
 * 	\copyright
 *  Copyright (C) 2006-2012 by SimpLight Nanoelectronics, Ltd.\n
 *                 All rights reserved.\n
 * This material constitutes the trade secrets and confidential,\n
 * proprietary information of SimpLight.  This material is not to\n
 * be disclosed, reproduced, copied, or used in any manner  not\n
 * permitted under license from SimpLight Nanoelectronics Ltd.\n
 */

/*********************************************************************************
 **                        Edit History
 ** -------------------------------------------------------------------------
 ** DATE           NAME             DESCRIPTION
 ** 2013/01/07     Wang Ya Feng          Create.
 *********************************************************************************/

#ifndef _SL_TYPE_H_
#define _SL_TYPE_H_

#ifndef _BASIC_TYPE_
#define _BASIC_TYPE_
typedef unsigned char               u8;
typedef unsigned short              u16;
typedef unsigned long               u32;
typedef char                        s8;
typedef short                       s16;
typedef long                        s32;
#ifdef __cplusplus
#else
typedef unsigned char               bool;
#endif

typedef char                        ascii;
typedef unsigned char               byte;           /*  unsigned 8-bit data     */
typedef unsigned short              word;           /*  unsigned 16-bit data    */
typedef unsigned long               dword;          /*  unsigned 32-bit data    */
typedef unsigned char               uint8;
typedef signed char                 int8;
typedef unsigned short int          uint16;
typedef signed short int            int16;
typedef unsigned int                uint32;
typedef signed int                  int32;
typedef char                        boolean;

#ifndef size_t
typedef unsigned int                size_t;
#endif

#ifndef ssize_t
typedef int                         ssize_t;
#endif

#ifdef WITH_LONG_LONG
typedef unsigned long long          uintmax_t;
typedef long long                   intmax_t;
#else
typedef unsigned long               uintmax_t;
typedef long                        intmax_t;
#endif

typedef unsigned long               UINT32;
typedef unsigned short              UINT16;
typedef long                        INT32;
typedef short                       INT16;
typedef unsigned char               UINT8;
typedef char                        INT8;
#if defined(_M_IX86)
#pragma warning(disable:4142)
typedef __int64                     UINT64;
typedef __int64                     INT64;
#else
typedef unsigned long long          UINT64;
typedef long long                   INT64;
#endif
typedef unsigned char               BOOL;
typedef short                       WCHAR;
typedef char                        CHAR;
typedef unsigned char               BYTE;
typedef float                       FLOAT;
typedef double                      DOUBLE;
typedef UINT32                      HANDLE;
typedef UINT8*                      PUINT8;
typedef UINT32*                     PUINT32;
typedef INT32*                      PINT32;
typedef UINT16*                     PUINT16;
typedef INT16*                      PINT16;
typedef CHAR *                      PCHAR;
typedef void*                       PVOID;
typedef unsigned short              WORD;
typedef unsigned long               DWORD;  /* actually long, but this should be okay */
typedef volatile unsigned char      REG8;
typedef volatile unsigned short     REG16;
typedef volatile unsigned int       REG32;
typedef int                         INT;
typedef char                        S8;
typedef char *                      PS8;
typedef unsigned char               U8;
typedef unsigned char   *           PU8;
typedef unsigned short              pBOOL; 
typedef short int                   S16; 
typedef short int *                 PS16; 
typedef unsigned short int          U16; 
typedef unsigned short int *        PU16;
typedef int                         S32;
typedef int *                       PS32;
typedef unsigned int                U32;
typedef unsigned int *              PU32;
typedef float                       float32;
typedef unsigned long long          U64;
typedef long long                   S64;

#define PUBLIC
#define SRVAPI
#define PROTECTED
#define LOCAL                       static
#define PRIVATE                     static
#define EXPORT                      extern
#define REG                         register
#define CONST                       const
#define VOLATILE                    volatile
#define VOID                        void
#define INLINE                      static inline // Do compiler directives

#ifdef ENABLE_DEPRECATED
#define DEPRECATED __attribute__ ((deprecated))
#else
#define DEPRECATED
#endif /* ENABLE_DEPRECATED */

#define TRUE                        (1==1)
#define FALSE                       (1==0)
#define NULL                        0
#define CS_NULL                     (void*)0
#define NIL                         0
#define CS_NIL                      0
#define HNULL                       0
#define NULL_CHAR                   '\0'

#if defined(UNICODE)
typedef UINT16* PSTR;
typedef CONST UINT16* PCSTR;
#error: it should not define  this type for TCHAR 
typedef UINT16 TCHAR;
#else
typedef UINT8* PSTR;
typedef CONST UINT8* PCSTR;
typedef UINT8 TCHAR;
typedef UINT8* PTCHAR;
#endif

#endif /* _BASIC_TYPE_ */

#if (SL_APART == 1)
#define	APP_ENTRY_START	__attribute__((section(".app_entry")))
#else
#define	APP_ENTRY_START
#endif

#endif /* _SL_TYPE_H_ */

