#ifndef FPGA_DRIVER_HPP
#define FPGA_DRIVER_HPP

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "bsp_macro.hpp"
#include <bitset>
#include <functional>

#define FPGA_ADC_SELECT_CMD                   ( uint32_t ) 0x0a
#define FPGA_ADC_UNSELECT_CMD                 ( uint32_t ) 0x0b
#define FPGA_RED_LED_ON_CMD                   ( uint32_t ) 0x0c
#define FPGA_RED_LED_OFF_CMD                  ( uint32_t ) 0x0d
#define FPGA_GREEN_LED_ON_CMD                 ( uint32_t ) 0x0e
#define FPGA_GREEN_LED_OFF_CMD                ( uint32_t ) 0x0f

class fpga_driver
{

public:

  explicit fpga_driver();
  virtual ~fpga_driver();
    
  void fpga_adc_select( uint32_t channel_num );
  void fpga_adc_unselect( void );
  void fpga_red_led_off( uint32_t channel_num );
  void fpga_red_led_on( uint32_t channel_num );
  void fpga_green_led_off( uint32_t channel_num );
  void fpga_green_led_on( uint32_t channel_num );
  void fpga_reset( void );

private:

  void fpga_write( uint32_t command, uint32_t data );
  void fpga_driver_gpio_init();
  void fpga_driver_gpio_deinit();
};

#endif /* FPGA_DRIVER_HPP */
