#ifndef _SL_APP_SMS_H_
#define _SL_APP_SMS_H_

/****************************************************
                                  variable

****************************************************/


/****************************************************
                                  function

****************************************************/
void SL_AppInitSms();
void SL_AppSmsReadCb(S32 slResult, U8* pucbuf, U16 usLenth);
#endif

