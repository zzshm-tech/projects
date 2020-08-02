#include "sl_type.h"
#include "sl_debug.h"
#include "sl_error.h"
#include "sl_dtmf.h"

/****************************************************
                                  function

****************************************************/

void SL_AppDtmfStartCb(U8* pucValue)
{
    SL_ApiPrint("SLAPP: SL_AppDtmfStartCb,key[%d]", *pucValue);
}

void SL_AppStartDtmf()
{
    SL_DtmfStart(SL_AppDtmfStartCb, SL_DTMF);
}

