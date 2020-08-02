/*!
 *	\file sl_gpio.h
 *	\brief A set of gpio (general purpose input output) APIs in SL1360 platform
 *
 * 	\details The file defines gpio pins in SL1360 SoC chip and APIs to control the gpio pins.
 *      The gpio are named following the GPIO pin definitions in the chip. Only part of
 *      the gpio pins are available, please refer to SL_GPIO_PIN_LIST for details.
 *	\n
 * 	<h3>GPIO Operations</h3>
 * 	Only the following gpios can be used in user mode: \n
 *	SL_GPIO0_0 .. SL_GPIO0_3, \n SL_GPIO0_11 .. SL_GPIO0_15, \n
 *	SL_GPIO1_0,\n SL_GPIO1_3,\n SL_GPIO1_4,\n SL_GPIO1_7. \n
 *      All other GPIOs are reserved and should not be used by user.
 *	Next, user can use function SL_GpioSetDir() to set GPIO direction according to requirements.
 *	Then set value by SL_GpioSetOutput() or get value by SL_GpioGetInput() for one GPIO port.
 *	And SL1360 support interrupt mode for GPIO if you enable it by SL_GpioEnableIntr(), but you 
 *	need set interrupt mode by SL_GpioSetIntrMode() and register ISR by SL_ExRegisterGpioISR() 
 *	before enable the interrupt.
 *
 *	\version 		0.1
 *	\author 		SimpLight
 *	\date 			2012-08-15
 * 	\copyright
 *  Copyright (C) 2006-2011 by SimpLight Nanoelectronics, Ltd.\n
 *                 All rights reserved.\n
 * This material constitutes the trade secrets and confidential,\n
 * proprietary information of SimpLight.  This material is not to\n
 * be disclosed, reproduced, copied, or used in any manner  not\n
 * permitted under license from SimpLight Nanoelectronics Ltd.\n
 *
 *	\example ex_gpio.c
 */

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

/*! \enum SL_GPIO_LIST
 *  \brief List of 7 gpio pins.
 */
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

/*! \enum SL_GPO_LIST
 *  \brief List of 2 gpo pins.
 */
typedef enum  
{
    SL_GPO_0,  /*!< available. */
    SL_GPO_1,  /*!< available. */	 
    SL_GPO_INVALID
}SL_GPO_LIST;

/*! \enum SL_GPIO_PIN_TYPE
 *  \brief Define gpio pin types: input or output
 */
typedef enum  
{
    SL_GPIO_IN, /*!< an input pin */
    SL_GPIO_OUT /*!< an output pin */
}SL_GPIO_DIRECTION_TYPE;

/*! \enum SL_GPIO_INTR_MODE
 *  \brief Define interrupt trigger mode.

 * Interrupt will be generated when one of below signal changes occurs on a GPIO pin:
 * falling edge, rising edge or both.
 */
typedef enum 
{
    SL_EDGE_RISING,  /*!< Rising edge on a GPIO pin will trigger interrupt */
    SL_EDGE_FALLING, /*!< Falling edge on a GPIO pin will trigger interrupt */
    SL_EDGE_BOTH,    /*!< Any change on a GPIO pin will trigger interrupt */
    //SL_NO_EDGE,		 /*!< Rising or falling edge on a GPIO pin will not trigger interrupt */
}SL_GPIO_INTR_MODE;

/*! \enum SL_GPIO_PIN_STATUS
 *  \brief Define gpio output pin status
 */
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

/*!
 * \fn UINT32 SL_GpioGetDir(UINT32 gpiox)
 * \brief Get pin type of a specific GPIO pin
 * \param[in] gpiox gpio pin defined in the gpio list
 * \retval SL_DIRECTION_IN		input pin
 * \retval SL_DIRECTION_OUT	output pin
 */
U32 SL_GpioGetDir(U32 gpiox);

/*!
 * \fn void SL_GpioSetDir(UINT32 gpiox, UINT32 gpio_dir)
 * \brief Set pin type of a GPIO pin
 * \param[in] gpiox		the GPIO pin to be configured
 * \param[in] gpio_dir	gpio pin type
 * \return None
 * \attention only part of GPIO pins can be accessed by users.
 */
void SL_GpioSetDir(U32 gpiox, U32 gpio_dir);

/*!
 * \fn UINT32 SL_GpioGetInput(UINT32 gpiox)
 * \brief Get input value of an input GPIO pin
 * \param[in] gpiox		the input GPIO pin
 * \retval SL_PIN_LOW 	Pull a GPIO pin to low
 * \retval SL_PIN_HIGH	Pull a GPIO pin to high
 */
U32 SL_GpioRead(U32 gpiox);

/*!
 * \fn void SL_GpioSetOutput(UINT32 gpiox, UINT32 pin_val)
 * \brief Set output value of an output GPIO pin
 * \param[in] gpiox		the output GPIO pin
 * \param[in] pin_val	SL_PIN_LOW or SL_PIN_HIGH
 * \return None
 */
void SL_GpioWrite(U32 gpiox, U32 pin_val);
void SL_GpoWrite(U32 gpo, U32 pin_val);
void SL_GpioIntDetectInit(U32 gpiox, SL_GPIO_IRQ_MASK* pstIrqMask, SL_GPIO_IRQ_HANDLER Handler);
void SL_GpioIntDetectClose(U32 gpiox);

#endif
