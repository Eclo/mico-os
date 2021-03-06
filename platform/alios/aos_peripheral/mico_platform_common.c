/* MiCO Team
 * Copyright (c) 2017 MXCHIP Information Tech. Co.,Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include "mico_debug.h"
#include "mico_common.h"

#include "mico_platform.h"


//#ifndef ALIOS_HAS_MICO_PLATFORM
/******************************************************
*                      Macros
******************************************************/

/******************************************************
*                    Constants
******************************************************/

/******************************************************
*                   Enumerations
******************************************************/

/******************************************************
*                 Type Definitions
******************************************************/

/******************************************************
*                    Structures
******************************************************/

/******************************************************
*               Static Function Declarations
******************************************************/

/******************************************************
*               Variable Definitions
******************************************************/

/* Externed from platforms/<Platform>/platform.c */
#ifdef ALIOS_DEV_GPIO
extern const platform_gpio_t            platform_gpio_pins[];
platform_gpio_driver_t                  platform_gpio_drivers[MICO_GPIO_MAX];
#endif

#ifdef ALIOS_DEV_ADC
extern const platform_adc_t             platform_adc_peripherals[];
#endif

#ifdef ALIOS_DEV_I2C
extern const platform_i2c_t             platform_i2c_peripherals[];
platform_i2c_driver_t                   platform_i2c_drivers[MICO_I2C_MAX];
#endif

#ifdef ALIOS_DEV_PWM
extern const platform_pwm_t             platform_pwm_peripherals[];
platform_pwm_driver_t                   platform_pwm_drivers[MICO_PWM_MAX];
#endif

#ifdef ALIOS_DEV_SPI
extern const platform_spi_t             platform_spi_peripherals[];
platform_spi_driver_t                   platform_spi_drivers[MICO_SPI_MAX];

#endif

#ifdef ALIOS_DEV_UART
extern const platform_uart_t            platform_uart_peripherals[];
platform_uart_driver_t                  platform_uart_drivers[MICO_UART_MAX];
#endif

#ifdef ALIOS_DEV_FLASH
#include <hal/soc/flash.h>
typedef hal_logic_partition_t           platform_logic_partition_t;
extern const mico_logic_partition_t     mico_partitions[];
#endif


/******************************************************
*               Function Definitions
******************************************************/


#ifdef ALIOS_DEV_ADC
 OSStatus MicoAdcInitialize( mico_adc_t adc, uint32_t sampling_cycle )
 {
   if ( adc >= MICO_ADC_NONE )
     return kUnsupportedErr;
   return (OSStatus) platform_adc_init( &platform_adc_peripherals[adc], sampling_cycle );
 }

 OSStatus  MicoAdcFinalize( mico_adc_t adc )
 {
   if ( adc >= MICO_ADC_NONE )
     return kUnsupportedErr;
   return (OSStatus) platform_adc_deinit( &platform_adc_peripherals[adc] );
 }

 uint16_t mico_adc_get_bit_range( mico_adc_t adc )
 {
     if ( adc >= MICO_ADC_NONE )
       return kUnsupportedErr;
     return platform_adc_get_bit_range( &platform_adc_peripherals[adc] );
 }

 OSStatus MicoAdcTakeSample( mico_adc_t adc, uint16_t* output )
 {
   if ( adc >= MICO_ADC_NONE )
     return kUnsupportedErr;
   return (OSStatus) platform_adc_take_sample( &platform_adc_peripherals[adc], output );
 }

 OSStatus MicoAdcTakeSampleStreram( mico_adc_t adc, void* buffer, uint16_t buffer_length )
 {
   if ( adc >= MICO_ADC_NONE )
     return kUnsupportedErr;
   return (OSStatus) platform_adc_take_sample_stream( &platform_adc_peripherals[adc], buffer, buffer_length );
 }

#endif


#ifdef ALIOS_DEV_GPIO

