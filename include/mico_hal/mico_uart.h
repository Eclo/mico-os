/**
 ******************************************************************************
 * @file    MicoDriverUart.h
 * @author  William Xu
 * @version V1.0.0
 * @date    16-Sep-2014
 * @brief   This file provides all the headers of UART operation functions.
 ******************************************************************************
 *
 *  The MIT License
 *  Copyright (c) 2014 MXCHIP Inc.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is furnished
 *  to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ******************************************************************************
 */


#ifndef __MICODRIVERUART_H__
#define __MICODRIVERUART_H__


#include "mico_common.h"
#include "RingBufferUtils.h"
#ifdef ALIOS_SUPPORT
#include "aos_peripheral.h"
#else
#include "platform_peripheral.h"
#endif

/* Legacy definitions */
#define MicoUartInitialize          mico_uart_init
#define MicoUartFinalize            mico_uart_deinit
#define MicoUartSend                mico_uart_send
#define MicoUartRecv                mico_uart_recv
#define MicoUartGetLengthInBuffer   mico_uart_recvd_data_len
#define MicoStdioUartInitialize     mico_stdio_uart_init

/** @addtogroup MICO_PLATFORM
* @{
*/

/** @defgroup MICO_UART MICO UART Driver
* @brief  Universal Asynchronous Receiver Transmitter (UART) Functions
* @{
*/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

typedef int8_t mico_uart_t;  /**< MiCO UART peripheral handle, MICO_UART_XX define by board/<board_name>/mico_board.h. */

typedef platform_uart_config_t mico_uart_config_t;

/******************************************************
 *                 Function Declarations
 ******************************************************/



/**@brief Initialises a UART interface
 *
 * @note Prepares an UART hardware interface for communications
 *
 * @param  uart     : the interface which should be initialised
 * @param  config   : UART configuration structure
 * @param  optional_rx_buffer : Pointer to an optional RX ring buffer
 *
 * @return    kNoErr        : on success.
 * @return    kGeneralErr   : if an error occurred with any step
 */
OSStatus mico_uart_init( mico_uart_t uart, const mico_uart_config_t* config, ring_buffer_t* optional_rx_buffer );


/**@brief Initialises STDIO UART interface
 *
 * @note Make sure you want oto use STDIO UART other than printf
 *
 * @param  config   : UART configuration structure
 * @param  optional_rx_buffer : Pointer to an optional RX ring buffer
 *
 * @return    kNoErr        : on success.
 * @return    kGeneralErr   : if an error occurred with any step
 */
OSStatus mico_stdio_uart_init( const mico_uart_config_t* config, ring_buffer_t* optional_rx_buffer );



/**@brief Initialises a STDIO UART interface, internal use only
 *
 * @note Prepares an UART hardware interface for stdio communications
 *
 * @param  config   : UART configuration structure
 * @param  optional_rx_buffer : Pointer to an optional RX ring buffer
 *
 * @return    kNoErr        : on success.
 * @return    kGeneralErr   : if an error occurred with any step
 */
OSStatus mico_stdio_uart_init( const mico_uart_config_t* config, ring_buffer_t* optional_rx_buffer );


/**@brief Deinitialises a UART interface
 *
 * @param  uart : the interface which should be deinitialised
 *
 * @return    kNoErr        : on success.
 * @return    kGeneralErr   : if an error occurred with any step
 */
OSStatus mico_uart_deinit( mico_uart_t uart );


/**@brief Transmit data on a UART interface
 *
 * @param  uart     : the UART interface
 * @param  data     : pointer to the start of data
 * @param  size     : number of bytes to transmit
 *
 * @return    kNoErr        : on success.
 * @return    kGeneralErr   : if an error occurred with any step
 */
OSStatus mico_uart_send( mico_uart_t uart, const void* data, uint32_t size );


/**@brief Receive data on a UART interface
 *
 * @param  uart     : the UART interface
 * @param  data     : pointer to the buffer which will store incoming data
 * @param  size     : number of bytes to receive
 * @param  timeout  : timeout in millisecond
 *
 * @return    kNoErr        : on success.
 * @return    kGeneralErr   : if an error occurred with any step
 */
OSStatus mico_uart_recv( mico_uart_t uart, void* data, uint32_t size, uint32_t timeout );

/**@brief Read the length of the data that is already recived by uart driver and stored in buffer
 *
 * @param uart     : the UART interface
 *
 * @return    Data length
 */
uint32_t mico_uart_recvd_data_len( mico_uart_t uart );

/** @} */
/** @} */

#endif
