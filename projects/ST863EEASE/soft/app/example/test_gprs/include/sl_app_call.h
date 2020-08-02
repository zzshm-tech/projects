#ifndef _SL_APP_CALL_H_
#define _SL_APP_CALL_H_

/****************************************************
                                  variable

****************************************************/

#define APP_TIMER_CALL_RING       1
#define APP_TIMER_CALL_HANGUP     2

/****************************************************
                                  function

****************************************************/
void SL_AppInitCall();
void SL_AppAnswer(void);
void SL_AppHangUp();
#endif