OSStatus mico_gpio_init( mico_gpio_t gpio, mico_gpio_config_t configuration )
{
    if ( gpio >= MICO_GPIO_NONE )
        return kUnsupportedErr;
   return (OSStatus) platform_gpio_init( &platform_gpio_drivers[gpio], &platform_gpio_pins[gpio], configuration );
}

OSStatus mico_gpio_output_high( mico_gpio_t gpio )
{
  if ( gpio >= MICO_GPIO_NONE )
    return kUnsupportedErr;
  return (OSStatus) platform_gpio_output_high( &platform_gpio_drivers[gpio] );
}

OSStatus mico_gpio_output_low( mico_gpio_t gpio )
{
  if ( gpio >= MICO_GPIO_NONE )
    return kUnsupportedErr;
  return (OSStatus) platform_gpio_output_low( &platform_gpio_drivers[gpio] );
}

OSStatus mico_gpio_output_toggle( mico_gpio_t gpio )
{
  if ( gpio >= MICO_GPIO_NONE )
    return kUnsupportedErr;
  return (OSStatus) platform_gpio_output_trigger( &platform_gpio_drivers[gpio] );
}

OSStatus mico_gpio_deinit( mico_gpio_t gpio )
{
  if ( gpio >= MICO_GPIO_NONE )
    return kUnsupportedErr;
  return (OSStatus) platform_gpio_deinit( &platform_gpio_drivers[gpio] );
}

bool mico_gpio_input_get( mico_gpio_t gpio )
{
  if ( gpio >= MICO_GPIO_NONE )
    return kUnsupportedErr;
  return platform_gpio_input_get( &platform_gpio_drivers[gpio] );
}


OSStatus mico_gpio_enable_irq( mico_gpio_t gpio, mico_gpio_irq_trigger_t trigger, mico_gpio_irq_handler_t handler,
                               void* arg )
{
    if ( gpio >= MICO_GPIO_NONE )
        return kUnsupportedErr;
    return (OSStatus) platform_gpio_irq_enable(  &platform_gpio_drivers[gpio], &platform_gpio_pins[gpio], trigger, handler, arg );
}

OSStatus mico_gpio_disable_irq( mico_gpio_t gpio )
{
    if ( gpio >= MICO_GPIO_NONE )
        return kUnsupportedErr;
    return (OSStatus) platform_gpio_irq_disable(  &platform_gpio_drivers[gpio] );
}

#endif

#ifdef ALIOS_DEV_I2C
OSStatus MicoI2cInitialize( mico_i2c_device_t* device )
{
    platform_i2c_config_t config;
    OSStatus result;

    if ( device->port >= MICO_I2C_NONE )
        return kUnsupportedErr;

    config.address = device->address;
    config.address_width = device->address_width;
    config.flags &= ~I2C_DEVICE_USE_DMA;
    config.speed_mode = device->speed_mode;

    if ( platform_i2c_drivers[device->port].i2c_mutex == NULL )
        mico_rtos_init_mutex( &platform_i2c_drivers[device->port].i2c_mutex );

    mico_rtos_lock_mutex( &platform_i2c_drivers[device->port].i2c_mutex );
    result = (OSStatus) platform_i2c_init( &platform_i2c_drivers[device->port], &platform_i2c_peripherals[device->port], &config );
    mico_rtos_unlock_mutex( &platform_i2c_drivers[device->port].i2c_mutex );

    return result;
}

OSStatus MicoI2cFinalize( mico_i2c_device_t* device )
{
    platform_i2c_config_t config;

    if ( device->port >= MICO_I2C_NONE )
        return kUnsupportedErr;

    config.address = device->address;
    config.address_width = device->address_width;
    config.flags &= ~I2C_DEVICE_USE_DMA;
    config.speed_mode = device->speed_mode;

    if ( platform_i2c_drivers[device->port].i2c_mutex != NULL ) {
        mico_rtos_deinit_mutex( &platform_i2c_drivers[device->port].i2c_mutex );
        platform_i2c_drivers[device->port].i2c_mutex = NULL;
    }

    return (OSStatus) platform_i2c_deinit(  &platform_i2c_drivers[device->port], &config );
}

