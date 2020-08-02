#include "sl_http.h"
#include "sl_gprs.h"
#include "sl_debug.h"
#include "sl_error.h"
#include "sl_lbs.h"

void SL_LBS_HttpSendDataCb(void)
{
    SL_Print("---> HTTP Send Data Done\n");
}
void SL_LBS_HttpPostData(void)
{
	SL_LBS_REQ_INFO_STRUCT stReqInfo;

    SL_Print("---> SL LBS Send data ...\n");
	SL_LbsInit();

	SL_Memset(&stReqInfo, 0, sizeof(SL_LBS_REQ_INFO_STRUCT));
	stReqInfo.blLongLatValidFlag = FALSE;
	stReqInfo.enMapType = 0x0f;
	
	SL_LbsSendReq(&stReqInfo);
}

void SL_LBS_HttpGprsAttCb(U8 ucState, S32 slResult)
{
    SL_ApiPrint("SLAPP: SL_AppGprsAttCb ucState[%d], result[%d]", ucState, slResult);

    if((ucState == SL_GPRS_ATTACHED) && (slResult == 0))
    {		
        SL_LBS_HttpPostData();
    }

    if(ucState == SL_GPRS_DETACHED)
    {
        SL_Print("SLAPP: SL_AppGprsAttCb dettach, ucState[%d]", ucState);
    }
}

void SL_LBS_HttpRecvDataCb(char *pktPtr)
{
	S32 slRet = SL_RET_OK;
	SL_LBS_RSP_INFO_STRUCT stRspInfo;

    SL_Print("---> SL LBS recv: %s\n", pktPtr);
	SL_Memset(&stRspInfo, 0, sizeof(SL_LBS_RSP_INFO_STRUCT));
	slRet = SL_LbsRspInfoParse(pktPtr, &stRspInfo);

	SL_Print("SLAPP: SL_LbsRspInfoParse ret [%d]", slRet);
	if(slRet == SL_RET_OK)
	{
        SL_Print("SLAPP: longi[%d], lati[%d], longiG[%d], latiG[%d], longiB[%d], latiB[%d],"
			"longiQ[%d], latiQ[%d], radius[%d]", 
			stRspInfo.ulLongi, stRspInfo.ulLati, stRspInfo.ulLongiGoogle, stRspInfo.ulLatiGoogle, 
			stRspInfo.ulLongiBaidu, stRspInfo.ulLatiBaidu, stRspInfo.ulLongiGaode, stRspInfo.ulLatiGaode,
			stRspInfo.ulRadius);
	}
}

void SL_LBS_HttpErrHandlerCb(void)
{
    SL_Print("---> [ERROR] SL lbs Err\n");
}

void SL_AppLbsTest(void)
{
    SL_HTTP_CALLBACK http_cb;
    S32 slRet = 0;

    http_cb.pstSlHttpSendDataSucc = SL_LBS_HttpSendDataCb;
    http_cb.pstSlHttpRecvDataSucc = SL_LBS_HttpRecvDataCb;
    http_cb.pstSlHttpErrHandle = SL_LBS_HttpErrHandlerCb;
    SL_HTTP_Callback_Init(&http_cb);
    SL_Http_Init();
	
    slRet = SL_GprsAtt(SL_GPRS_ATTACHED, SL_LBS_HttpGprsAttCb);
    if(slRet != SL_RET_OK)
    {
        SL_ApiPrint("SLAPP: SL_AppGprsAttach attach fail[%d]", slRet);
    }
}

