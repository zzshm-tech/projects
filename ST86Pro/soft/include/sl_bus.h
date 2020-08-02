/*********************************************************************************
** File Name:     sl_bus.h                                                *
** Author:         Wang Ya Feng                                                  *
** DATE:           2013/01/09                                                   *
** Copyright:      2013 by SimpLight Nanoelectronics,Ltd. All Rights Reserved.  *
*********************************************************************************

*********************************************************************************
** Description:    Implement for common function of API layer                   *
** Note:           None                                                         *
*********************************************************************************

*********************************************************************************
**                        Edit History                                          *
** -------------------------------------------------------------------------    *
** DATE                   NAME             DESCRIPTION                                  *
** 2013/01/09     Wang Ya Feng       Create
*********************************************************************************/
#ifndef _SL_BUS_H_
#define _SL_BUS_H_

/****************************************************
                                    define

****************************************************/
/*I2C*/
// ============================================================================
// SL_I2C_BUS_ID_TYPE
// ----------------------------------------------------------------------------
/// That type describes the supported I2C bus IDs.
// ============================================================================
typedef enum
{
    SL_I2C_BUS_ID_1,
    SL_I2C_BUS_ID_2,
    SL_I2C_BUS_ID_3,
    SL_I2C_BUS_ID_QTY,
    SL_I2C_BUS_ID_INVALID = 0xff,
} SL_I2C_BUS_ID_TYPE;


/*SPI*/

// =============================================================================
// SL_SPI_BUS_ID_TYPE
// -----------------------------------------------------------------------------
/// Defines which SPI is used by a function of the SPI. Depending on the
/// hardware you are running on, some of those SPIs might not exist
// =============================================================================
typedef enum
{
    SL_SPI_BUS_ID_1,
    SL_SPI_BUS_ID_2,
#if (CHIP_SPI2_CS_QTY == 0)
    SL_SPI_BUS_ID_QTY = 1
#else
    SL_SPI_BUS_ID_QTY = 2
#endif
} SL_SPI_BUS_ID_TYPE;

// =============================================================================
// SL_SPI_CS_TYPE
// -----------------------------------------------------------------------------
/// Chip Select
/// Used to select a Chip Select
// =============================================================================
typedef enum
{
    SL_SPI_CS0 = 0,
    SL_SPI_CS1,
    SL_SPI_CS2,
    SL_SPI_CS3,
    SL_SPI_CS_QTY
 } SL_SPI_CS_TYPE;

// =============================================================================
// SL_SPI_DELAY_TYPE
// -----------------------------------------------------------------------------
/// Delays
/// Used to define the configuration delays
// =============================================================================
typedef enum
{
    /// Delay of 0 half-period
    SL_SPI_HALF_CLK_PERIOD_0, 
    /// Delay of 1 half-period
    SL_SPI_HALF_CLK_PERIOD_1, 
    /// Delay of 2 half-period
    SL_SPI_HALF_CLK_PERIOD_2, 
    /// Delay of 3 half-period
    SL_SPI_HALF_CLK_PERIOD_3, 

    SL_SPI_HALF_CLK_PERIOD_QTY
} SL_SPI_DELAY_TYPE;

// =============================================================================
// SL_SPI_RX_TRIGGER_CFG_TYPE
// -----------------------------------------------------------------------------
/// Reception  trigger (or treshold) level.
/// The SPI module can be setup to generate an interrupt when the reception FIFO 
/// is above a configurable threshold (Rx fifo trigger)
// =============================================================================
typedef enum
{
    /// 1 Data received in the Rx FIFO
    SL_SPI_RX_TRIGGER_1_BYTE, 
    /// 2 Data received in the Rx FIFO
    SL_SPI_RX_TRIGGER_4_BYTE, 
    /// 3 Data received in the Rx FIFO
    SL_SPI_RX_TRIGGER_8_BYTE, 
    /// 4 Data received in the Rx FIFO
    SL_SPI_RX_TRIGGER_12_BYTE,
    SL_SPI_RX_TRIGGER_QTY
} SL_SPI_RX_TRIGGER_CFG_TYPE;