bool MicoI2cProbeDevice( mico_i2c_device_t* device, int retries )
{
    bool ret;
    platform_i2c_config_t config;

    if ( device->port >= MICO_I2C_NONE )
        return kUnsupportedErr;

    config.address = device->address;
    config.address_width = device->address_width;
    config.flags &= ~I2C_DEVICE_USE_DMA;
    config.speed_mode = device->speed_mode;

    mico_rtos_lock_mutex( &platform_i2c_drivers[device->port].i2c_mutex );
    ret = platform_i2c_probe_device( &platform_i2c_drivers[device->port], &config, retries );
    mico_rtos_unlock_mutex( &platform_i2c_drivers[device->port].i2c_mutex );
    
    return ret;
}

OSStatus MicoI2cBuildTxMessage( mico_i2c_message_t* message, const void* tx_buffer, uint16_t tx_buffer_length,
                                uint16_t retries )
{
    return (OSStatus) platform_i2c_init_tx_message( message, tx_buffer, tx_buffer_length, retries );
}

OSStatus MicoI2cBuildRxMessage( mico_i2c_message_t* message, void* rx_buffer, uint16_t rx_buffer_length,
                                uint16_t retries )
{
    return (OSStatus) platform_i2c_init_rx_message( message, rx_buffer, rx_buffer_length, retries );
}

OSStatus MicoI2cBuildCombinedMessage( mico_i2c_message_t* message, const void* tx_buffer, void* rx_buffer,
                                      uint16_t tx_buffer_length, uint16_t rx_buffer_length, uint16_t retries )
{
    return (OSStatus) platform_i2c_init_combined_message( message, tx_buffer, rx_buffer, tx_buffer_length,
                                                          rx_buffer_length, retries );
}

OSStatus MicoI2cTransfer( mico_i2c_device_t* device, mico_i2c_message_t* messages, uint16_t number_of_messages )
{
    OSStatus err = kNoErr;
    platform_i2c_config_t config;

    if ( device->port >= MICO_I2C_NONE )
        return kUnsupportedErr;

    config.address = device->address;
    config.address_width = device->address_width;
    config.flags &= ~I2C_DEVICE_USE_DMA;
    config.speed_mode = device->speed_mode;

    mico_rtos_lock_mutex( &platform_i2c_drivers[device->port].i2c_mutex );
    err = platform_i2c_transfer( &platform_i2c_drivers[device->port], &config, messages, number_of_messages );
    mico_rtos_unlock_mutex( &platform_i2c_drivers[device->port].i2c_mutex );

    return err;
}
#endif

#ifdef ALIOS_DEV_PS
void MicoMcuPowerSaveConfig( int enable )
{
//    if (enable == 1)
//        platform_mcu_powersave_enable( );
//    else
//        platform_mcu_powersave_disable( );
}

void MicoSystemStandBy( uint32_t secondsToWakeup )
{
//    platform_mcu_enter_standby( secondsToWakeup );
}
#endif

#ifdef ALIOS_DEV_PWM
OSStatus MicoPwmInitialize(mico_pwm_t pwm, uint32_t frequency, float duty_cycle)
{
    if ( pwm >= MICO_PWM_NONE )
        return kUnsupportedErr;
    return (OSStatus) platform_pwm_init( &platform_pwm_drivers[pwm],&platform_pwm_peripherals[pwm], frequency, duty_cycle );
}

OSStatus MicoPwmStart( mico_pwm_t pwm )
{
    if ( pwm >= MICO_PWM_NONE )
        return kUnsupportedErr;
    return (OSStatus)platform_pwm_start( &platform_pwm_drivers[pwm] );
}

OSStatus MicoPwmStop( mico_pwm_t pwm )
{
    if ( pwm >= MICO_PWM_NONE )
        return kUnsupportedErr;
    return (OSStatus) platform_pwm_stop( &platform_pwm_drivers[pwm] );
}
#endif

#ifdef ALIOS_DEV_RTC
OSStatus mico_rtc_init(void)
{
    return platform_rtc_init();
}

