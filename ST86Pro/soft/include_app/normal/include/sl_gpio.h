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
    SL_GPIO_0,  /*!< available. */
    SL_GPIO_1, 	/*!< available. */
    SL_GPIO_2,  /*!< available. */
    SL_GPIO_3, 	/*!< available. */
    SL_GPIO_4,  /*!< available. */
    SL_GPIO_5,  /*!< available. */
    SL_GPIO_6, 	/*!< available. */
    SL_GPIO_7,  /*!< reference to different product. */
    SL_GPIO_8,  /*!< reference to different product. */
    SL_GPIO_9,  /*!< reference to different product. */
    SL_GPIO_10, /*!< reference to different product. */
    SL_GPIO_11, /*!< reference to different product. */
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
