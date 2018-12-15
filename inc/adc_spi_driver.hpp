#ifndef ADC_SPI_DRIVER_HPP
#define ADC_SPI_DRIVER_HPP

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "bsp_macro.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <string>
#include <functional>
#include <bitset>
#include <map>
#include <cmath>

#define ADC_SS_RESET_MODE           std::bitset< 1 >( 0b0 )
#define ADC_SS_START_MODE           std::bitset< 1 >( 0b1 )
#define ADC_SS_FALSE_MODE           std::bitset< 1 >( 0b0 )
#define ADS1118_SS_POS              15

#define ADC_MUX_RESET_MODE          std::bitset< 3 >( 0b000 )
#define ADC_MUX_0P_1N_MODE          std::bitset< 3 >( 0b000 )
#define ADC_MUX_0P_3N_MODE          std::bitset< 3 >( 0b001 )
#define ADC_MUX_1P_3N_MODE          std::bitset< 3 >( 0b010 )
#define ADC_MUX_2P_3N_MODE          std::bitset< 3 >( 0b011 )
#define ADC_MUX_0P_GN_MODE          std::bitset< 3 >( 0b100 )
#define ADC_MUX_1P_GN_MODE          std::bitset< 3 >( 0b101 )
#define ADC_MUX_2P_GN_MODE          std::bitset< 3 >( 0b110 )
#define ADC_MUX_3P_GN_MODE          std::bitset< 3 >( 0b111 )
#define ADS1118_MUX_POS             12

#define ADC_PGA_RESET_MODE          std::bitset< 3 >( 0b010 )
#define ADC_PGA_FSR_6144_MODE       std::bitset< 3 >( 0b000 )
#define ADC_PGA_FSR_4096_MODE       std::bitset< 3 >( 0b001 )
#define ADC_PGA_FSR_2048_MODE       std::bitset< 3 >( 0b010 )
#define ADC_PGA_FSR_1024_MODE       std::bitset< 3 >( 0b011 )
#define ADC_PGA_FSR_512_MODE        std::bitset< 3 >( 0b100 )
#define ADC_PGA_FSR_256_MODE        std::bitset< 3 >( 0b101 )
#define ADC_PGA_FSR_128_MODE        std::bitset< 3 >( 0b110 )
#define ADC_PGA_FSR_64_MODE         std::bitset< 3 >( 0b111 )
#define ADS1118_PGA_POS             9

#define ADC_OPMODE_RESET_MODE       std::bitset< 1 >( 0b1 )
#define ADC_OPMODE_CC_MODE          std::bitset< 1 >( 0b0 )
#define ADC_OPMODE_SS_PWRDWN_MODE   std::bitset< 1 >( 0b1 )
#define ADS1118_OPMODE_POS          8

#define ADC_DR_RESET_MODE           std::bitset< 3 >( 0b100 )
#define ADC_DR_8_MODE               std::bitset< 3 >( 0b000 )
#define ADC_DR_16_MODE              std::bitset< 3 >( 0b001 )
#define ADC_DR_32_MODE              std::bitset< 3 >( 0b010 )
#define ADC_DR_64_MODE              std::bitset< 3 >( 0b011 )
#define ADC_DR_128_MODE             std::bitset< 3 >( 0b100 )
#define ADC_DR_250_MODE             std::bitset< 3 >( 0b101 )
#define ADC_DR_475_MODE             std::bitset< 3 >( 0b110 )
#define ADC_DR_860_MODE             std::bitset< 3 >( 0b111 )
#define ADS1118_DR_POS              5

#define ADC_TSMODE_RESET_MODE       std::bitset< 1 >( 0b0 )
#define ADC_TSMODE_ADC_MODE         std::bitset< 1 >( 0b0 )
#define ADC_TSMODE_TEMP_MODE        std::bitset< 1 >( 0b1 )
#define ADS1118_TSMODE_POS          4

#define ADC_PULLUP_RESET_MODE       std::bitset< 1 >( 0b1 )
#define ADC_PULLUP_DIS_MODE         std::bitset< 1 >( 0b0 )
#define ADC_PULLUP_EN_MODE          std::bitset< 1 >( 0b1 )
#define ADS1118_PULLUPEN_POS        3

#define ADC_NOP_RESET_MODE          std::bitset< 2 >( 0b01 )
#define ADC_NOP_INVALID_DATA_MODE   std::bitset< 2 >( 0b00 )
#define ADC_NOP_VALID_DATA_MODE     std::bitset< 2 >( 0b01 )
#define ADS1118_NOP_POS             1

#define ADC_RDY_FLAG_MODE           std::bitset< 1 >( 0b0 )
#define ADS1118_RDY_FLAG_POS        0

class adc_spi_driver
{
public:

  explicit adc_spi_driver();
  virtual ~adc_spi_driver();

  typedef struct {

	std::bitset< 1 > OS = ADC_SS_RESET_MODE;
	std::bitset< 3 > MUX = ADC_MUX_RESET_MODE;
	std::bitset< 3 > PGA = ADC_PGA_RESET_MODE;
	std::bitset< 1 > MODE = ADC_OPMODE_RESET_MODE;
	std::bitset< 3 > DR = ADC_DR_RESET_MODE;
	std::bitset< 1 > TS_MODE = ADC_TSMODE_RESET_MODE;
	std::bitset< 1 > PULL_UP_EN = ADC_PULLUP_RESET_MODE;
	std::bitset< 2 > NOP = ADC_NOP_RESET_MODE;
	std::bitset< 1 > RDY_FLAG = ADC_RDY_FLAG_MODE;
	
  } ADS1118_ConfRegInitTypeDef;

  typedef struct {

	uint16_t data = 0x0000;
	uint8_t channel = 0x00;
	
  } ADS1118_ConvRegInitTypeDef;
    
  uint16_t adc_get_data( ADS1118_ConfRegInitTypeDef * ADS1118_ConfRegInitStruct );
  void adc_transmit_data( ADS1118_ConfRegInitTypeDef * ADS1118_ConfRegInitStruct );
  
private:
  
  void adc_spi_driver_gpio_init();
  void adc_spi_driver_gpio_deinit();
  void adc_spi_driver_periph_init();
  void adc_spi_driver_periph_deinit();
  
};

#endif /* ADC_SPI_DRIVER_HPP */
