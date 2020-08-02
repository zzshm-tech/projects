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
void SL_Reset(void);
/*!
 * \fn SL_Sleep(UINT32 msec)
 * \brief This function suspends the calling task for \b msec millisecond.
 * \param[in] msec millisecond to sleep
 * \return None
 */
void SL_Sleep(U32 ulMsecond);
void SL_PowerDown(U8 ucPowerType);
void SL_PowerOn(void);
BOOL SL_StartWatchDog(U32 ulTime);
void SL_FeedWatchDog(void);
void SL_StopWatchDog(void);
S32 SL_GetCoreVer(U8* pucVer, U16 usLenth);
//S32 SL_GetSDKVer(U8* pucVer, U16 usLenth);
BOOL SL_CoreInitFinish(void);
void SL_GetPowerOnReason(U8* penCause);
void SL_GetDeviceCurrentRunState(S32* plSimCard, S32* plCreg, S32* plCgreg, S8* pucRssi, U8* pucBer);
BOOL SL_IsSIMInserted(void);
S32 SL_GSM_GetIMEI(U8* pucImei, U16 usLenth);
S32 SL_GSM_GetGMR(U8* pucGmr, U16 usLenth);
U8 SL_GSM_GetCALIB();
S32 SL_GSM_GetSN(U8* pucImei, U16 usLenth);
S32 SL_SIM_GetIMSI(U8* pucImsi, U16 usLenth);
S32 SL_GSM_GetPSN(U8* pucPsn, U16 usLenth);
S32 SL_GSM_GetSIGN(U8* pucSign, U16 usLenth);
S32 SL_GSM_WriteCfun(U8 ucCfun, U16 usLenth);
VOID SL_GSM_WriteTest(U8 ucTest,U8* testinfo);
S32 SL_GSM_WriteSign(U8* pucSign, U16 usLenth);
S32 SL_GSM_WriteIMEI(U8* pucImei, U16 usLenth);
S32 SL_GSM_WriteSN(U8* pucImei, U16 usLenth);
S32 SL_GetOperator(U8* pucOperate, U8 ucLenth);
S32 SL_RetrieveNodeBInfo(SL_NW_NODEB_INFO* pstSlNodeBInfo);
S32 SL_GetNwStatus(void);
void SL_PmuEnablePower(SL_PMD_POWER_ID_TYPE enPmdId, BOOL on);
U16 SL_GetAdcValue(SL_ADC_CHAN enChan);
SL_APP_UPDATE_STATUS SL_AppUpdateInit(U8 *buf, U32 len);
S32 SL_SimPinInit(SL_SIM_CALLBACK* cb);
S32 SL_SimEnterPwd(U8* pucPwd);
S32 SL_SimGetAuthInfo(void);
S32 SL_SimChangePwd(U8 ucFac, U8* pucOldPwd, U8* pucNewPwd);
S32 SL_SimLock(U8 ucFac, U8 ucMode, U8* pucPwd);
S32 SL_SimLockStatusQuery(U8 ucFac);
S32 SL_SimSetPwd(U8* pucPukPwd, U8* pucPinPwd);
S32 SL_SimDropCbInit(SL_SIM_CB_DROP cb);
S32 SL_SimGetCCID(SL_SIM_CB_GET_CCID cb);
S32 SL_RetrieveCellInfo(SL_NW_CELL_TYPE enCellType, SL_NW_CB_CELL_INFO cb);
VOID SL_PwlSetLevel(SL_PWL_ID enSlPwlId, U8 ucLevel);
VOID SL_SetFlightMode(SL_FLIGHT_MODE_CB cb);
VOID SL_ExitFlightMode(SL_EXIT_FLIGHT_MODE_CB cb);
VOID SL_InitBatteryCb(SL_BATTERY_CB cb);
VOID SL_InitPwrOnCb(SL_PWRON_CB cb);
VOID SL_InitPwrOffCb(SL_PWROFF_CB cb);
void SL_FintIrqSetMask(BOOL mask);
BOOL SL_GetRfTemp(U8* pucTemp);

#endif
