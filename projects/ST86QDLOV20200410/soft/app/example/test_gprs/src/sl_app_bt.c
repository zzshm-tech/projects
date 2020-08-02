#include "sl_type.h"
#include "sl_debug.h"
#include "sl_error.h"
#include "sl_bluetooth.h"

#if SL_BT_APP==1

#define  BT_ROLE  1

static   U8 g_Index = 0;
BT_ADDR  g_BtAddr[5][BT_ADDR_STRING_LEN+1] = {0};

/****************************************************
                                  function

****************************************************/
void SL_AppBtPwrOnCb(S32 slResult)
{
	SL_ApiPrint("SLAPP[BT]: SL_AppBtPwrOnCb result[%d]", slResult);

	if(BT_ROLE)
	{
	  U8 Buf[100]={0};
	  U8 Len = 0;
	  SL_BtSetPin("0000",4);
	  SL_BtQueryPin(Buf,&Len);
	  SL_ApiPrint("SLAPP[BT]: SL_BtQueryPin[%s]", Buf);
	  SL_Memset(Buf,0,100);
	  SL_BtSetName("AK47",4);
	  SL_BtQueryName(Buf,&Len);
	  SL_ApiPrint("SLAPP[BT]: SL_BtQueryName[%s]", Buf);
	  SL_Memset(Buf,0,100);
	  SL_BtQueryLaddr(Buf,&Len);
	  SL_ApiPrint("SLAPP[BT]: SL_BtQueryLaddr[%s]", Buf);

	  SL_BtScan(1,30);
	}
}

void SL_AppBtPwrOffCb(S32 slResult)
{
	SL_ApiPrint("SLAPP[BT]: SL_AppBtPwrOffCb result[%d]", slResult);
}

void SL_AppBtScanCb(S32 slResult,u8* pAddr)
{
    U8 addrBuf[100]={0};
	if(pAddr != NULL)
	  SL_Memcpy(addrBuf,pAddr,17);
	SL_ApiPrint("SLAPP[BT]: SL_AppBtScanCb result[%d],ADDR=%s", slResult,addrBuf);

    SL_Memcpy(g_BtAddr[g_Index],pAddr,BT_ADDR_STRING_LEN);
	g_Index++;
}

void SL_AppBtScanEndCb(S32 slResult)
{
	SL_ApiPrint("SLAPP[BT]: SL_AppBtScanEndCb result[%d]", slResult);
	SL_BtPair(g_BtAddr[0],BT_ADDR_STRING_LEN,10);
}

void SL_AppBtPairCb(S32 slResult)
{
	SL_ApiPrint("SLAPP[BT]: SL_AppBtPairCb result[%d]", slResult);

	if(slResult != SL_RET_OK)
	{
	  SL_Sleep(3000);
	  SL_BtPair(g_BtAddr[0],BT_ADDR_STRING_LEN,10);
	  return;
	}

    SL_Sleep(3000);
	SL_BtConnect(g_BtAddr[0],BT_ADDR_STRING_LEN,10);
}

void SL_AppBtBePairCb(S32 slResult)
{
	SL_ApiPrint("SLAPP[BT]: SL_AppBtBePairCb result[%d]", slResult);
}

void SL_AppBtConnectCb(S32 slResult)
{
	SL_ApiPrint("SLAPP[BT]: SL_AppBtConnectCb result[%d]", slResult);

	if(slResult != SL_RET_OK)
	{
	  SL_Sleep(3000);
	  SL_BtConnect(g_BtAddr[0],BT_ADDR_STRING_LEN,10);
	  return;
	}

	U32 state = 0;
	state = SL_BtQueryState();
	SL_ApiPrint("SLAPP[BT]: SL_BtQueryState[%d]", state);

	SL_BtSendData("1234567890", 10);
}

void SL_AppBtBeConnectCb(S32 slResult)
{
	SL_ApiPrint("SLAPP[BT]: SL_AppBtBeConnectCb result[%d]", slResult);
}


void SL_AppBtDisConnectCb(S32 slResult)
{
	SL_ApiPrint("SLAPP[BT]: SL_AppBtDisConnectCb result[%d]", slResult);
}

void SL_AppBtSendDataCb(S32 slResult)
{
	SL_ApiPrint("SLAPP[BT]: SL_AppBtSendDataCb result[%d]", slResult);
}

void SL_AppBtRcvDataCb(S32 slResult,U8* dataBuf, U8* dataLen)
{
	U8 bt_dataBuf[255]={0};
	if(dataBuf != NULL)
	  SL_Memcpy(bt_dataBuf,dataBuf,*dataLen);

	SL_Sleep(1000);
	SL_BtSendData("abcdefghjk", 10);

	SL_ApiPrint("SLAPP[BT]: SL_AppBtRcvDataCb dataBuf=%s,Len=%d",bt_dataBuf,*dataLen);
}

void SL_AppBtInit(void)
{
    SL_BT_CALLBACK  bt_callback;
    bt_callback.pstSlBtPwrOn = SL_AppBtPwrOnCb;
	bt_callback.pstSlBtPwrOff= SL_AppBtPwrOffCb;
	bt_callback.pstSlBtScan= SL_AppBtScanCb;
	bt_callback.pstSlBtScanEnd= SL_AppBtScanEndCb;
	bt_callback.pstSlBtPair= SL_AppBtPairCb;
	bt_callback.pstSlBtBePair= SL_AppBtBePairCb;
	bt_callback.pstSlBtConnect= SL_AppBtConnectCb;
	bt_callback.pstSlBtBeConnect= SL_AppBtBeConnectCb;
	bt_callback.pstSlBtDisconnect= SL_AppBtDisConnectCb;
	bt_callback.pstSlBtSenddata= SL_AppBtSendDataCb;
	bt_callback.pstSlBtRcvdata= SL_AppBtRcvDataCb;
    SL_BtInit(&bt_callback);
	SL_AppBtTest();
}

void SL_AppBtTest(void)
{
    SL_BtPowerOn();
}

#endif


