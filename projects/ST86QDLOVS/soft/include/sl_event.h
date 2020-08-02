#ifndef _SL_EVENT_H_
#define _SL_EVENT_H_

#define SL_EV_TIMER                                 (50 + 43)

#define SL_EV_MMI_EV_BASE              0x01000000
#define SL_EV_MMI_EV_END               0xA0000000

#define SL_EV_UART_RECEIVE_DATA_IND    SL_EV_MMI_EV_END - 1 
#define SL_EV_UART1_WKUP_IRQ_IND	   SL_EV_MMI_EV_END - 2
#define SL_EV_UART2_WKUP_IRQ_IND	   SL_EV_MMI_EV_END - 3

#endif

