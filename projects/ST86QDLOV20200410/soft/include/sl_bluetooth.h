#ifndef __SL_BLUETOOTH_H__
#define __SL_BLUETOOTH_H__

#define BT_PIN_LEN         (4)
#define BT_ADDR_STRING_LEN (17)
#define BT_MAX_NAME_LEN    (59)

#define SIZE_OF_BDADDR 6
typedef struct {
	U8 bytes[SIZE_OF_BDADDR];
} BT_ADDR;

typedef enum
{
    BT_POWEROFF,
    BT_READY,
    BT_PAIRED,
    BT_CONNECTED
} BT_STATE;

typedef void (*SL_BT_POWERON_CB)(S32 slResult);
typedef void (*SL_BT_POWEROFF_CB)(S32 slResult);
typedef void (*SL_BT_SCAN_CB)(S32 slResult,u8* pAddr);
typedef void (*SL_BT_SCAN_END_CB)(S32 slResult);
typedef void (*SL_BT_PAIR_CB)(S32 slResult);
typedef void (*SL_BT_BEPAIR_CB)(S32 slResult, U8* pAddr);
typedef void (*SL_BT_CONNECT_CB)(S32 slResult);
typedef void (*SL_BT_BECONNECT_CB)(S32 slResult, U8* pAddr);
typedef void (*SL_BT_DISCONNECT_CB)(S32 slResult);
typedef void (*SL_BT_SENDDATA_CB)(S32 slResult);
typedef void (*SL_BT_RCVDATA_CB)(S32 slResult,U8* dataBuf,U16* dataLen);

typedef struct{
    SL_BT_POWERON_CB pstSlBtPwrOn;
    SL_BT_POWEROFF_CB pstSlBtPwrOff;
	SL_BT_SCAN_CB pstSlBtScan;
	SL_BT_SCAN_END_CB pstSlBtScanEnd;
	SL_BT_PAIR_CB pstSlBtPair;
	SL_BT_BEPAIR_CB pstSlBtBePair;
	SL_BT_CONNECT_CB pstSlBtConnect;
	SL_BT_BECONNECT_CB pstSlBtBeConnect;
	SL_BT_DISCONNECT_CB pstSlBtDisconnect;
	SL_BT_SENDDATA_CB pstSlBtSenddata;
	SL_BT_RCVDATA_CB pstSlBtRcvdata;
}SL_BT_CALLBACK;

/*****************************************************************************
* Name:            SL_BtGetCb
* Description:    Get BT callback
* Parameter:     NULL
* Return:           SL_BT_CALLBACK*
*                 
******************************************************************************/
SL_BT_CALLBACK* SL_BtGetCb(void);

/*****************************************************************************
* Name:            SL_BtInit
* Description:    BT init 
* Parameter:     SL_BT_CALLBACK
* Return:           0:   OK
*                       -1: Fail
******************************************************************************/
S32 SL_BtInit(SL_BT_CALLBACK *CB);

/*****************************************************************************
* Name:            SL_BtPowerOn
* Description:    BT start work 
* Parameter:     NULL
* Return:           0:   OK
*                       -1: Fail
******************************************************************************/
S32 SL_BtPowerOn(void);

/*****************************************************************************
* Name:            SL_BtPowerOff
* Description:    BT stop work 
* Parameter:     NULL
* Return:           0:   OK
*                       -1: Fail
******************************************************************************/
S32 SL_BtPowerOff(void);

/*****************************************************************************
* Name:            SL_BtScan
* Description:    BT scan other device 
* Parameter:     
*                       devCount   scan device num
*                       timeout      scan tineout
* Return:           0:   OK
*                       -1: Fail
******************************************************************************/
S32 SL_BtScan(U8 devCount,U16 timeOut);

/*****************************************************************************
* Name:            SL_BtPair
* Description:    Two BT device pair 
* Parameter:     
*                       pAddr        device address buf
*                       addrLen     address length
*                       timeout      pair tineout
* Return:           0:   OK
*                       -1: Fail
******************************************************************************/
S32 SL_BtPair(U8* pAddr,U8 addrLen,U16 timeout);

/*****************************************************************************
* Name:            SL_BtConnect
* Description:    Two BT device connect 
* Parameter:     
*                       pAddr        device address buf
*                       addrLen     address length
*                       timeout      connect tineout
* Return:           0:   OK
*                       -1: Fail
******************************************************************************/
S32 SL_BtConnect(U8* pAddr,U8 addrLen,U16 timeout);

/*****************************************************************************
* Name:            SL_BtDisconnect
* Description:    Two BT device disconnect 
* Parameter:     NULL
* Return:           0:   OK
*                       -1: Fail
******************************************************************************/
S32 SL_BtDisconnect(void);

/*****************************************************************************
* Name:            SL_BtSendData
* Description:    Two BT device send data 
* Parameter:     
*                       pData        data buf
*                       addrLen     data length
* Return:           0:   OK
*                       -1: Fail
******************************************************************************/
S32 SL_BtSendData(U8* pData, U16 dataLen);

/*****************************************************************************
* Name:            SL_BtQueryState
* Description:    Query BT state
* Parameter:     NULL
* Return:           
*                      BT_POWEROFF
*                      BT_READY
*                      BT_PAIRED
*                      BT_CONNECTED                   
******************************************************************************/
U32 SL_BtQueryState(void);

/*****************************************************************************
* Name:            SL_BtQueryPin
* Description:    Query BT pin code
* Parameter:     
*                       pinCode     pinCode buf
*                       pinLen        pinCode length
* Return:           0:   OK
*                       -1: Fail
******************************************************************************/
S32 SL_BtQueryPin(U8* pinCode,U8* pinLen);

/*****************************************************************************
* Name:            SL_BtSetPin
* Description:    Set BT pin code
* Parameter:     
*                       pinCode     pinCode buf
*                       pinLen        pinCode length
* Return:           0:   OK
*                       -1: Fail
******************************************************************************/
S32 SL_BtSetPin(U8* pinCode,U8 pinLen);

/*****************************************************************************
* Name:            SL_BtQueryName
* Description:    Query BT name
* Parameter:     
*                       pName       pName buf
*                       nameLen    name length
* Return:           0:   OK
*                       -1: Fail
******************************************************************************/
S32 SL_BtQueryName(U8* pName,U8* nameLen);

/*****************************************************************************
* Name:            SL_BtSetName
* Description:    Set BT name
* Parameter:     
*                       pName       pName buf
*                       nameLen    name length
* Return:           0:   OK
*                       -1: Fail
******************************************************************************/
S32 SL_BtSetName(U8* pName,U8 nameLen);

/*****************************************************************************
* Name:            SL_BtQueryLaddr
* Description:    Query BT local address
* Parameter:     
*                       pAddr        device address buf
*                       addrLen     address length
* Return:           0:   OK
*                       -1: Fail
******************************************************************************/
S32 SL_BtQueryLaddr(U8* pAddr,U8* addrLen);

/*****************************************************************************
* Name:            SL_BtSetLaddr
* Description:    Set BT local address
* Parameter:     
*                       pAddr        device address buf
*                       addrLen     address length
* Return:           0:   OK
*                       -1: Fail
******************************************************************************/
S32 SL_BtSetLaddr(U8* pAddr,U8 addrLen);

#endif
