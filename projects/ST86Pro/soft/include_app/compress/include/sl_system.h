/*********************************************************************************
 ** File Name:      sl_system.h                                                *
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
#ifndef _SL_SYSTEM_H_
#define _SL_SYSTEM_H_
#include "sl_type.h"
/************************************************
                           define
************************************************/
typedef void (*SL_SIM_CB_ENT_PINPWD)(S32 slResult);
typedef void (*SL_SIM_CB_CHG_PINPWD)(S32 slResult);
typedef void (*SL_SIM_CB_SET_PINPWD)(S32 slResult);
typedef void (*SL_SIM_CB_CLCK)(S32 slResult);
typedef void (*SL_SIM_CB_CPIN_QUERY)(S32 slResult, U8 ucPinStatus);
typedef void (*SL_SIM_CB_CLCK_QUERY)(S32 slResult, U8 ucLckStatus);
typedef void (*SL_SIM_CB_DROP)(void);
typedef void (*SL_SIM_CB_GET_CCID)(S32 slResult, U8* pucCcid, U8 ucCcidLenth);
typedef void (*SL_FLIGHT_MODE_CB)(S32 slResult);
typedef void (*SL_EXIT_FLIGHT_MODE_CB)(S32 slResult);
typedef void (*SL_PWROFF_CB)(void);
typedef struct
{
    SL_SIM_CB_ENT_PINPWD slSimEnterPinPwdCb;
    SL_SIM_CB_CHG_PINPWD slSimChangePinPwdCb;
    SL_SIM_CB_SET_PINPWD slSimSetPinPwdCb;
    SL_SIM_CB_CLCK slSimClckCb;
    SL_SIM_CB_CPIN_QUERY slSimCpinQueryCb;
    SL_SIM_CB_CLCK_QUERY slSimClckQueryCb;
    SL_SIM_CB_DROP slSimDropCb;
    SL_SIM_CB_GET_CCID slSimGetCcidCb;
}SL_SIM_CALLBACK;
typedef struct
{
    SL_FLIGHT_MODE_CB slFlightModeCb;
    SL_EXIT_FLIGHT_MODE_CB slExitFlightModeCb;
}SL_FLIGHT_CALLBACK;
typedef struct 
{
    U32 ulSlMCC;   //mobile country code
    U32 ulSlMNC;   //mobile network code
    U32 ulSlLAC;   //Location Area Identification.
    U32 ulSlCI;    //Cell Identity.
    U8 iBsic;      //Synchronisation code,[0..63].
    S8 iRxLev;     //Average Rx level  Val of dBm ,[-115..0].
    U16 usFreq;    //Absolute Radio Frequency Channel Number ,[0..1023].
    U8 ucTimeAdv;  //Timing Advance (valid in GSM connected mode only).[0..255] 255 = non significant.
}SL_NW_NODEB_INFO;
typedef struct
{
    U8 ucCellNum;
    SL_NW_NODEB_INFO stSlCellInfo[6];
}SL_NW_CELL_INFO;
typedef void (*SL_NW_CB_CELL_INFO)(S32 slResult, SL_NW_CELL_INFO* pstSlCellInfo);
typedef enum
{
    SL_CHARGER_DISCONNECTED = 0,
    SL_CHARGER_CONNECTED = 1,
    SL_CHARGER_CHRGING = 2,
    SL_CHARGER_FINISHED = 3,
    SL_CHARGER_ERROR_TEMPERATURE = 4,
    SL_CHARGER_ERROR_VOLTAGE = 5,
    SL_CHARGER_ERROR_UNKNOWN = 9
}SL_CHARGER_STATUS;
typedef enum
{
    SL_BATTERY_STATE_NORMAL = 0,
    SL_BATTERY_STATE_LOW = 1,
    SL_BATTERY_STATE_CRITICAL = 2,
    SL_BATTERY_STATE_SHUTDOWN = 3,
    SL_PM_BATTERY_STATE_UNKNOWN = 4
}SL_BATTERY_POWER_STATE;
typedef struct
{
    U32 ulBatVol;      // voltage of battery 
    U32 ulBatPwrPercent;    //percentage of full power of battery 
    SL_CHARGER_STATUS enSlChgState;  // charge state of battery 
    SL_BATTERY_POWER_STATE enSlBatState; // state of battery
}SL_BATTERY_INFO;
typedef void (*SL_BATTERY_CB)(SL_BATTERY_INFO* pstSlBatInfo);
/************************************************
                           enum 
************************************************/
typedef enum
{
    SL_POWRN_ON_CAUSE_KEY =        0x01,
    SL_POWRN_ON_CAUSE_CHARGE =     0x02,
    SL_POWRN_ON_CAUSE_ALARM =      0x04,
    SL_POWRN_ON_CAUSE_EXCEPTION =  0x08,
    SL_POWRN_ON_CAUSE_RESET =      0x10,
    SL_POWRN_ON_CAUSE_UNKOWN =     0x80
}SL_POWERON_CAUSE;
typedef void (*SL_PWRON_CB)(SL_POWERON_CAUSE enCause);
typedef enum
{
    SL_SIM_ABSENT = 0x00,
    SL_SIM_NORMAL = 0x01,
    SL_SIM_TEST = 0x02,
    SL_SIM_ABNORMAL = 0x03,
    SL_SIM_STATUS_END = 0x04
}SL_SIM_STATUS;
typedef enum
{
    SL_SIM_SET_UNLOCK,
    SL_SIM_SET_LOCK
}SL_SIM_LOCK_SET;
typedef enum
{
    SL_SIM_STATUS_UNLOCK,
    SL_SIM_STATUS_LOCK,
    SL_SIM_STATUS_INVALID
}SL_SIM_LOCK_STATUS;
typedef enum
{
    SL_SIM_AUTH_READY,
    SL_SIM_AUTH_PIN1_READY,
    SL_SIM_AUTH_SIMPIN,
    SL_SIM_AUTH_SIMPUK,
    SL_SIM_AUTH_NOSIM = 17,
    SL_SIM_AUTH_PIN1BLOCK,
    SL_SIM_AUTH_PIN2BLOCK,
    SL_SIM_AUTH_PIN1_DISABLE,
    SL_SIM_AUTH_SIM_PRESENT,
    SL_SIM_AUTH_SIM_PHASE,
    SL_SIM_AUTH_END
}SL_SIM_AUTH_TYPE;
typedef enum
{
    SL_NW_STATUS_NOTREGISTERED_NOTSEARCHING =  0,// Not registered and not searching. 
    SL_NW_STATUS_REGISTERED_HOME =             1,//Registered and in the home area. 
    SL_NW_STATUS_NOTREGISTERED_SEARCHING =     2,//Not registered and searching for a new operator. 
    SL_NW_STATUS_REGISTRATION_DENIED =         3,//Registration denied. 
    SL_NW_STATUS_UNKNOWN =                     4,//Unknown registration. 
    SL_NW_STATUS_REGISTERED_ROAMING =          5
}SL_NW_REG_STATUS;
typedef enum
{
    SL_NW_CELL_SERVING = 1,
    SL_NW_CELL_NEIGHBOR,
    SL_NW_CELL_INVALID
}SL_NW_CELL_TYPE;
//=============================================================================
// SL_PMD_POWER_ID_TYPE
// -----------------------------------------------------------------------------
/// PMIC power ID
/// ID used by #pmd_EnablePower to enable or disable a power.
// =============================================================================
typedef enum {
    /// Micro bias enable
    SL_PMD_POWER_MIC,
    /// Camera LDO enable
    SL_POWER_CAMERA,
    /// Audio LDO enable
    SL_POWER_AUDIO,
    /// Stereo DAC LDO enable
    SL_POWER_STEREO_DAC,
    /// Loud Speaker enable
    SL_POWER_LOUD_SPEAKER,
    /// RF Power Amplifier
    SL_POWER_PA,
    /// USB LDOs enable
    SL_POWER_USB,
    /// SD/MMC LDO enable
    SL_POWER_SDMMC,
    /// FM LDO enable
    SL_POWER_FM,
    /// Ear Piece Micro bias enable
    SL_POWER_EARPIECE,
    /// BlueTooth LDOs enable
    SL_POWER_BT,
    /// Camera flash Light enable
    SL_POWER_CAMERA_FLASH,
    /// (main) LCD LDO enable
    SL_POWER_LCD,
    /// I2C LDO enable
    SL_POWER_I2C,
    // must be in sync with pmd_map.xmd PMD_MAP_POWER_ID_QTY
    SL_POWER_ID_QTY
} SL_PMD_POWER_ID_TYPE;
typedef enum
{
	SL_ADC_CHAN_0,
	SL_ADC_CHAN_1,
	SL_ADC_CHAN_2,
	SL_ADC_CHAN_3,
	SL_ADC_CHAN_7 = 7,
    SL_ADC_CHAN_QTY
} SL_ADC_CHAN;
typedef enum
{
	SL_APP_UPDATE_OK,
	SL_APP_UPDATE_ILLEGAL,
	SL_APP_UPDATE_BINLEN_OVERFLOW,
	SL_APP_UPDATE_BUFLEN_UNDERFLOW,
	SL_APP_UPDATE_CRC_ERR,
} SL_APP_UPDATE_STATUS;
typedef enum 
{
    SL_PWL_0,
    SL_PWL_1,
    SL_PWL_QTY
}SL_PWL_ID;
/*************************************************
                                  function
*************************************************/
#include "sl_app.h"
#define SL_Reset AA.SL_Reset_p
#define SL_Sleep AA.SL_Sleep_p
#define SL_PowerDown AA.SL_PowerDown_p
#define SL_PowerOn AA.SL_PowerOn_p
#define SL_StartWatchDog AA.SL_StartWatchDog_p
#define SL_FeedWatchDog AA.SL_FeedWatchDog_p
#define SL_StopWatchDog AA.SL_StopWatchDog_p
#define SL_GetCoreVer AA.SL_GetCoreVer_p
//S32 SL_GetSDKVer(U8* pucVer, U16 usLenth);
#define SL_CoreInitFinish AA.SL_CoreInitFinish_p
#define SL_GetPowerOnReason AA.SL_GetPowerOnReason_p
#define SL_GetDeviceCurrentRunState AA.SL_GetDeviceCurrentRunState_p
#define SL_IsSIMInserted AA.SL_IsSIMInserted_p
#define SL_GSM_GetIMEI AA.SL_GSM_GetIMEI_p
#define SL_GSM_GetGMR AA.SL_GSM_GetGMR_p
#define SL_GSM_GetCALIB AA.SL_GSM_GetCALIB_p
#define SL_GSM_GetSN AA.SL_GSM_GetSN_p
#define SL_SIM_GetIMSI AA.SL_SIM_GetIMSI_p
#define SL_GSM_GetPSN AA.SL_GSM_GetPSN_p
#define SL_GSM_GetSIGN AA.SL_GSM_GetSIGN_p
#define SL_GSM_WriteCfun AA.SL_GSM_WriteCfun_p
#define SL_GSM_WriteTest AA.SL_GSM_WriteTest_p
#define SL_GSM_WriteSign AA.SL_GSM_WriteSign_p
#define SL_GSM_WriteIMEI AA.SL_GSM_WriteIMEI_p
#define SL_GSM_WriteSN AA.SL_GSM_WriteSN_p
#define SL_GetOperator AA.SL_GetOperator_p
#define SL_RetrieveNodeBInfo AA.SL_RetrieveNodeBInfo_p
#define SL_GetNwStatus AA.SL_GetNwStatus_p
#define SL_PmuEnablePower AA.SL_PmuEnablePower_p
#define SL_GetAdcValue AA.SL_GetAdcValue_p
#define SL_AppUpdateInit AA.SL_AppUpdateInit_p
#define SL_SimPinInit AA.SL_SimPinInit_p
#define SL_SimEnterPwd AA.SL_SimEnterPwd_p
#define SL_SimGetAuthInfo AA.SL_SimGetAuthInfo_p
#define SL_SimChangePwd AA.SL_SimChangePwd_p
#define SL_SimLock AA.SL_SimLock_p
#define SL_SimLockStatusQuery AA.SL_SimLockStatusQuery_p
#define SL_SimSetPwd AA.SL_SimSetPwd_p
#define SL_SimDropCbInit AA.SL_SimDropCbInit_p
#define SL_SimGetCCID AA.SL_SimGetCCID_p
#define SL_RetrieveCellInfo AA.SL_RetrieveCellInfo_p
#define SL_PwlSetLevel AA.SL_PwlSetLevel_p
#define SL_SetFlightMode AA.SL_SetFlightMode_p
#define SL_ExitFlightMode AA.SL_ExitFlightMode_p
#define SL_InitBatteryCb AA.SL_InitBatteryCb_p
#define SL_InitPwrOnCb AA.SL_InitPwrOnCb_p
#define SL_InitPwrOffCb AA.SL_InitPwrOffCb_p
#define SL_FintIrqSetMask AA.SL_FintIrqSetMask_p
#define SL_GetRfTemp AA.SL_GetRfTemp_p
#endif