// =============================================================================
// HAL_SPI_TX_TRIGGER_CFG_T
// -----------------------------------------------------------------------------
/// Tranmission FIFO trigger (or treshold) level. 
/// The SPI module can be setup to generate an interrupt when the emission FIFO 
/// is above a configurable threshold (Tx FIFO trigger)
// =============================================================================
typedef enum{
    /// 1 Data spot is empty in the Tx FIFO  
    SL_SPI_TX_TRIGGER_1_EMPTY, 
    /// 2 Data spots are empty in the Tx FIFO
    SL_SPI_TX_TRIGGER_4_EMPTY, 
    /// 8 Data spots are empty in the Tx FIFO
    SL_SPI_TX_TRIGGER_8_EMPTY, 
    /// 12 Data spots are empty in the Tx FIFO
    SL_SPI_TX_TRIGGER_12_EMPTY,

    SL_SPI_TX_TRIGGER_QTY
}SL_SPI_TX_TRIGGER_CFG_TYPE;

// =============================================================================
// SL_SPI_TRANSFERT_MODE_TYPE
// -----------------------------------------------------------------------------
/// Data transfert mode: via DMA or direct. 
/// To allow for an easy use of the SPI modules, a non blocking Hardware 
/// Abstraction Layer interface is provided. Each transfer direction 
/// (send/receive) can be configured as:
// =============================================================================
typedef enum {
    /// Direct polling: The application sends/receives the data directly to/from 
    /// the hardware module. The number of bytes actually sent/received is 
    /// returned. No Irq is generated.
    SL_SPI_DIRECT_POLLING = 0,

    /// Direct IRQ: The application sends/receives the data directly to/from 
    /// the hardware module. The number of bytes actually sent/received is 
    /// returned. An Irq can be generated when the Tx/Rx FIFO reaches the 
    /// pre-programmed level.
    SL_SPI_DIRECT_IRQ, 

    /// DMA polling: The application sends/receives the data through a DMA to 
    /// the hardware module. The function returns 0 when no DMA channel is 
    /// available. No bytes are sent. The function returns the number of bytes 
    /// to send when a DMA resource is available. They will all be sent. A 
    /// function allows to check if the previous DMA transfer is finished. No 
    /// new DMA transfer in the same direction will be allowed before the end 
    /// of the previous transfer.
    SL_SPI_DMA_POLLING, 

    /// DMA IRQ: The application sends/receives the data through a DMA to the 
    /// hardware module. The function returns 0 when no DMA channel is
    /// available. No bytes are sent. The function returns the number of bytes 
    /// to send when a DMA resource is available. They will all be sent. An 
    /// IRQ is generated when the current transfer is finished. No new DMA 
    /// transfer in the same direction will be allowed before the end of the 
    /// previous transfer.
    SL_SPI_DMA_IRQ, 
    
    /// The SPI is off
    SL_SPI_OFF, 
    
    SL_SPI_TM_QTY
} SL_SPI_TRANSFERT_MODE_TYPE;

// =============================================================================
// SL_SPI_IRQ_STATUS_TYPE
// -----------------------------------------------------------------------------
/// This structure is used to represent the IRQ status and mask 
/// of the SPI module.
// =============================================================================
typedef struct
{
    /// receive FIFO overflow irq
    U32 rxOvf:1;
    /// transmit FIFO threshold irq
    U32 txTh:1;
    /// transmit Dma Done irq
    U32 txDmaDone:1;
    /// receive FIFO threshold irq
    U32 rxTh:1;
    /// receive Dma Done irq
    U32 rxDmaDone:1;
} SL_SPI_IRQ_STATUS_TYPE;

// =============================================================================
// SL_SPI_IRQ_HANDLER_TYPE
// -----------------------------------------------------------------------------
/// Type of the user IRQ handler
// =============================================================================
typedef VOID (*SL_SPI_IRQ_HANDLER_TYPE)(SL_SPI_IRQ_STATUS_TYPE);