OSStatus mico_rtc_get_time(time_t *t)
{
    return platform_rtc_get_time(t);
}

OSStatus mico_rtc_set_time(time_t t)
{
    return platform_rtc_set_time(t);
}
#endif


#ifdef ALIOS_DEV_SPI
OSStatus MicoSpiInitialize( const mico_spi_device_t* spi )
{
    platform_spi_config_t config;
    OSStatus err = kNoErr;

    if ( spi->port >= MICO_SPI_NONE )
        return kUnsupportedErr;

    if ( platform_spi_drivers[spi->port].spi_mutex == NULL )
        mico_rtos_init_mutex( &platform_spi_drivers[spi->port].spi_mutex );

    config.cs = &platform_gpio_pins[spi->chip_select];
    config.cs_drv = &platform_gpio_drivers[spi->chip_select];
    config.speed = spi->speed;
    config.mode = spi->mode;
    config.bits = spi->bits;

    mico_rtos_lock_mutex( &platform_spi_drivers[spi->port].spi_mutex );
    err = platform_spi_init( &platform_spi_drivers[spi->port], &platform_spi_peripherals[spi->port], &config );
    mico_rtos_unlock_mutex( &platform_spi_drivers[spi->port].spi_mutex );

    return err;
}

OSStatus MicoSpiFinalize( const mico_spi_device_t* spi )
{
    OSStatus err = kNoErr;

    if ( spi->port >= MICO_SPI_NONE )
        return kUnsupportedErr;

    if ( platform_spi_drivers[spi->port].spi_mutex == NULL )
        mico_rtos_init_mutex( &platform_spi_drivers[spi->port].spi_mutex );

    mico_rtos_lock_mutex( &platform_spi_drivers[spi->port].spi_mutex );
    err = platform_spi_deinit( &platform_spi_drivers[spi->port] );
    mico_rtos_unlock_mutex( &platform_spi_drivers[spi->port].spi_mutex );

    return err;
}

OSStatus MicoSpiTransfer( const mico_spi_device_t* spi, const mico_spi_message_segment_t* segments,
                          uint16_t number_of_segments )
{
    platform_spi_config_t config;
    OSStatus err = kNoErr;

    if ( spi->port >= MICO_SPI_NONE )
        return kUnsupportedErr;

    if ( platform_spi_drivers[spi->port].spi_mutex == NULL )
        mico_rtos_init_mutex( &platform_spi_drivers[spi->port].spi_mutex );

    config.cs = &platform_gpio_pins[spi->chip_select];
    config.cs_drv = &platform_gpio_drivers[spi->chip_select];
    config.speed = spi->speed;
    config.mode = spi->mode;
    config.bits = spi->bits;

    mico_rtos_lock_mutex( &platform_spi_drivers[spi->port].spi_mutex );
    err = platform_spi_init( &platform_spi_drivers[spi->port], &platform_spi_peripherals[spi->port], &config );
    err = platform_spi_transfer( &platform_spi_drivers[spi->port], &config, segments, number_of_segments );
    mico_rtos_unlock_mutex( &platform_spi_drivers[spi->port].spi_mutex );

    return err;
}

// OSStatus MicoSpiSlaveInitialize( mico_spi_t spi, const mico_spi_slave_config_t* config )
// {
//   if ( spi >= MICO_SPI_NONE )
//     return kUnsupportedErr;

//   return (OSStatus) platform_spi_slave_init( &platform_spi_slave_drivers[spi], &platform_spi_peripherals[spi], config );
// }

// OSStatus MicoSpiSlaveFinalize( mico_spi_t spi )
// {
//   if ( spi >= MICO_SPI_NONE )
//     return kUnsupportedErr;

//   return (OSStatus) platform_spi_slave_deinit( &platform_spi_slave_drivers[spi] );
// }

// OSStatus  MicoSpiSlaveSendErrorStatus( mico_spi_t spi, mico_spi_slave_transfer_status_t error_status )
// {
//   if ( spi >= MICO_SPI_NONE )
//     return kUnsupportedErr;

