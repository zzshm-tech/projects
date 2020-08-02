#ifndef __SL_WIFI_H__
#define __SL_WIFI_H__

#define SL_WIFI_AP_MAX_CNT		16
#define SL_IW_ESSID_MAX_SIZE	32
#define SL_ETH_ALEN				6

typedef void (*SL_WIFI_POWERON_CB)(S32 slResult, U8* ucpMacAddr, U32 ulLen);
typedef void (*SL_WIFI_POWEROFF_CB)(S32 slResult);
typedef void (*SL_WIFI_SCAN_CB)(U8* ucpDesc, U32 ulCnt);
typedef void (*SL_WIFI_CONNECTAP_CB)(S32 slResult, U32 ulErrNo);
typedef void (*SL_WIFI_DISCONNECTAP_CB)(void);
typedef void (*SL_WIFI_DHCP_CB)(S32 slResult, void* vpIpAddrInd);

typedef struct{
    SL_WIFI_POWERON_CB pstSlWifiPwrOn;
    SL_WIFI_POWEROFF_CB pstSlWifiPwrOff;
	SL_WIFI_SCAN_CB pstSlWifiScan;
	SL_WIFI_CONNECTAP_CB pstSlWifiConnectAp;
	SL_WIFI_DISCONNECTAP_CB pstSlWifiDisconnectAp;
	SL_WIFI_DHCP_CB pstSlWifiDhcp;
}SL_WIFI_CALLBACK;

typedef struct{
	U8 ssid[SL_IW_ESSID_MAX_SIZE + 1];
	U8 bss_type;
	U8 channel;
	U8 dot11i_info;
	U8 bssid[SL_ETH_ALEN];
	U8 rssi;
	U8 auth_info;
	U8 rsn_cap[2];
	U8 rsn_grp;
    U8 rsn_pairwise[3];
    U8 rsn_auth[3];
    U8 wpa_grp;
    U8 wpa_pairwise[3];
    U8 wpa_auth[3];
}SL_WIFI_BSS_DESC_t;

typedef struct
{
	U8 ref_count;
	U16 msg_len;  
	U8 cause;
	U8 ip_addr[4];
	U8 pri_dns_addr[4];
	U8 sec_dns_addr[4];
	U8 gateway[4];
	U8 netmask[4];
}SL_WIFI_IPADDR_IND_t;

typedef enum {
	SL_WIFI_PWRON_NORMAL,
	SL_WIFI_PWRON_TX_MACDATA,	
}SL_WIFI_PWRON_MODE_t;

SL_WIFI_CALLBACK* SL_WifiGetCb(void);
S32 SL_WifiInit(SL_WIFI_CALLBACK *CB);
S32 SL_WifiPowerOn(U8 *ucpMacAddr, SL_WIFI_PWRON_MODE_t mode);
S32 SL_WifiPowerOff(void);
S32 SL_WifiScan(void);
S32 SL_WifiConnectAp(SL_WIFI_BSS_DESC_t *pWifiBssDesc, U8 *ucpPasswd, U8 ucLen);
S32 SL_WifiDisconnectAp(void);
S32 SL_WifiDhcp(void);
S32 SL_WifiSendMacData(U32* pulMacData, U16 usLen, U8 ucType);
SL_WIFI_PWRON_MODE_t SL_WifiGetPwrOnMode(void);
S32 SL_WifiSetTxPower(U16 usDiv, U16 usBase);

#endif
