#ifndef _SL_APP_WIFI_H_
#define _SL_APP_WIFI_H_


/****************************************************
                                  function

****************************************************/
void wifi_power_on(void);
void wifi_power_off(void);
void wifi_scan(void);

    
/********************************************************************************
//说明: wifi上电回调函省｝
//参数1: 上电结果。 
//参数2: MAC地址。
//参数3: 长度。
********************************************************************************/
void wifi_power_on_call_back(S32 slResult, U8* ucpMacAddr, U32 ulLen);

void wifi_power_off_call_back(S32 slResult);
void wifi_scan_call_back(U8* ucpDesc, U32 ulCnt);
void wifi_connect_call_back(S32 slResult, U32 ulErrNo);
void wifi_disconnect_call_back(void);
void wifi_dhcp_call_back(S32 slResult, void* vpIpAddrInd);

void wifi_init(void);

#endif