//   return (OSStatus) platform_spi_slave_send_error_status( &platform_spi_slave_drivers[spi], error_status );
// }

// OSStatus MicoSpiSlaveReceiveCommand( mico_spi_t spi, mico_spi_slave_command_t* command, uint32_t timeout_ms )
// {
//   if ( spi >= MICO_SPI_NONE )
//     return kUnsupportedErr;  

//   return (OSStatus) platform_spi_slave_receive_command( &platform_spi_slave_drivers[spi], command, timeout_ms );
// }

// OSStatus MicoSpiSlaveTransferData( mico_spi_t spi, mico_spi_slave_transfer_direction_t direction, mico_spi_slave_data_buffer_t* buffer, uint32_t timeout_ms )
// {
//   if ( spi >= MICO_SPI_NONE )
//     return kUnsupportedErr;  

//   return (OSStatus) platform_spi_slave_transfer_data( &platform_spi_slave_drivers[spi], direction, buffer, timeout_ms );
// }

// OSStatus MicoSpiSlaveGenerateInterrupt( mico_spi_t spi, uint32_t pulse_duration_ms )
// {
//   if ( spi >= MICO_SPI_NONE )
//     return kUnsupportedErr;

//   return (OSStatus) platform_spi_slave_generate_interrupt( &platform_spi_slave_drivers[spi], pulse_duration_ms );
// }

#endif

#ifdef ALIOS_DEV_UART
OSStatus mico_uart_init( mico_uart_t uart, const mico_uart_config_t* config, ring_buffer_t* optional_rx_buffer )
{
    if ( uart >= MICO_UART_NONE )
        return kUnsupportedErr;

#ifndef MICO_DISABLE_STDIO
    /* Interface is used by STDIO. Uncomment MICO_DISABLE_STDIO to overcome this */
    if ( uart == MICO_STDIO_UART )
    {
        return kGeneralErr;
    }
#endif

    return (OSStatus) platform_uart_init( &platform_uart_drivers[uart], &platform_uart_peripherals[uart], config,
                                          optional_rx_buffer );
}

OSStatus mico_stdio_uart_init( const mico_uart_config_t* config, ring_buffer_t* optional_rx_buffer )
{

    return (OSStatus) platform_uart_init( &platform_uart_drivers[MICO_STDIO_UART],
                                          &platform_uart_peripherals[MICO_STDIO_UART],
                                          config, optional_rx_buffer );
}


OSStatus mico_uart_deinit( mico_uart_t uart )
{
    if ( uart >= MICO_UART_NONE )
        return kUnsupportedErr;

    return (OSStatus) platform_uart_deinit( &platform_uart_drivers[uart] );
}

OSStatus mico_uart_send( mico_uart_t uart, const void* data, uint32_t size )
{
    if ( uart >= MICO_UART_NONE )
        return kUnsupportedErr;

    return (OSStatus) platform_uart_transmit_bytes( &platform_uart_drivers[uart], (const uint8_t*) data, size );
}

OSStatus mico_uart_recv( mico_uart_t uart, void* data, uint32_t size, uint32_t timeout )
{
    if ( uart >= MICO_UART_NONE )
        return kUnsupportedErr;

    return (OSStatus) platform_uart_receive_bytes( &platform_uart_drivers[uart], (uint8_t*) data, size, timeout );
}

uint32_t mico_uart_recvd_data_len( mico_uart_t uart )
{
    if ( uart >= MICO_UART_NONE )
        return 0;

    return (OSStatus) platform_uart_get_length_in_buffer( &platform_uart_drivers[uart] );
}
#endif

#ifdef ALIOS_DEV_RNG
OSStatus MicoRandomNumberRead( void *inBuffer, int inByteCount )
{
   return (OSStatus) platform_random_number_read( inBuffer, inByteCount );
}
#endif

extern void hal_reboot(void);

void mico_system_reboot( void )
{
    hal_reboot();
}

