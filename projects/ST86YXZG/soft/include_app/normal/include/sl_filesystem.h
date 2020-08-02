/*********************************************************************************
 ** File Name:      sl_filesystem.h                                                *
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
#ifndef _SL_FILESYSTEM_H_
#define _SL_FILESYSTEM_H_
#include "sl_type.h"
/****************************************************
                                      macro define
****************************************************/
    // Folder or File name size in byts, to support fat long file name.
#define SL_FS_FILE_NAME_LEN                 255 
    // Max path length,to support fat long file name.
#define SL_FS_PATH_LEN                      260 
    // Max number of  open file
#define SL_FS_NR_OPEN_FILES_MAX             32
    // Max number of directory layer.
#define SL_FS_NR_DIR_LAYERS_MAX             16
    // Max folder or File name size in byts for uincode.
#define SL_FS_FILE_NAME_UNICODE_LEN    (2 * SL_FS_FILE_NAME_LEN) 
    // Max path size for unicode.
#define SL_FS_PATH_UNICODE_LEN              (2 * SL_FS_PATH_LEN) 
// Length of terminated character('\0' for OEM).
#define SL_LEN_FOR_NULL_CHAR                 1
// Size of terminated character('\0' for Unicode).
#define SL_LEN_FOR_UNICODE_NULL_CHAR         2
/****************************************************
                                      enum define
****************************************************/
typedef enum
{
    SL_FS_RDONLY = 0,  // Read only.
    SL_FS_WRONLY,    // Write only.
    SL_FS_RDWR,    // Read and Write.
    // If the file exists, this flag has no effect except as noted under FS_O_EXCL below. Otherwise, the file shall be created.
    SL_FS_CREAT = 00100,
    // If FS_O_CREAT and FS_O_EXCL are set, the function shall fail if the file exists.
    SL_FS_EXCL = 00200,
    // If the file exists, and is a regular file, and the file is successfully opened FS_O_WRONLY or FS_O_RDWR, its length shall be truncated to 0.
    SL_FS_TRUNC = 01000,
    // If set, the file offset shall be set to the end of the file prior to each write.
    SL_FS_APPEND = 02000
}SL_FS_OPEN_TYPE;
typedef enum
{
    SL_FS_SEEK_BEGIN,
    SL_FS_SEEK_CUR,
    SL_FS_SEEK_END,
}SL_FS_SEEK_POS;
typedef enum
{
    SL_FS_DEV_TYPE_FLASH = 0,     // Flash device.
    SL_FS_DEV_TYPE_TFLASH,         // T-Flash device.
}SL_FS_DEV_TYPE;
//----------------------------------------------------------
// File type define.
//----------------------------------------------------------
typedef enum
{
    SL_FS_TYPE_FAT = 1,  // File system type -- FAT.
    SL_FS_TYPE_FFS       // File system type -- FFS.
}SL_FS_TYPE;
//-----------------------------------------------------------
// Format flag.
//-----------------------------------------------------------
typedef enum
{
    SL_FS_FORMAT_QUICK,  // format type -- quick.   
    SL_FS_FORMAT_NORMAL  // format type -- normal(full).    
}SL_FS_FORMAT_TYPE;
/****************************************************
                                    structure define
****************************************************/
//-------------------------------------
// Find file information.
//-------------------------------------
typedef struct
{
    U32  ulStMode;     // Mode of file 
    U32  ulStSize;       // For regular files, the file size in bytes
    U32  ulAtime;   // Time of last access to the file
    U32  ulMtime;   // Time of last data modification
    U32  ulCtime;    // Time of last status(or inode) change
    U8   aucStName[SL_FS_FILE_NAME_LEN + SL_LEN_FOR_NULL_CHAR]; // The name of file. 
}SL_FS_FIND_DATA,*PSL_FS_FIND_DATA;
/****************************************************
                                      function
****************************************************/
#include "sl_app.h"
#define SL_FileGetFreeSize AA.SL_FileGetFreeSize_p
#define SL_FileCreate AA.SL_FileCreate_p
#define SL_FileOpen AA.SL_FileOpen_p
#define SL_FileRead AA.SL_FileRead_p
#define SL_FileWrite AA.SL_FileWrite_p
#define SL_FileSeek AA.SL_FileSeek_p
#define SL_FileGetFilePosition AA.SL_FileGetFilePosition_p
#define SL_FileTruncate AA.SL_FileTruncate_p
#define SL_FileFlush AA.SL_FileFlush_p
#define SL_FileClose AA.SL_FileClose_p
#define SL_FileGetSize AA.SL_FileGetSize_p
#define SL_FileDelete AA.SL_FileDelete_p
#define SL_FileCheck AA.SL_FileCheck_p
#define SL_FileRename AA.SL_FileRename_p
#define SL_FileCreateDir AA.SL_FileCreateDir_p
#define SL_FileRemoveDir AA.SL_FileRemoveDir_p
#define SL_FileCheckDir AA.SL_FileCheckDir_p
#define SL_FileFindFirst AA.SL_FileFindFirst_p
#define SL_FileFindNext AA.SL_FileFindNext_p
#define SL_FileFindClose AA.SL_FileFindClose_p
#define SL_FileSysGetSpaceInfo AA.SL_FileSysGetSpaceInfo_p
#define SL_FileSysFormat AA.SL_FileSysFormat_p
#endif
