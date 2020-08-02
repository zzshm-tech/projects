#include "sl_type.h"
#include "sl_debug.h"
#include "sl_error.h"
#include "sl_stdlib.h"
#include "sl_pbk.h"
#include "sl_uart.h"

/****************************************************
                                  variable

****************************************************/

/****************************************************
                                  function

****************************************************/

void SL_AppPbkCnum(S32 slResult, U8* pucName, U8* pucTelNo, U8 ucTelType)
{
    SL_ApiPrint("SLAPP: SL_AppPbkCnum slResult[%d], name[%s], telno[%s], type[%d]", slResult, pucName, pucTelNo, ucTelType);
}

void SL_AppPbkCpbf(S32 slResult, U8 ucIndex, U8* pucTelNo, U8 ucTelType, U8* pucName)
{
    SL_ApiPrint("SLAPP: SL_AppPbkCpbf slResult[%d], name[%s], telno[%s], type[%d], index[%d]", 
        slResult, pucName, pucTelNo, ucTelType, ucIndex);
}

void SL_AppPbkCpbr(S32 slResult, U8 ucIndex, U8* pucTelNo, U8 ucTelType, U8* pucName)
{
    SL_ApiPrint("SLAPP: SL_AppPbkCpbr slResult[%d], name[%s], telno[%s], type[%d], index[%d]", 
        slResult, pucName, pucTelNo, ucTelType, ucIndex);

    SL_UartSendData(SL_UART_1, pucTelNo, SL_Strlen(pucTelNo));
    SL_UartSendData(SL_UART_1, pucName, SL_Strlen(pucName));
    SL_UartSendData(SL_UART_1, "\r\n", SL_Strlen("\r\n"));
}

void SL_AppPbkCpbs(S32 slResult)
{
    SL_ApiPrint("SL: SL_AppPbkCpbs slResult[%d]", slResult);
}

void SL_AppPbkCpbw(S32 slResult)
{
    SL_ApiPrint("SL: SL_AppPbkCpbw slResult[%d]", slResult);
}

void SL_AppPbkCpbd(S32 slResult)
{
    SL_ApiPrint("SL: SL_AppPbkCpbd slResult[%d]", slResult);
}

void SL_AppPbkInit(void)
{
    SL_PBK_CALLBACK cb;

    SL_Memset(&cb, 0, sizeof(SL_PBK_CALLBACK));

    cb.pslCnumCb = SL_AppPbkCnum;
    cb.pslCpbfCb = SL_AppPbkCpbf;
    cb.pslCpbrCb = SL_AppPbkCpbr;
    cb.pslCpbsCb = SL_AppPbkCpbs;
    cb.pslCpbwCb = SL_AppPbkCpbw;
    cb.pslCpbdCb = SL_AppPbkCpbd;
    SL_PbkInit(&cb);
}

void SL_AppPbkReadEntry(void)
{
    SL_PbkReadEntry(1,10);
}

void SL_AppPbkWriteEntry(void)
{
    SL_PbkWriteEntry(1, "+13810109423", "wyf");
    SL_ApiPrint("SLAPP: pbk add 1");
    SL_PbkWriteEntry(2, "18210072331", "liupeng");
    SL_ApiPrint("SLAPP: pbk add 2");
    SL_PbkWriteEntry(3, "18210072330", "zhengwei");
    SL_ApiPrint("SLAPP: pbk add 3");
    SL_PbkWriteEntry(4, "13261132972", "Õı—«∑Ê");
}

void SL_AppPbkFindEntry(U8* pucName)
{
    SL_PbkFindEntry(pucName);
}

void SL_AppPbkDelEntry(void)
{
    SL_PbkDeleteEntry(3);
}

void SL_AppPbkSetStorage(void)
{
    SL_PbkSetStorage(SL_PBK_SM);
}

