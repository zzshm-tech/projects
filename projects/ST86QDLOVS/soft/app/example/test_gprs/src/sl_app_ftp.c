#include "sl_type.h"
#include "sl_debug.h"
#include "sl_error.h"
#include "sl_stdlib.h"
#include "sl_system.h"
#include "sl_uart.h"
#include "sl_ftp.h"
#include "sl_tcpip.h"
#include "sl_os.h"
#include "sl_timer.h"
#include "sl_app_event.h"
#include "sl_app_ftp.h"

#define SL_FTP_TEST_DOWNLOAD

/****************************************************
                                  variable

****************************************************/
extern HANDLE g_SLAppFtpTask;
U32 g_slftpdnlen = 0;
U8 *g_slftpdnbuf = NULL;
static U8 aucApn[] = {"CMNET"};
static U8 aucUsrId[] = {"S80"};
static U8 aucPassword[] = {"S80"};

/****************************************************
                                  function

****************************************************/

void SL_AppSendFtpOpenRequest()
{
    SL_EVENT event;
    SL_TASK hTask;
    
    SL_ApiPrint("---> SL Send FTP open msg\n");
    event.nEventId = SL_APP_FTPOP_REQ;
    hTask.element[0] = g_SLAppFtpTask;
    SL_SendEvents(hTask, &event);
}

void SL_AppFtpGprsNetActRsp(U8 ucCidIndex, S32 slErrorCode)
{
    S32 slRet = 0;
    
    SL_ApiPrint("******* SL_TcpipGprsNetActive %d*********\n", slRet);
    if (slErrorCode == SL_RET_OK)
    {
       SL_AppSendFtpOpenRequest();
    }
}

void SL_AppFtpInitGPRS(void)
{
    SL_TCPIP_CALLBACK stSlTcpipCb;
    
    stSlTcpipCb.pstSlnetAct = SL_AppFtpGprsNetActRsp;
    SL_TcpipGprsNetInit(0, &stSlTcpipCb);
    SL_ApiPrint("--->SLAPP gprs attach and active!\n");
}

S32 SL_AppFtpStartTcpip()
{
    S32 slRet = 0;
    
    SL_TcpipGprsApnSet(aucApn, aucUsrId, aucPassword);
    
    SL_ApiPrint("******* SL_TcpipGprsNetActive *********\n");
    slRet = SL_TcpipGprsNetActive();
    if(slRet != SL_RET_OK)
    {
       SL_ApiPrint("SLAPP: gprs net act fail[%d]", slRet);
    }
    return slRet;
}

void SL_AppFtpOpen(void)
{
    SL_FTP_Connect_Info ftp_info;
    S32 ret = 0;
    
    ftp_info.u16DestPort = 21;
    SL_Strcpy(ftp_info.uaUrl, "www.simplnano.com"); //这里一定要是公网的IP地址或、//域名
    SL_Strcpy(ftp_info.uaUsername, "student"); //这里是用户名
    SL_Strcpy(ftp_info.uaPassword, "simplnano"); //这里是用密码
    
    ret = SL_Ftp_Open(&ftp_info);
    SL_ApiPrint("---> FTP open ret val=%d\n", ret);
}

void SL_AppFtpOpenCB(S32 openst)
{
    S32 ret = 0;
    SL_EVENT event;
    
    if (SL_RET_OK == openst)
    {
    #ifdef  SL_FTP_TEST_DOWNLOAD 
       SL_ApiPrint("--->SL ftp open success! Get file attribute!\n");
       ret = SL_Ftp_Download("sl_test.bin", 1);
       SL_ApiPrint("--->SL ftp file download ret=%d\n", ret);
    #endif
    }
    else
    {
       SL_ApiPrint("---> [ERROR] SL ftp open fail, and send ftp open msg!\n");
    }
}

void SL_AppFtpDownloadCB(U8 *cacheaddr, U32 cachelen, S32 dnldst)
{
    S32 ret = 0;
    
    if (SL_RET_OK == dnldst)
    {
       SL_ApiPrint("--->SL ftp download success! Close ftp!\n");
       g_slftpdnbuf = (U8*)cacheaddr; //此buffer中就是下载的bin文件
       g_slftpdnlen = cachelen;
    }
    else
    {
       SL_ApiPrint("---> [ERROR] SL ftp download fail! Close ftp!\n");
    }
    ret = SL_Ftp_Close();
    SL_ApiPrint("---> SL ftp close ret=%d\n", ret);
}

void SL_AppFtpCloseCB(S32 closest)
{
    #ifdef SL_FTP_TEST_DOWNLOAD
    SL_EVENT event;
    SL_TASK hTask;
    #endif
    
    if (SL_RET_OK == closest)
    {
       SL_ApiPrint("--->SL ftp close success!\n");
    #ifdef SL_FTP_TEST_DOWNLOAD
       SL_ApiPrint("--->SL ftp close success! Send msg to update app!\n");
       event.nEventId = SL_APP_UPDATE_REQ;
       event.nParam1 = (U32)g_slftpdnbuf;
       event.nParam2 = g_slftpdnlen;
       hTask.element[0] = g_SLAppFtpTask;
       SL_SendEvents(hTask, &event);
    #endif
    }
    else
    {
       SL_ApiPrint("---> [ERROR] SL ftp close fail!\n");
    }
}

void SL_AppFtpInit()
{
    SL_FTP_CALLBACK ftp_callbk;

    ftp_callbk.pstSlFtpLogin = SL_AppFtpOpenCB;
    ftp_callbk.pstSlFtpDownload = SL_AppFtpDownloadCB;
    ftp_callbk.pstSlFtpClose = SL_AppFtpCloseCB;
    SL_Ftp_Callback_Init(&ftp_callbk);
    SL_AppFtpInitGPRS();
}

