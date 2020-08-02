#include "sl_type.h"
#include "sl_debug.h"
#include "sl_error.h"
#include "sl_stdlib.h"
#include "sl_uart.h"
#include "sl_sms.h"
#include "sl_app_event.h"

/****************************************************
                                  variable

****************************************************/
static char test_data_sms_txt[] = {"welcome to S80!"};//{"简约纳"};
static char test_data_sms_pdu[] = {"0011000f8100688112002733F1000801044F60597D"};

/****************************************************
                                  function

****************************************************/

void SL_AppSmsListCb(S32 slResult, U8 ucIndex, U8* pucbuf, U16 usLenth)
{
    SL_ApiPrint("SLAPP: SL_AppSmsListCb Index[%d], result[%d]!", ucIndex, slResult);
    SL_UartSendData(SL_UART_1, pucbuf, usLenth);
}

void SL_AppSmsReadCb(S32 slResult, U8* pucbuf, U16 usLenth)
{
    SL_ApiPrint("SLAPP: SL_AppSmsReadCb result[%d]!", slResult);
    
    if(slResult >= 0)
    {
        SL_ApiPrint("SLAPP: sms read OK, data[%s]", pucbuf);
        SL_UartSendData(SL_UART_1, pucbuf, usLenth);
    }
    else
    {
        SL_ApiPrint("SLAPP: sms read fail, ret[%d]", slResult);
    }

    SL_SmsReadSmsList(SL_SMS_LIST_ALL, SL_AppSmsListCb);
}

void SL_AppSmsUpdateTelNo(U8* pucTelNumber, U8 ucLenth, U8* pucPDUSendData)//输入电话号码为ascii字串
{
    U8 aucCallNo[20] = {0};
    U8 aucTelNoBCD[20] = {0};
    U8 ucIndex = 0;
    
    SL_ApiPrint("SLAPP: SL_AppSmsUpdateTelNo pucTelNumber[%s], ucLenth[%d]!", pucTelNumber, ucLenth);

    SL_Memcpy(aucCallNo, pucTelNumber + 1, ucLenth - 1);
    if((ucLenth - 1) % 2)  //除去"+"之后的电话号码长度
    {
        SL_Strcat(aucCallNo, "F");
    }
    for(U8 i = 0; i <  SL_Strlen(aucCallNo); i++)
    {
        if(i % 2)
        {
            aucTelNoBCD[i] = aucCallNo[i - 1];
        }
        else
        {
            aucTelNoBCD[i] = aucCallNo[i + 1];
        }
    }
    SL_ApiPrint("SLAPP: SL_AppSmsUpdateTelNo out TelNumber[%s]", aucTelNoBCD);

    ucIndex = (pucPDUSendData[0] - '0') * 10 + (pucPDUSendData[1] - '0') + 1 * 2 + 4 * 2;
    SL_Memcpy(pucPDUSendData + ucIndex, aucTelNoBCD, SL_Strlen(aucTelNoBCD));
}


void SL_AppSmsRcvCb(S32 slResult, U8 ucIndex, U8* pucbuf, U16 usLenth, U8* pucTelNumber)
{
    SL_ApiPrint("SLAPP: SL_AppSmsRcvCb index[%d], result[%d], telnum[%s]!", ucIndex, slResult, pucTelNumber);
    if(slResult != SL_RET_OK)
    {
        return;
    }
    SL_ApiPrint("SLAPP: SL_AppSmsRcvCb [%s]", pucbuf);
    SL_UartSendData(SL_UART_1, pucbuf, usLenth);
    if(SL_SmsGetSmsFormat())
    {
        SL_SmsSendTextSms(pucTelNumber, test_data_sms_txt);       
    }
    else
    {
        SL_AppSmsUpdateTelNo(pucTelNumber, SL_Strlen(pucTelNumber), test_data_sms_pdu);
        SL_ApiPrint("SLAPP: test_data_sms_pdu [%s]!", test_data_sms_pdu);
        SL_SmsSendPDUSms(SL_Strlen(test_data_sms_pdu) / 2 - 1, test_data_sms_pdu);
    }
    
    SL_SmsDeleteSms(3, SL_SMS_DEL_INDEX);
    SL_SmsReadSms(ucIndex, SL_AppSmsReadCb);
}

void SL_AppSmsFullCb(S32 slResult)
{
    SL_ApiPrint("SLAPP: SL_AppSmsFullCb result[%d].", slResult);
    SL_SmsDeleteSms(1, SL_SMS_DEL_ALL);
}

void SL_AppSmsSendCb(S32 slResult)
{
    SL_ApiPrint("SLAPP: SL_AppSmsSendCb result[%d].", slResult);
}

void SL_AppInitSms()
{
    SL_SmsSetSmsFormat(SL_SMS_FMT_PDU);
    SL_SmsSetStorage(SL_SMS_STORAGE_SIM, SL_SMS_STORAGE_SIM, SL_SMS_STORAGE_SIM);
    //SL_SmsSetTxtModeDataCodSch(8);
    SL_SmsSetNewSmsCallback(SL_AppSmsRcvCb);
    SL_SmsSetSmsFullCallback(SL_AppSmsFullCb);
    SL_SmsSetSendSmsCallback(SL_AppSmsSendCb);
    SL_ApiPrint("SLAPP: SL_AppSmsInit OK!");
}

void SL_AppSmsTest()
{
    U8* pucBuff = NULL;
    U16 lenth = 0, len1 = 0;
#if 0
    lenth = SL_Strlen(test_data_sms_pdu);
    len1 = sizeof(test_data_sms_pdu);
    SL_ApiPrint("SLAPP: SL_AppSmsTest lenth[%d, %d]", lenth, len1);
    pucBuff = SL_GetMemory(len1);
    SL_Memset(pucBuff, 0, len1);
    SL_Memcpy(pucBuff, test_data_sms_pdu, len1);
    SL_UartSendData(SL_UART_1, pucBuff, len1);
    SL_ApiPrint("SLAPP: SL_AppSmsTest[%s]", pucBuff);
    SL_SmsSendPDUSms(lenth / 2 - 1, pucBuff);
#else
    //SL_SmsDeleteSms(4, SL_SMS_DEL_INDEX);
    //SL_SmsReadSms(4, SL_AppSmsReadCb);
    lenth = SL_Strlen(test_data_sms_pdu);
    SL_SmsSendPDUSms(lenth / 2 - 1, test_data_sms_pdu);

    //SL_SmsSendTextSms("13810109423", test_data_sms_txt);
#endif
}
