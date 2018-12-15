#include "current_sources_driver.hpp"

const double vref_volts = 3.3f;
const double r_shint_ohms = 220.0f;
const double dac_quants = 4096.0f;  
bool current_source_on_state = false;
double current_ma = 0.0f;

current_sources_driver::current_sources_driver()
{
  this->current_sources_driver_gpio_init();
  this->current_sources_driver_periph_init();
}

void current_sources_driver::current_sources_driver_gpio_init()
{
  GPIO_InitTypeDef * GPIO_InitStruct = new GPIO_InitTypeDef;
  
  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin = DAC_GPIO_PIN;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( DAC_GPIO_PORT, GPIO_InitStruct );
  
  delete GPIO_InitStruct;
}

void current_sources_driver::current_sources_driver_gpio_deinit()
{
  GPIO_InitTypeDef * GPIO_InitStruct = new GPIO_InitTypeDef;
  
  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin = DAC_GPIO_PIN;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( DAC_GPIO_PORT, GPIO_InitStruct );
  GPIO_ResetBits( DAC_GPIO_PORT, DAC_GPIO_PIN );
  
  delete GPIO_InitStruct;
}

void current_sources_driver::current_sources_driver_periph_init()
{
  DAC_InitTypeDef * DAC_InitStruct = new DAC_InitTypeDef;

  DAC_StructInit( DAC_InitStruct );
  DAC_InitStruct->DAC_Trigger = DAC_Trigger_None;
  DAC_InitStruct->DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStruct->DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init( DAC_Channel_1, DAC_InitStruct );
  DAC_Cmd( DAC_Channel_1, ENABLE );
  DAC_SetChannel1Data( DAC_Align_12b_R, 0x00 );

  delete DAC_InitStruct;
}

void current_sources_driver::current_sources_driver_periph_deinit()
{
  DAC_DeInit();
}
  
void current_sources_driver::current_source_set_voltage( double mvolt )
{
  DAC_SetChannel1Data( DAC_Align_12b_R, static_cast< uint16_t >(( mvolt /( vref_volts * 1000 )) * dac_quants ));
}

void current_sources_driver::current_source_set_current( double milliamperes )
{
  if( milliamperes == 0.0f ){

	this->current_source_turn_off();
	
  } else {

	this->current_source_turn_on();
	current_source_set_voltage( r_shint_ohms * milliamperes );
	
  }
}

void current_sources_driver::current_source_turn_off()
{
  this->current_sources_driver_gpio_deinit();
  this->current_sources_driver_periph_deinit();
  current_source_on_state = false;
}

void current_sources_driver::current_source_turn_on()
{
  this->current_sources_driver_gpio_init();
  this->current_sources_driver_periph_init();
  current_source_on_state = true;
}

current_sources_driver::~current_sources_driver()
{
  this->current_sources_driver_gpio_deinit();
  this->current_sources_driver_periph_deinit();
}