// =============================================================================
// SL_SPI_CFG_TYPE
// -----------------------------------------------------------------------------
/// Structure for configuration. 
/// A configuration structure allows to open or change the SPI with the desired 
/// parameters.
// =============================================================================
typedef struct
{
    /// Select the Chip Select
    SL_SPI_CS_TYPE enabledCS; 
    /// Polarity of this CS
    BOOL csActiveLow;
    /// When \c TRUE, the emission commands will fill the Rx FIFO with read
    /// data, thus enabling the ability to receive data. \n
    /// When \c FALSE, nothing is written in the Rx FIFO when data are sent.
    /// It is not possible to read received data, which are discarded.
    BOOL inputEn;
    /// If the first edge after the CS activation is a falling edge, set to 
    /// \c TRUE.\n Otherwise, set to \c FALSE.
    BOOL clkFallEdge;
    /// The delay between the CS activation and the first clock edge,
    /// can be 0 to 2 half clocks.
    SL_SPI_DELAY_TYPE clkDelay; 
    /// The delay between the CS activation and the output of the data, 
    /// can be 0 to 2 half clocks.
    SL_SPI_DELAY_TYPE doDelay; 
    /// The delay between the CS activation and the sampling of the input data,
    /// can be 0 to 3 half clocks.
    SL_SPI_DELAY_TYPE diDelay; 
    /// The delay between the end of transfer and the CS deactivation, can be 
    /// 0 to 3 half clocks.
    SL_SPI_DELAY_TYPE csDelay;
    /// The time when the CS must remain deactivated before a new transfer, 
    /// can be 0 to 3 half clocks.
    SL_SPI_DELAY_TYPE csPulse; 
    /// Frame size in bits
    U32 frameSize;
    /// OE ratio - Value from 0 to 31 is the number of data out to transfert 
    /// before the SPI_DO pin switches to input. When 0m the SPI_DO pin switching 
    /// direction mode is not enabled.
    U8 oeRatio;
    /// SPI maximum clock frequency: the SPI clock will be the highest
    /// possible value inferior to this parameter.
    U32 spiFreq;
    /// Value for the reception FIFO above which an interrupt may be generated.
    SL_SPI_RX_TRIGGER_CFG_TYPE rxTrigger; 

    /// Value for the emission FIFO above which an interrupt may be generated.
    SL_SPI_RX_TRIGGER_CFG_TYPE txTrigger; 

    /// Reception transfer mode
    SL_SPI_TRANSFERT_MODE_TYPE rxMode;

    /// Emission transfer mode
    SL_SPI_TRANSFERT_MODE_TYPE txMode;

    /// IRQ mask for this CS
    SL_SPI_IRQ_STATUS_TYPE mask;

    /// IRQ handler for this CS;
    SL_SPI_IRQ_HANDLER_TYPE handler;

} SL_SPI_CFG_TYPE;

// =============================================================================
// SL_SPI_PIN_TYPE
// -----------------------------------------------------------------------------
/// SPI Pin
///
/// Used to specify an SPI pin.
// =============================================================================
typedef enum
{
    /// Chip select 0 pin
    SL_SPI_PIN_CS0 = 0, 
    /// Chip select 1 pin
    SL_SPI_PIN_CS1,  
    /// Chip select 2 pin, if available
    SL_SPI_PIN_CS2, 
    /// Chip select 3 pin, if available
    SL_SPI_PIN_CS3, 
    /// Clock pin
    SL_SPI_PIN_CLK, 
    /// Data out pin
    SL_SPI_PIN_DO, 
    /// Data in pin
    SL_SPI_PIN_DI, 
    SL_SPI_PIN_LIMIT 
} SL_SPI_PIN_TYPE;

// =============================================================================
// SL_SPI_PIN_STATE_TYPE
// -----------------------------------------------------------------------------
/// SPI Pin State
///
/// Used to specify the state of an SPI pin. It can be
/// zero, one or Z (high impedence, tri-state).
// =============================================================================
typedef enum
{
    SL_SPI_PIN_STATE_SPI   =   0,
    SL_SPI_PIN_STATE_Z     =   1,
    SL_SPI_PIN_STATE_0     =   2,
    SL_SPI_PIN_STATE_1     =   3
} SL_SPI_PIN_STATE_TYPE;

// =============================================================================
// HAL_SPI_PATTERN_MODE_T
// -----------------------------------------------------------------------------
/// The pattern mode can be used in two: record data after the pattern has been
/// received on the SPI, or data are not recorded while the pattern is seens on
/// the bus. HAL_SPI_PATTERN_WHILE feature is not available for every chip.
// =============================================================================
typedef enum
{
    /// Standard mode, we don't care about the pattern
    SL_SPI_PATTERN_NO,
    /// No data written until the pattern is read on the bus
    SL_SPI_PATTERN_UNTIL,
    /// No data are written while the pattern is read on the bus
    SL_SPI_PATTERN_WHILE
} SL_SPI_PATTERN_MODE_T;