#ifdef ALIOS_DEV_WDG
OSStatus mico_wdg_init( uint32_t timeout )
{
    return (OSStatus) platform_watchdog_init( timeout );
}

void mico_wdg_reload( void )
{
    platform_watchdog_kick( );
}
#endif

#ifdef ALIOS_DEV_FLASH
mico_logic_partition_t* MicoFlashGetInfo( mico_partition_t partition )
{
    return hal_flash_get_info(partition);
}


//static OSStatus MicoFlashInitialize( mico_partition_t partition )
//{
//    return hal_flash_init( partition );
//}

OSStatus MicoFlashErase( mico_partition_t partition, uint32_t off_set, uint32_t size )
{
    return hal_flash_erase(partition, off_set, size);
}

OSStatus MicoFlashWrite( mico_partition_t partition, volatile uint32_t* off_set, uint8_t* inBuffer,
                         uint32_t inBufferLength )
{
    return hal_flash_write(partition, (uint32_t *)off_set, inBuffer, inBufferLength);

}

OSStatus MicoFlashRead( mico_partition_t partition, volatile uint32_t* off_set, uint8_t* outBuffer,
                        uint32_t inBufferLength )
{
    return hal_flash_read(partition, (uint32_t *)off_set, outBuffer, inBufferLength);
}

OSStatus MicoFlashEnableSecurity( mico_partition_t partition, uint32_t off_set, uint32_t size )
{
    return hal_flash_enable_secure(partition, off_set, size);
}

char *mico_get_bootloader_ver( void )
{
    static char ver[33];
    const mico_logic_partition_t* bootloader_partition = &mico_partitions[MICO_PARTITION_BOOTLOADER];
    uint32_t version_offset = bootloader_partition->partition_length - 0x20;

    memset( ver, 0, sizeof(ver) );
    MicoFlashRead( MICO_PARTITION_BOOTLOADER, &version_offset, (uint8_t *) ver, 32 );
    return ver;
}

#ifdef BOOTLOADER
#include "bootloader.h"

void mico_set_bootload_ver(void)
{
    uint8_t ver[33];
    mico_logic_partition_t *boot_partition = MicoFlashGetInfo( MICO_PARTITION_BOOTLOADER );
    uint32_t flashaddr = boot_partition->partition_length - 0x20;
    int i;

    memset(ver, 0, sizeof(ver));
    MicoFlashRead( MICO_PARTITION_BOOTLOADER, &flashaddr, (uint8_t *)ver , 32);
    for(i=0;i<32;i++) {
        if (ver[i] != 0xFF)
        return;
    }
    snprintf((char *)ver, 33, "%s %s %d", MODEL, Bootloader_REVISION , MICO_STDIO_UART_BAUDRATE);
    flashaddr = boot_partition->partition_length - 0x20;
    MicoFlashDisableSecurity( MICO_PARTITION_BOOTLOADER, 0x0, boot_partition->partition_length );
    MicoFlashWrite( MICO_PARTITION_BOOTLOADER, &flashaddr, ver , 32);
}

OSStatus MicoFlashDisableSecurity( mico_partition_t partition, uint32_t off_set, uint32_t size )
{
    return hal_flash_dis_secure(partition, off_set, size);
}
#endif

#endif

#if 0
static micoMemInfo_t mico_memory;

extern unsigned char __StackLimit[];
extern unsigned char __end__[];

micoMemInfo_t* mico_memory_info( void )
{
    struct mallinfo mi = mallinfo();

    int total_mem = __StackLimit - __end__;

    mico_memory.allocted_memory = mi.uordblks;
    mico_memory.free_memory = total_mem - mi.uordblks;
    mico_memory.num_of_chunks = mi.ordblks;
    mico_memory.total_memory = total_mem;
    return &mico_memory;
}
#endif

WEAK void platform_eth_mac_address( char *mac )
{
    mac[0] = 0x00;
    mac[1] = 0x02;
    mac[2] = 0xF7;
    mac[3] = 0xF0;
    mac[4] = 0x00;
    mac[5] = 0x00;
}

