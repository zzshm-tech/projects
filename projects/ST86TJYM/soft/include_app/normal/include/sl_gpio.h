/*********************************************************************************
 **                        Edit History                                          *
 ** -------------------------------------------------------------------------    *
 ** DATE           NAME             DESCRIPTION                                  *
 ** 2012/08/15     David 	        Create.                                      *
 *********************************************************************************/
#ifndef _SL_GPIO_H_
#define _SL_GPIO_H_
#include "sl_type.h"
/****************************************************
                                     macro define 
****************************************************/
typedef void (*SL_GPIO_IRQ_HANDLER)(void);
typedef enum  
{
    SL_GPIO_BEGIN = 0xff,
    SL_GPIO_0,  /* ST86_GPIO0->GPIO12. */
    SL_GPIO_1,  /* ST86_GPIO1->GPIO1. */
    SL_GPIO_2,  /* ST86_GPIO2->GPIO15. */
    SL_GPIO_3,  /* ST86_GPIO3->GPIO3. */
    SL_GPIO_4,  /* ST86_GPIO4->GPIO16. */
    SL_GPIO_5,  /* ST86_GPIO5->GPIO14. */
    SL_GPIO_6,  /* ST86_GPIO6->GPIO6. */
    SL_GPIO_7,  /* ST87_KEYIN_0_GPIO7->GPIO20, only input. */
    SL_GPIO_8,  /* ST87_KEYIN_4_GPIO8->GPIO21, only input. */
    SL_GPIO_9,  /* ST87_KEYIN_5_GPIO9->GPIO10, in/out. */
    SL_GPIO_10, /* ST87_KEYOUT_1_GPIO10->GPIO17, can not be used, reserved. */
    SL_GPIO_11, /* ST87_KEYOUT_2_GPIO11->GPIO11, can not be used, reserved. */
    SL_GPIO_12, /* ST86_STATUS->GPIO2, only output. */
    SL_GPIO_13, /* ST86_UART2_CTS->GPIO5. */
    SL_GPIO_14, /* ST86_UART2_RTS->GPIO7. */
    SL_GPIO_INVALID
}SL_GPIO_LIST;
typedef enum  
{
    SL_GPO_0,  /*!< available. */
    SL_GPO_1,  /*!< available. */	 
    SL_GPO_INVALID
}SL_GPO_LIST;
typedef enum  
{
    SL_GPIO_IN, /*!< an input pin */
    SL_GPIO_OUT /*!< an output pin */
}SL_GPIO_DIRECTION_TYPE;
typedef enum 
{
    SL_EDGE_RISING,  /*!< Rising edge on a GPIO pin will trigger interrupt */
    SL_EDGE_FALLING, /*!< Falling edge on a GPIO pin will trigger interrupt */
    SL_EDGE_BOTH,    /*!< Any change on a GPIO pin will trigger interrupt */
    //SL_NO_EDGE,		 /*!< Rising or falling edge on a GPIO pin will not trigger interrupt */
}SL_GPIO_INTR_MODE;
typedef enum 
{
    SL_PIN_LOW, /*!< Pull a GPIO pin to low or "0" */
    SL_PIN_HIGH /*!< Pull a GPIO pin to high or "1" */
}SL_GPIO_PIN_STATUS;
typedef struct
{
    /// Defines whether an interruption will be triggered on a rising
    /// edge on the GPIO
    BOOL rising;
    /// Defines whether an interruption will be triggered on a falling
    /// edge on the GPIO
    BOOL falling;
    /// Defines if the GPIO's signal will be debounced before the interrupt
    /// is triggered
    BOOL debounce;
    /// Defines if the interruption is on level (\c TRUE), or on edge (\c FALSE)
    BOOL level;
} SL_GPIO_IRQ_MASK;
#include "sl_app.h"
#define SL_GpioGetDir AA.SL_GpioGetDir_p
#define SL_GpioSetDir AA.SL_GpioSetDir_p
#define SL_GpioRead AA.SL_GpioRead_p
#define SL_GpioWrite AA.SL_GpioWrite_p
#define SL_GpoWrite AA.SL_GpoWrite_p
#define SL_GpioIntDetectInit AA.SL_GpioIntDetectInit_p
#define SL_GpioIntDetectClose AA.SL_GpioIntDetectClose_p
#endif
