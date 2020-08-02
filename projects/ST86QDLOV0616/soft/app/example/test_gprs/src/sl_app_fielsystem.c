#include "sl_type.h"
#include "sl_debug.h"
#include "sl_error.h"
#include "sl_uart.h"
#include "sl_app_event.h"
#include "sl_filesystem.h"

/****************************************************
                                  variable

****************************************************/
static unsigned char file_data[] = {"GPRSLOGN0100000000000086566200000000"};

/****************************************************
                                  extern variable

****************************************************/
extern HANDLE g_SLApp3;

/****************************************************
                                  extern function

****************************************************/
extern void SL_AppSendMsg(HANDLE stTask, U32 ulMsgId, U32 ulParam);

/****************************************************
                                  function

****************************************************/

void SL_AppFileCreateHandle(U8* pstFileName)
{
    S32 fd = 0;
    S32 slLenth = 0;
    S32 slRet = 0;
    
    fd = SL_FileOpen(pstFileName, SL_FS_RDWR | SL_FS_CREAT);
    if(fd < 0)
    {
        SL_ApiPrint("SLAPP: SL_FileOpen fail!");
        return;
    }
    #if 1
    slLenth = SL_FileWrite(fd, file_data, sizeof(file_data));
    if(slLenth < 0)
    {
        SL_ApiPrint("SLAPP: SL_FileWrite fail!");
        slRet = SL_FileClose(fd);
        SL_ApiPrint("SLAPP: SL_FileClose ret[%x]!", slRet);
        return;
    }
    #else
    slLenth = SL_FileGetSize(fd);
    SL_ApiPrint("SLAPP: file size[%d]!", slLenth);
    #endif
    slRet = SL_FileClose(fd);
    SL_ApiPrint("SLAPP: SL_FileClose ret[%x]!", slRet);
    if(slRet == SL_RET_OK)
    {
        //SL_AppSendMsg(g_SLApp3, EVT_APP_READ_FILE, 0);
    }
    return;
}

void SL_AppFileReadHandle()
{
    S32 fd = 0;
    S32 slLenth = 0;
    S32 slRet = 0;
    U8 aucTestData[100] = {0};
        
    fd = SL_FileOpen("¼òÔ¼ÄÉ", SL_FS_RDONLY);
    if(fd < 0)
    {
        SL_ApiPrint("SLAPP: SL_FileOpen fail!");
        return;
    }
    slLenth = SL_FileRead(fd, aucTestData, sizeof(file_data));
    if(slLenth < 0)
    {
        SL_ApiPrint("SLAPP: SL_FileWrite fail!");
        slRet = SL_FileClose(fd);
        SL_ApiPrint("SLAPP: SL_FileClose ret[%x]!", slRet);
        return;
    }
    SL_ApiPrint("SLAPP: SL_FileRead ok, aucTestData[%s], lenth[%d]!", aucTestData, slLenth);
    slRet = SL_FileClose(fd);
    SL_ApiPrint("SLAPP: SL_FileClose ret[%x]!", slRet);
    return;
}

void SL_AppFileFind(U8* pucFileName)
{
    S32 slRet = 0;
    SL_FS_FIND_DATA SlFsFindData;

    slRet = SL_FileFindFirst(pucFileName, &SlFsFindData);
    SL_ApiPrint("SLAPP: SL_AppFileFind ret[%d]", slRet);
    SL_ApiPrint("SLAPP: SL_AppFileFind mode[%d], size[%d], atime[%d], mtime[%d], ctime[%d], name[%s]", 
            SlFsFindData.ulStMode, SlFsFindData.ulStSize, SlFsFindData.ulAtime, SlFsFindData.ulMtime, SlFsFindData.ulCtime,
            SlFsFindData.aucStName);
    SL_MEMBLOCK(SlFsFindData.aucStName, 20, 16);
    SL_UartSendData(SL_UART_1, SlFsFindData.aucStName, 10);
}
