#include "fpga_driver.hpp"

fpga_driver::fpga_driver()
{
  this->fpga_driver_gpio_init();
}

void fpga_driver::fpga_driver_gpio_init()
{
  GPIO_InitTypeDef * GPIO_InitStruct = new GPIO_InitTypeDef;

  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin = FPGA_CMD_GPIO_PIN0 | FPGA_CMD_GPIO_PIN1 | FPGA_CMD_GPIO_PIN2 | FPGA_CMD_GPIO_PIN3 | FPGA_CMD_GPIO_PIN4 | FPGA_CMD_GPIO_PIN5 | FPGA_CMD_GPIO_PIN6 | FPGA_DATA_GPIO_PIN0 | FPGA_DATA_GPIO_PIN1 | FPGA_DATA_GPIO_PIN2 | FPGA_DATA_GPIO_PIN3 | FPGA_DATA_GPIO_PIN4 | FPGA_DATA_GPIO_PIN5 | FPGA_DATA_GPIO_PIN6 | FPGA_DATA_GPIO_PIN7;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( FPGA_CMD_DATA_GPIO_PORT, GPIO_InitStruct );

  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin = FPGA_EN_GPIO_PIN;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( FPGA_EN_GPIO_PORT, GPIO_InitStruct );

  GPIO_ResetBits( FPGA_EN_GPIO_PORT, FPGA_EN_GPIO_PIN );
  GPIO_ResetBits( FPGA_CMD_DATA_GPIO_PORT, FPGA_CMD_GPIO_PIN0 | FPGA_CMD_GPIO_PIN1 | FPGA_CMD_GPIO_PIN2 | FPGA_CMD_GPIO_PIN3 | FPGA_CMD_GPIO_PIN4 | FPGA_CMD_GPIO_PIN5 | FPGA_CMD_GPIO_PIN6 | FPGA_DATA_GPIO_PIN0 | FPGA_DATA_GPIO_PIN1 | FPGA_DATA_GPIO_PIN2 | FPGA_DATA_GPIO_PIN3 | FPGA_DATA_GPIO_PIN4 | FPGA_DATA_GPIO_PIN5 | FPGA_DATA_GPIO_PIN6 | FPGA_DATA_GPIO_PIN7 );
  GPIO_SetBits( FPGA_CMD_DATA_GPIO_PORT, FPGA_CMD_GPIO_PIN1 | FPGA_DATA_GPIO_PIN6 | FPGA_DATA_GPIO_PIN7 );

  delete GPIO_InitStruct;
}

void fpga_driver::fpga_driver_gpio_deinit()
{
  GPIO_InitTypeDef * GPIO_InitStruct = new GPIO_InitTypeDef;

  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin = FPGA_CMD_GPIO_PIN0 | FPGA_CMD_GPIO_PIN1 | FPGA_CMD_GPIO_PIN2 | FPGA_CMD_GPIO_PIN3 | FPGA_CMD_GPIO_PIN4 | FPGA_CMD_GPIO_PIN5 | FPGA_CMD_GPIO_PIN6 | FPGA_DATA_GPIO_PIN0 | FPGA_DATA_GPIO_PIN1 | FPGA_DATA_GPIO_PIN2 | FPGA_DATA_GPIO_PIN3 | FPGA_DATA_GPIO_PIN4 | FPGA_DATA_GPIO_PIN5 | FPGA_DATA_GPIO_PIN6 | FPGA_DATA_GPIO_PIN7;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( FPGA_CMD_DATA_GPIO_PORT, GPIO_InitStruct );
  
  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin = FPGA_EN_GPIO_PIN;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( FPGA_EN_GPIO_PORT, GPIO_InitStruct );

  GPIO_ResetBits( FPGA_EN_GPIO_PORT, FPGA_EN_GPIO_PIN );
  GPIO_ResetBits( FPGA_CMD_DATA_GPIO_PORT, FPGA_CMD_GPIO_PIN0 | FPGA_CMD_GPIO_PIN1 | FPGA_CMD_GPIO_PIN2 | FPGA_CMD_GPIO_PIN3 | FPGA_CMD_GPIO_PIN4 | FPGA_CMD_GPIO_PIN5 | FPGA_CMD_GPIO_PIN6 | FPGA_DATA_GPIO_PIN0 | FPGA_DATA_GPIO_PIN1 | FPGA_DATA_GPIO_PIN2 | FPGA_DATA_GPIO_PIN3 | FPGA_DATA_GPIO_PIN4 | FPGA_DATA_GPIO_PIN5 | FPGA_DATA_GPIO_PIN6 | FPGA_DATA_GPIO_PIN7 );
  GPIO_SetBits( FPGA_CMD_DATA_GPIO_PORT, FPGA_CMD_GPIO_PIN1 | FPGA_DATA_GPIO_PIN6 | FPGA_DATA_GPIO_PIN7 );

  delete GPIO_InitStruct;
}

void fpga_driver::fpga_write( uint32_t command, uint32_t data )
{
  std::bitset< 16 > * port_data = new std::bitset< 16 >( data | ( command << 8 ));
  GPIO_Write( FPGA_CMD_DATA_GPIO_PORT, port_data->to_ulong() );
  GPIO_ResetBits( FPGA_EN_GPIO_PORT, FPGA_EN_GPIO_PIN );
  GPIO_SetBits( FPGA_EN_GPIO_PORT, FPGA_EN_GPIO_PIN );
  GPIO_ResetBits( FPGA_EN_GPIO_PORT, FPGA_EN_GPIO_PIN );
  port_data->reset();
  GPIO_Write( FPGA_CMD_DATA_GPIO_PORT, port_data->to_ulong() );
  delete port_data;
}

void fpga_driver::fpga_adc_select( uint32_t channel_num )
{
  this->fpga_write( FPGA_ADC_SELECT_CMD, channel_num );
}

void fpga_driver::fpga_adc_unselect()
{
  this->fpga_write( FPGA_ADC_UNSELECT_CMD, 0x00 );
}

void fpga_driver::fpga_red_led_off( uint32_t channel_num )
{
  this->fpga_write( FPGA_RED_LED_OFF_CMD, channel_num );
}

void fpga_driver::fpga_red_led_on( uint32_t channel_num )
{
  this->fpga_write( FPGA_RED_LED_ON_CMD, channel_num );
}

void fpga_driver::fpga_green_led_off( uint32_t channel_num )
{
  this->fpga_write( FPGA_GREEN_LED_OFF_CMD, channel_num );
}

void fpga_driver::fpga_green_led_on( uint32_t channel_num )
{
  this->fpga_write( FPGA_GREEN_LED_ON_CMD, channel_num );
}

void fpga_driver::fpga_reset( void )
{
  for( unsigned int i = 0; i < CHANNELS_NUMBER; i ++ ){
	
	this->fpga_red_led_off( i );
	this->fpga_green_led_off( i );

  }
  
  this->fpga_adc_unselect();;
}

fpga_driver::~fpga_driver()
{
  this->fpga_driver_gpio_deinit();
}
