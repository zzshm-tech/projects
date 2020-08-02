#ifndef SL_APP_H
#define SL_APP_H
#include "sl_assistgps.h"
#include "sl_audio.h"
#include "sl_audiorecord.h"
#include "sl_bus.h"
#include "sl_call.h"
#include "sl_debug.h"
#include "sl_dtmf.h"
#include "sl_error.h"
#include "sl_event.h"
#include "sl_filesystem.h"
#include "sl_gpio.h"
#include "sl_gprs.h"
#include "sl_kpd.h"
#include "sl_lowpower.h"
#include "sl_memory.h"
#include "sl_os.h"
#include "sl_rtc.h"
#include "sl_sms.h"
#include "sl_stdlib.h"
#include "sl_system.h"
#include "sl_tcpip.h"
#include "sl_timer.h"
#include "sl_type.h"
#include "sl_uart.h"
#include "sl_pbk.h"
#include "sl_ftp.h"
#include "sl_lbs.h"
#include "sl_http.h"

#define __FUNCTAB__ __attribute__((section(".functab")))


struct FUNCPTR {
  S32 (*SL_AssistGpsConfig_p)(S8* pucServername, U16 usPort);
  S32 (*SL_AssistGpsGetLoc_p)(SL_ASSISTGPS_GET_LOC_CB cb);
  S32 (*SL_AssistGpsArpConfig_p)(U32 uiServer, U32 uiLanguage);
  S32 (*SL_AssistGpsGetAddr_p)(U32 ulLongitude, U32 ulLatitude, SL_ASSISTGPS_GET_ADDR_CB cb);
  S32 (*SL_AssistGpsGetUtc_p)(SL_ASSISTGPS_UTC_t *pUtcTime);
  S32 (*SL_AudioStartPlayFile_p)(HANDLE fileHandle, U8 ucFileType, U8 ucAudioPath, SL_AUDIO_PLAY_FINISH stAudioPlayCb);
  S32 (*SL_AudioStopPlayFile_p)(void);
  S32 (*SL_AudioPlayStream_p)(S32* pucStream, U32 ulStreamSize, U32 ulFormat, BOOL bLoop, SL_AUDIO_PLAY_FINISH stAudioPlayCb);
  S32 (*SL_AudioStopPlayStream_p)(void);
  S32 (*SL_AudioPlayPause_p)(BOOL bPause);
  S32 (*SL_AudioSetVolume_p)(U8 ucVolType, U8 ucLevel);
  S32 (*SL_AudioSetPathVolume_p)(U8 ucLevel);
  S32 (*SL_AudioChangePath_p)(SL_AUDIO_PATH_TYPE enAudioPath);
  S32 (*SL_AudioSetMicGain_p)(U8 ucLevel);
  S32 (*SL_AudioSetSideToneGain_p)(U8 ucLevel);
  S32 (*SL_AudioGetMicGain_p)(void);
  S32 (*SL_AudioGetSideToneGain_p)(void);
  S32 (*SL_AudioVts_p)(U8* pucDtmfStr, U8 ucLenth);
  S32 (*SL_AudioRecordInit_p)(SL_AUDIOREC_CALLBACK* cb);
  S32 (*SL_AudioRecordStart_p)(U8* pucfilename);
  S32 (*SL_AudioRecordStop_p)(SL_AUDIOREC_INFO* pstSlAudRecInfo);
  S32 (*SL_AudioRecordPlayStart_p)(U8* pucfilename);
  S32 (*SL_AudioRecordPlayStop_p)(void);
  S32 (*SL_AudioPlayToOpposite_p)(U8* pucfilename, U8 ucLoopFlag);
  S32 (*SL_AudioStopPlayToOpposite_p)(void);
  S32 (*SL_VoiceRecordStart_p)(U8 time);
  S32 (*SL_VoiceRecordStop_p)();
  S32 (*SL_AudioPlayTTSToOpposite_p)(U8* pucText, U32 ulSize);
  S32 (*SL_AudioStopPlayTTSToOpposite_p)(void);
  S32 (*SL_I2cOpen_p)(U8 ucI2cBusid);
  S32 (*SL_I2cClose_p)(U8 ucI2cBusid);
  S32 (*SL_I2cWriteByte_p)(U8 ucI2cBusid, U32 ulSlaveAddr, U32 ulMemAddr, U8 data);
  S32 (*SL_I2cReadByte_p)(U8 ucI2cBusid, U32 ulSlaveAddr, U32 ulMemAddr, U8* pData);
  S32 (*SL_I2cSendData_p)(U8 ucI2cBusid, U32 ulSlaveAddr, U32 ulMemAddr, CONST U8* pData, U32 ulLength);
  S32 (*SL_I2cReadData_p)(U8 ucI2cBusid, U32 ulSlaveAddr, U32 ulMemAddr, U8* pData, U32 ulLength);
  S32 (*SL_SpiOpen_p)(U8 ucSpiBusid, U8 ucCsNo, CONST SL_SPI_CFG_TYPE* pstSpiCfg);
  S32 (*SL_SpiClose_p)(U8 ucSpiBusid, U8 ucCsNo);
  S32 (*SL_SpiWriteData_p)(U8 ucSpiBusid, U8 ucCsNo, CONST U8* pStartAddress, U32 ulLength);
  S32 (*SL_SpiReadData_p)(U8 ucSpiBusid, U8 ucCsNo, U8* pDestAddress, U32 ulLength);
  BOOL (*SL_SpiActCs_p)(U8 ucSpiBusid, U8 ucCsNo);
  BOOL (*SL_SpiDeactCs_p)(U8 ucSpiBusid, U8 ucCsNo);
  S32 (*SL_SpiSetPinState_p)(U8 ucSpiBusid, U8 ucPin, U8 ucPinState);
  BOOL (*SL_SpiTxDmaDone_p)(U8 ucSpiBusid, U8 ucCsNo);
  BOOL (*SL_SpiRxDmaDone_p)(U8 ucSpiBusid, U8 ucCsNo);
  VOID (*SL_SpiClearTxDmaDone_p)(U8 ucSpiBusid);
  VOID (*SL_SpiClearRxDmaDone_p)(U8 ucSpiBusid);
  BOOL (*SL_SpiTxFinished_p)(U8 ucSpiBusid, U8 ucCsNo);
  VOID (*SL_SpiFlushFifo_p)(U8 ucSpiBusid);
  VOID (*SL_SpiTxIfcChannelRelease_p)(U8 ucSpiBusid);
  VOID (*SL_SpiRxIfcChannelRelease_p)(U8 ucSpiBusid);
  VOID (*SL_SpiStartInfiniteWriteMode_p)(U8 ucSpiBusid, U8 ucCsNo, SL_SPI_INFINITE_TX* penSpiInifiTx);
  VOID (*SL_SpiStopInfiniteWriteMode_p)(U8 ucSpiBusid, U8 ucCsNo);
  S32 (*SL_CallInit_p)(SL_CALL_CALLBACK* cb);
  S32 (*SL_CallDial_p)(SL_CALL_TYPE enCallType, char* callNo);
  S32 (*SL_CallAnswer_p)(void);
  S32 (*SL_CallHangup_p)(void);
  SL_CALL_ACTIVE_STATE (*SL_CallGetActiveStatus_p)(void);
  S32 (*SL_Print_p)(const S8 *fmt, ...);
  S32 (*SL_ApiPrint_p)(const S8 *fmt, ...);
  void (*SL_MEMBLOCK_p)(U8* buffer, U16 len, U8 radix);
  void (*SL_HstSendEvt_p)(U32 x);
  S32 (*SL_DtmfStart_p)(SL_DTMF_DECTECT_CB cb, SL_TMF_DET_TYPE enSlTmfType);
  S32 (*SL_DtmfThreshSet_p)(U8 ucThresh1, U8 ucThresh2);
  S32 (*SL_DtmfStop_p)(void);
  S32 (*SL_DtmfSendCode_p)(U8 ucCodeNum, U8* pucCode, U32* pucSendTimeMs, U32* pucMuteTimeMs);
  S32 (*SL_FileGetFreeSize_p)(void);
  S32 (*SL_FileCreate_p)(U8* pstFileName);
  S32 (*SL_FileOpen_p)(U8* pstFileName, SL_FS_OPEN_TYPE enFlag);
  S32 (*SL_FileRead_p)(S32 slFilehandle, U8* pucbuff, U32 ulLenth);
  S32 (*SL_FileWrite_p)(S32 slFilehandle, U8* pucbuff, U32 ulLenth);
  S32 (*SL_FileSeek_p)(S32 slFilehandle, S32 sloffset, SL_FS_SEEK_POS stSeekPos);
  S32 (*SL_FileGetFilePosition_p)(S32 slFilehandle, U32* pulPosition);
  S32 (*SL_FileTruncate_p)(S32 slFilehandle, U32 ulFileSize);
  S32 (*SL_FileFlush_p)(S32 slFilehandle);
  S32 (*SL_FileClose_p)(S32 slFilehandle);
  S32 (*SL_FileGetSize_p)(S32 slFilehandle);
  S32 (*SL_FileDelete_p)(U8* pucFileName);
  S32 (*SL_FileCheck_p)(U8* pucFileName);
  S32 (*SL_FileRename_p)(U8* pucOldname, U8* pucNewname);
  S32 (*SL_FileCreateDir_p)(U8* pucDirName);
  S32 (*SL_FileRemoveDir_p)(U8* pucDirName);
  S32 (*SL_FileCheckDir_p)(U8* pucDirName);
  S32 (*SL_FileFindFirst_p)(U8* pucFileName, PSL_FS_FIND_DATA pSlFsFindData);
  S32 (*SL_FileFindNext_p)(S32 slFilehandle, PSL_FS_FIND_DATA pSlFsFindData);
  S32 (*SL_FileFindClose_p)(S32 slFilehandle);
  S32 (*SL_FileSysGetSpaceInfo_p)(SL_FS_DEV_TYPE enStorage, U32* pulFreeSpace, U32* pulTotalSpace);
  S32 (*SL_FileSysFormat_p)(SL_FS_TYPE enSlFsType, SL_FS_FORMAT_TYPE enSlFlag);
  U32 (*SL_GpioGetDir_p)(U32 gpiox);
  void (*SL_GpioSetDir_p)(U32 gpiox, U32 gpio_dir);
  U32 (*SL_GpioRead_p)(U32 gpiox);
  void (*SL_GpioWrite_p)(U32 gpiox, U32 pin_val);
  void (*SL_GpoWrite_p)(U32 gpo, U32 pin_val);
  void (*SL_GpioIntDetectInit_p)(U32 gpiox, SL_GPIO_IRQ_MASK* pstIrqMask, SL_GPIO_IRQ_HANDLER Handler);
  void (*SL_GpioIntDetectClose_p)(U32 gpiox);
  S32 (*SL_GprsAtt_p)(U8 ucState, SL_GPRS_ATT_CB cb);
  S32 (*SL_GprsSetPdp_p)(U8 ucCid, U8 ucPdpType, U8* pucApn);
  S32 (*SL_GprsAct_p)(U8 ucCid, U8 ucState, SL_GPRS_ACT_CB cb);
  S32 (*SL_GprsGetAttState_p)(U8* pucAttState);
  S32 (*SL_GprsGetActState_p)(U8 ucCid, U8* pucActState);
  S32 (*SL_GprsSetPdpWithUsr_p)(U8 ucCid, U8 ucPdpType, U8* pucApn, U8* pucUsr, U8* pucPasswrd);
  void (*SL_KeyEventCallback_p)(SL_KEY_CB_UPDW cb);
  void (*SL_LpwrEnterDSleep_p)(SL_WAKEUP_TYPE_t wkup_type);
  void (*SL_LpwrEnterWakeup_p)(void);
  SL_LPWR_STATUS_t (*SL_LpwrGetCurrSt_p)(void);
  PVOID (*SL_GetMemory_p)(U32 ulSize);
  BOOL (*SL_FreeMemory_p)(PVOID mem);
  U32 (*SL_MemMaxCanAllocSize_p)(void);
  U32 (*SL_MemTotalLeftSize_p)(void);
  HANDLE (*SL_CreateTask_p)(SL_TASK_ENTRY pTaskEntry, U16 usStackSize, U8 ucPriotiry, PCSTR pTaskName);
  HANDLE (*SL_GetAppTaskHandle_p)(VOID);
  S32 (*SL_CreateSemaphore_p)(SL_SEMAPHORE *semaphore, U32 initial_count);
  S32 (*SL_DeleteSemaphore_p)(SL_SEMAPHORE semaphore);
  S32 (*SL_TakeSemaphore_p)(SL_SEMAPHORE semaphore, U32 timeout);
  S32 (*SL_GiveSemaphore_p)(SL_SEMAPHORE semaphore);
  S32 (*SL_SendEvents_p)(SL_TASK hTask, SL_EVENT *pEvent);
  void (*SL_GetEvent_p)(SL_TASK hTask, SL_EVENT* pstEvent);
  U32 (*SL_CreateMutex_p)(void);
  U8 (*SL_TakeMutex_p)(U8 ucMutexId);
  void (*SL_GiveMutex_p)(U8 ucMutexId, U8 ucUserId);
  void (*SL_DeleteMutex_p)(U8 ucMutexId);
  U32 (*SL_GetCurrentTaskPriority_p)(void);
  U32 (*SL_SysRuntime_p)(VOID);
  HANDLE (*SL_EnterCriticalSection_p)(void);
  BOOL (*SL_ExitCriticalSection_p)(HANDLE hSection);
  S32 (*SL_RtcSet_p)(SL_RTC_TIME* pstSlRtcTime);
  S32 (*SL_RtcGet_p)(SL_RTC_TIME* pstSlRtcTime);
  S32 (*SL_RtcSetAlarm_p)(SL_RTC_ALARM *pstSlRtcAlarm);
  S32 (*SL_RtcDelAlarm_p)(U8 ucIdx);
  S32 (*SL_RtcClrAllAlarm_p)(VOID);
  S32 (*SL_RtcGetLastErr_p)(VOID);
  S32 (*SL_RtcSetAlmCb_p)(SL_ALM_CB cb);
  S32 (*SL_SmsSendTextSms_p)(U8* pucDialNo, U8* msg);
  S32 (*SL_SmsSendPDUSms_p)(U16 usLenth, U8* msg);
  S32 (*SL_SmsReadSms_p)(U8 ucIndex, SL_SMS_CB_CMGR cb);
  void (*SL_SmsSetNewSmsCallback_p)(SL_SMS_CB_CMTI cb);
  S32 (*SL_SmsSetSmsFormat_p)(U8 ucFormat);
  S32 (*SL_SmsGetSmsFormat_p)(void);
  S32 (*SL_SmsDeleteSms_p)(U8 ucIndex, U8 ucFlag);
  S32 (*SL_SmsSetStorage_p)(SL_SMS_STORAGE_TYPE enMem1, SL_SMS_STORAGE_TYPE enMem2, SL_SMS_STORAGE_TYPE enMem3);
  S32 (*SL_SmsReadSmsList_p)(SL_SMS_LIST_STATE_TYPE ucSmsState, SL_SMS_CB_CMGL cb);
  S32 (*SL_SmsSetInfoCenterNum_p) (U8* pucNum, U8 ucLenth);
  S32 (*SL_SmsGetInfoCenterNum_p)(U8* pucInfoCenterNum);
  void (*SL_SmsGetNetStatusCallback_p)(SL_SMS_CB_CMST cb);
  void (*SL_SmsSetSmsFullCallback_p)(SL_SMS_CB_FULL cb);
  void (*SL_SmsSetSendSmsCallback_p)(SL_SMS_CB_CMGS cb);
  U32 (*SL_SmsSetTxtModeDataCodSch_p)(SL_SMS_TXT_MODE_DATA_CODE_TYPE enSlSmsCodeType);
  U8* (*SL_SmsReadSmsTelNo_p)(void);
  S32 (*SL_SmsGetLongSmsPara_p)(SL_SMS_LONG_MSG* pstSmsLongMsg);
  char* (*SL_itoa_p)(S32 value, char *string, S32 radix);
  char* (*SL_ui64toa_p)(U64 value, U8* string, S32 radix);
  S32 (*SL_atoi_p)(char *string);
  U64 (*SL_atoui64_p)(char *string);
  char* (*SL_Strcpy_p)(char* dest, const char* src);
  char* (*SL_Strncpy_p)(char* dest, const char* src, U16 size);
  char* (*SL_Strcat_p)(char* s1, const char* s2);
  char* (*SL_Strncat_p)(char* s1, const char* s2, U16 size);
  U32 (*SL_Strlen_p)(const char* str);
  S32 (*SL_Strcmp_p)(const char* s1, const char* s2);
  S32 (*SL_Strncmp_p)(const char* s1, const char* s2, U16 size);
  void* (*SL_Memset_p)(void* dest, U8 value, U32 size);
  void* (*SL_Memcpy_p)(void* dest, const void* src, U16 size);
  S32 (*SL_Memcmp_p)(const void* dest, const void* src, U16 size);
  void* (*SL_Memmove_p)(void* dest, const void* src, U16 size);
  char* (*SL_Strstr_p)(const char* s1, const char* s2);
  char* (*SL_Strchr_p)(const char* s1, U16 ch);
  S32 (*SL_Sprintf_p)(char *buf, const char *fmt,...);
  volatile void (*SL_LongJmp_p) (SL_JMP_BUF env,  S32 value);
  DOUBLE (*SL_Sin_p)(DOUBLE x);
  DOUBLE (*SL_Cos_p)(DOUBLE x);
  DOUBLE (*SL_Asin_p)(DOUBLE x);
  DOUBLE (*SL_Acos_p)(DOUBLE x);
  DOUBLE (*SL_Tan_p)(DOUBLE x);
  DOUBLE (*SL_Cot_p)(DOUBLE x);
  DOUBLE (*SL_Atan_p)(DOUBLE x);
  DOUBLE (*SL_Sqrt_p)(DOUBLE x);
  DOUBLE (*SL_Atof_p)(char *string);
  DOUBLE (*SL_Fabs_p)(DOUBLE x);
  DOUBLE (*SL_Atan2_p)(DOUBLE y, DOUBLE x);
  void (*SL_Reset_p)(void);
  void (*SL_Sleep_p)(U32 ulMsecond);
  void (*SL_PowerDown_p)(U8 ucPowerType);
  void (*SL_PowerOn_p)(void);
  BOOL (*SL_StartWatchDog_p)(U32 ulTime);
  void (*SL_FeedWatchDog_p)(void);
  void (*SL_StopWatchDog_p)(void);
  S32 (*SL_GetCoreVer_p)(U8* pucVer, U16 usLenth);
  BOOL (*SL_CoreInitFinish_p)(void);
  void (*SL_GetPowerOnReason_p)(U8* penCause);
  void (*SL_GetDeviceCurrentRunState_p)(S32* plSimCard, S32* plCreg, S32* plCgreg, S8* pucRssi, U8* pucBer);
  BOOL (*SL_IsSIMInserted_p)(void);
  S32 (*SL_GSM_GetIMEI_p)(U8* pucImei, U16 usLenth);
  S32 (*SL_GSM_GetGMR_p)(U8* pucGmr, U16 usLenth);
  U8 (*SL_GSM_GetCALIB_p)();
  S32 (*SL_GSM_GetSN_p)(U8* pucImei, U16 usLenth);
  S32 (*SL_SIM_GetIMSI_p)(U8* pucImsi, U16 usLenth);
  S32 (*SL_GSM_GetPSN_p)(U8* pucPsn, U16 usLenth);
  S32 (*SL_GSM_GetSIGN_p)(U8* pucSign, U16 usLenth);
  S32 (*SL_GSM_WriteCfun_p)(U8 ucCfun, U16 usLenth);
  VOID (*SL_GSM_WriteTest_p)(U8 ucTest,U8* testinfo);
  S32 (*SL_GSM_WriteSign_p)(U8* pucSign, U16 usLenth);
  S32 (*SL_GSM_WriteIMEI_p)(U8* pucImei, U16 usLenth);
  S32 (*SL_GSM_WriteSN_p)(U8* pucImei, U16 usLenth);
  S32 (*SL_GetOperator_p)(U8* pucOperate, U8 ucLenth);
  S32 (*SL_RetrieveNodeBInfo_p)(SL_NW_NODEB_INFO* pstSlNodeBInfo);
  S32 (*SL_GetNwStatus_p)(void);
  void (*SL_PmuEnablePower_p)(SL_PMD_POWER_ID_TYPE enPmdId, BOOL on);
  U16 (*SL_GetAdcValue_p)(SL_ADC_CHAN enChan);
  SL_APP_UPDATE_STATUS (*SL_AppUpdateInit_p)(U8 *buf, U32 len);
  S32 (*SL_SimPinInit_p)(SL_SIM_CALLBACK* cb);
  S32 (*SL_SimEnterPwd_p)(U8* pucPwd);
  S32 (*SL_SimGetAuthInfo_p)(void);
  S32 (*SL_SimChangePwd_p)(U8 ucFac, U8* pucOldPwd, U8* pucNewPwd);
  S32 (*SL_SimLock_p)(U8 ucFac, U8 ucMode, U8* pucPwd);
  S32 (*SL_SimLockStatusQuery_p)(U8 ucFac);
  S32 (*SL_SimSetPwd_p)(U8* pucPukPwd, U8* pucPinPwd);
  S32 (*SL_SimDropCbInit_p)(SL_SIM_CB_DROP cb);
  S32 (*SL_SimGetCCID_p)(SL_SIM_CB_GET_CCID cb);
  S32 (*SL_RetrieveCellInfo_p)(SL_NW_CELL_TYPE enCellType, SL_NW_CB_CELL_INFO cb);
  VOID (*SL_PwlSetLevel_p)(SL_PWL_ID enSlPwlId, U8 ucLevel);
  VOID (*SL_SetFlightMode_p)(SL_FLIGHT_MODE_CB cb);
  VOID (*SL_ExitFlightMode_p)(SL_EXIT_FLIGHT_MODE_CB cb);
  VOID (*SL_InitBatteryCb_p)(SL_BATTERY_CB cb);
  VOID (*SL_InitPwrOnCb_p)(SL_PWRON_CB cb);
  VOID (*SL_InitPwrOffCb_p)(SL_PWROFF_CB cb);
  void (*SL_FintIrqSetMask_p)(BOOL mask);
  BOOL (*SL_GetRfTemp_p)(U8* pucTemp);
  VOID (*SL_JammFuncInit_p)(SL_JAMM_DETECT_CB cb);
  VOID (*SL_JammFuncEnable_p)(VOID);
  VOID (*SL_JammFuncDisable_p)(VOID);
  VOID (*SL_SetCustomizationSettings_p)(uint8 index, uint8 *pData, uint16 len);
  VOID (*SL_GetCustomizationSettings_p)(uint8 index, uint8 *pData, uint16 len);
  VOID (*SL_PmdSetLevel_p)(SL_PMD_LED id, UINT32 level);
  S32 (*SL_NwSetRegistrationManual_p)(U8* pucOpId, SL_NW_SET_REG_MANUAL_CB cb);
  S32 (*SL_TcpipGprsNetInit_p)(U8 ucCidIndex, SL_TCPIP_CALLBACK *cb);
  S32 (*SL_TcpipGprsApnSet_p)(U8* pucApn, U8* pucUsrId, U8* pucPassword);
  S32 (*SL_TcpipGprsApnGet_p)(U8* pucApn, U8* pucUsrId, U8* pucPassword);
  S32 (*SL_TcpipGprsNetActive_p)(void);
  S32 (*SL_TcpipGprsNetDeactive_p)(void);
  S32 (*SL_TcpipGprsNetGetState_p)(U8* pucNetState, U8* pucGprsRegState);
  S32 (*SL_TcpipSocketCreate_p)(S32 slSocketId, U8 ucSocketType);
  S32 (*SL_TcpipSocketClose_p)(S32 slSocketId);
  S32 (*SL_TcpipSocketConnect_p)(S32 slSocketId, U8* pucIpAddr, U16 usPort);
  S32 (*SL_TcpipSocketSend_p)(S32 slSocketId, U8* pucData, U16 usDatalenth);
  S32 (*SL_TcpipSocketRecv_p)(S32 slSocketId, U8* pucData, U16 usDatalenth);
  S32 (*SL_TcpipSocketSendTo_p)(S32 slSocketId, U8* pucData, U16 usDatalenth, U8* pucIpAddr, U16 usPort);
  S32 (*SL_TcpipSocketRecvFrom_p)(S32 slSocketId, U8* pucData, U16 usDatalenth, U8* pucIpAddr, U16 usPort);
  S32 (*SL_TcpipSocketBind_p)(S32 slSocketId);
  S32 (*SL_tcpip_socket_bind_p)(S32 slSocketId, U16 localPort);
  S32 (*SL_TcpipSocketListen_p)(S32 slSocketId);
  S32 (*SL_TcpipSocketAccept_p)(S32 slSocketId, U8* pucIpAddr, U16* pusPort);
  S32 (*SL_TcpipGetState_p)(S32 slSocketId);
  S32 (*SL_TcpipGetHostIpbyName_p)(U8 ucCid, U8* pucHostname, SL_TCPIP_GET_HOSTIP_BY_NAME cb);
  S32 (*SL_TcpipGetLocalIpAddr_p)(U8 ucCid, U8* pucIpAddr);
  S32 (*SL_TcpipGetDnsServerAddr_p)(U8 ucCid, U8* pucPrimaryAddr, U8* pucSecondAddr);
  S32 (*SL_TcpipSetDnsServerAddr_p)(U8 ucCid, BOOL bPrimarySet, U8* pucPrimayAddr, BOOL bSecondarySet, U8* pucSecondaryAddr);
  S32 (*SL_TcpipSocketCheckIp_p)(U8* pucIpAddrString, U32* pulIpAddr);
  U32 (*SL_TcpipSocketHtonl_p)(U32 n);
  U16 (*SL_TcpipSocketHtons_p)(U16 n);
  S32 (*SL_TcpipSetKeepAliveTime_p)(U32 ulTime, U32 ulEnableFlag);
  U8 (*SL_TcpipSetRetrTimes_p)(U8 ucRetrTimes);
  U8 (*SL_TcpipGetRetrTimes_p)(void);
  U8 (*SL_TcpipSetRetrTime_p)(U8 ucRetrTime);
  U8 (*SL_TcpipGetRetrTime_p)(void);
  U8 (*SL_TcpipGetCid_p)(void);
  S32 (*SL_TcpipOverWifiCfg_p)(S32 slSocketId);
  S32 (*SL_TcpipOverWifiClear_p)(S32 slSocketId);
  S32 (*SL_TcpipOverWifiStat_p)(S32 slSocketId, U8* pucStat);
  S32 (*SL_TcpipWifiLocalIpAddrCfg_p)(U8* pucIpAddr);
  S32 (*SL_TcpipGetWifiLocalIpAddr_p)(U8* pucIpAddr);
  BOOL (*SL_StartTimer_p)(SL_TASK hTask, U8 timeid, SL_TIMOPT timemode, U32 ticks);
  BOOL (*SL_StopTimer_p)(SL_TASK hTask, U8 timeid);
  U32 (*SL_SecondToTicks_p)(U32 ulSec);
  U32 (*SL_MilliSecondToTicks_p)(U32 ulMilliSec);
  BOOL (*SL_GetLocalTime_p)(SL_SYSTEMTIME* pSystemTime);
  BOOL (*SL_SetLocalTime_p)(SL_SYSTEMTIME* pSystemTime);
  U32 (*SL_TmGetTick_p)(void);
  BOOL (*SL_SetTimeZone_p)(S8 scTimeZone);
  S8 (*SL_GetTimeZone_p)(void);
  U32 (*SL_GetTime_p)(void);
  BOOL (*SL_FileTimeToSystemTime_p)(SL_FILETIME nFileTime, SL_SYSTEMTIME* pSystemTime);
  BOOL (*SL_UartOpen_p)(U8 ucUartId);
  void (*SL_UartClose_p)(U8 ucUartId);
  S32 (*SL_UartSendData_p)(SL_UART_ID enUartId, U8* pucBuf, U16 usLenth);
  S32 (*SL_UartSetBaudRate_p)(SL_UART_ID enUartId, SL_UART_BAUDRATE slRate);
  S32 (*SL_UartSetDCBConfig_p)(SL_UART_ID enUartId, 
                                SL_UART_DATABITS slDataBits, 
                                SL_UART_STOPBITS slStopBits, 
                                SL_UART_PARITY slParity);
  void (*SL_UartSetFlowCtrl_p)(U8 dte_AfcMode,U8 dce_AfcMode);
  U16 (*SL_UartGetTxRoomLeft_p)(SL_UART_ID enUartId);
  U32 (*SL_UartGetTxRestBytes_p)(SL_UART_ID enUartId);
  U16 (*SL_UartGetBytesAvail_p)(SL_UART_ID enUartId);
  void (*SL_UartClrRxBuffer_p)(SL_UART_ID enUartId);
  void (*SL_UartClrTxBuffer_p)(SL_UART_ID enUartId);
  void (*SL_UartSetAppTaskHandle_p)(SL_UART_ID enUartId, HANDLE taskHandle);
  HANDLE (*SL_UartGetAppTaskHandle_p)(SL_UART_ID enUartId);
  void (*SL_UartSetDlMode_p)(void);
  U8 (*SL_UartGetDlMode_p)(void);
  S32 (*SL_PbkInit_p)(SL_PBK_CALLBACK* cb);
  S32 (*SL_PbkGetOwnNumber_p)(void);
  S32 (*SL_PbkSetStorage_p)(SL_PBK_STORAGE_TYPE enStorageType);
  S32 (*SL_PbkReadEntry_p)(U8 ucStartIndex, U8 ucEndIndex);
  S32 (*SL_PbkWriteEntry_p)(U8 ucIndex, U8* pucTelNo, U8* pucName);
  S32 (*SL_PbkFindEntry_p)(U8* pucName);
  S32 (*SL_PbkDeleteEntry_p)(U8 ucIndex);
  void (*SL_PbkSetCharset_p)(SL_PBK_CHARSET enCharSet);
  SL_PBK_CHARSET (*SL_PbkGetCharSet_p)(void);
  SL_FTP_CALLBACK* (*SL_Ftp_GetCb_p)(void); 
  S32 (*SL_Ftp_Callback_Init_p)(SL_FTP_CALLBACK *cb);
  S32 (*SL_Ftp_Open_p)(SL_FTP_Connect_Info* pstConnectInfo);
  S32 (*SL_Ftp_Close_p)();
  S32 (*SL_Ftp_Download_p)(U8 *pu8RemoteFile, SL_FTP_File_TYPE emFileType);
  S32 (*SL_Ftp_Upload_p)(U8 *pu8FileName, U8 *pFileData, U32 u32FileLen, SL_FTP_File_TYPE emFileType);
  S32 (*SL_Ftp_PWD_p)(void);
  S32 (*SL_Ftp_CWD_p)(U8 *path);
  S32 (*SL_Ftp_CDUP_p)(void);
  S32 (*SL_Ftp_MKD_p)(U8 *dirName);
  S32 (*SL_Ftp_RMD_p)(U8 *dirName);
  S32 (*SL_Ftp_DEL_p)(U8 *fileName);
  S32 (*SL_Ftp_REN_p)(U8 *NewName, U8 *OldName);
  S32 (*SL_Ftp_LIST_p)(U8 *fileName);
  VOID (*SL_LbsInit_p)(VOID);
  S32 (*SL_LbsSendReq_p)(SL_LBS_REQ_INFO_STRUCT* pstLbsReqInfo);
  S32 (*SL_LbsRspInfoParse_p)(S8* scRcvInfo, SL_LBS_RSP_INFO_STRUCT* pstRspInfo);
  SL_HTTP_CALLBACK* (*SL_Http_GetCb_p)(void);
  S32 (*SL_HTTP_Callback_Init_p)(SL_HTTP_CALLBACK *cb);
  S32 (*SL_Http_Init_p)(void);
  S32 (*SL_Http_Set_Para_p)(char *pUrl, char *pUserName, char *pPassword, char *pUserAgent, SL_HTTP_RequestType ReqType);
  S32 (*SL_Http_Send_Data_p)(char *pData, SL_HTTP_DataType type);
};
extern struct FUNCPTR AA;
#endif