// =============================================================================
// HAL_SPI_INFINITE_TX_T
// -----------------------------------------------------------------------------
/// That type describes the characteristics and configuration of an infinite 
/// transfer on a SPI CS. That is typically used for devices like MMC which use
/// the pattern mode read feature. (The infinite transfer provides the clock
/// coping with the trashing reads before the pattern is finally seen).
/// When the pattern mode is enabled, data received on the SPI bus will only
/// be put in the Rx FIFO after a given pattern has been received.
/// The SPI will continue transfering data (zeros or ones, depending on the
/// configuration of sendOne) after the Tx fifo is empty (i.e. the IFC DMA
/// is finished) and until the Rx transfer is completed (i.e. the pattern
/// has been received and the Rx transfer size is reached).
/// After you start the inifinite write mode, you have to use the usual
/// send and get data functions.
/// The infinite transfert can be stopped by two ways:
/// - manually, by a call to #hal_SpiStopInfiniteWrite;
/// - automatically (prefered behaviour): the infinite transfer is stopped
/// by the RxDmaDone IRQ generated by the end of the reception done parallelly,
/// if the Rx mode is configured as #HAL_SPI_DMA_POLLING or #HAL_SPI_DMA_IRQ.
// =============================================================================

typedef struct
{
    /// \c TRUE if the infinite transfer is done by sending ones,
    /// \c FALSE if it is done by sending zeroes.
    BOOL sendOne;

    /// \c TRUE enable the auto-stopping feature of the infinite transfer.
    BOOL autoStop;

    /// Described the pattern mode. That is the read buffer 
    /// will only be filled with data received after a given pattern, 
    /// specified by the following field, is seen on the line,
    /// or after the pattern is no more read on it (while the pattern
    /// is seen, no data is recorded).
    SL_SPI_PATTERN_MODE_T patternMode;

    /// Value of the pattern waited before useful data are received.
    U32 pattern;
} SL_SPI_INFINITE_TX;

/****************************************************
                                    function

****************************************************/
/*I2C*/
S32 SL_I2cOpen(U8 ucI2cBusid);
S32 SL_I2cClose(U8 ucI2cBusid);
S32 SL_I2cWriteByte(U8 ucI2cBusid, U32 ulSlaveAddr, U32 ulMemAddr, U8 data);
S32 SL_I2cReadByte(U8 ucI2cBusid, U32 ulSlaveAddr, U32 ulMemAddr, U8* pData);
S32 SL_I2cSendData(U8 ucI2cBusid, U32 ulSlaveAddr, U32 ulMemAddr, CONST U8* pData, U32 ulLength);
S32 SL_I2cReadData(U8 ucI2cBusid, U32 ulSlaveAddr, U32 ulMemAddr, U8* pData, U32 ulLength);

/*SPI*/
S32 SL_SpiOpen(U8 ucSpiBusid, U8 ucCsNo, CONST SL_SPI_CFG_TYPE* pstSpiCfg);
S32 SL_SpiClose(U8 ucSpiBusid, U8 ucCsNo);
S32 SL_SpiWriteData(U8 ucSpiBusid, U8 ucCsNo, CONST U8* pStartAddress, U32 ulLength);
S32 SL_SpiReadData(U8 ucSpiBusid, U8 ucCsNo, U8* pDestAddress, U32 ulLength);
BOOL SL_SpiActCs(U8 ucSpiBusid, U8 ucCsNo);
BOOL SL_SpiDeactCs(U8 ucSpiBusid, U8 ucCsNo);
S32 SL_SpiSetPinState(U8 ucSpiBusid, U8 ucPin, U8 ucPinState);
BOOL SL_SpiTxDmaDone(U8 ucSpiBusid, U8 ucCsNo);
BOOL SL_SpiRxDmaDone(U8 ucSpiBusid, U8 ucCsNo);
VOID SL_SpiClearTxDmaDone(U8 ucSpiBusid);
VOID SL_SpiClearRxDmaDone(U8 ucSpiBusid);
BOOL SL_SpiTxFinished(U8 ucSpiBusid, U8 ucCsNo);
VOID SL_SpiFlushFifo(U8 ucSpiBusid);
VOID SL_SpiTxIfcChannelRelease(U8 ucSpiBusid);
VOID SL_SpiRxIfcChannelRelease(U8 ucSpiBusid);
VOID SL_SpiStartInfiniteWriteMode(U8 ucSpiBusid, U8 ucCsNo, SL_SPI_INFINITE_TX* penSpiInifiTx);
VOID SL_SpiStopInfiniteWriteMode(U8 ucSpiBusid, U8 ucCsNo);

#endif
