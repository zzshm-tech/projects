#ifndef _SL_APP_TCPIP_H_
#define _SL_APP_TCPIP_H_

/****************************************************
                                  macro define

****************************************************/
#define APP_TIMER_TCPIP_RESEND       3

/****************************************************
                                  variable

****************************************************/


/****************************************************
                                  function

****************************************************/
void SL_AppInitTcpip();
S32 SL_AppStartTcpip();
void SL_AppTcpipRsnd(S32 slSockId);

#endif

