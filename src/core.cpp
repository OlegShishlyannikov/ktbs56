#include "core.hpp"

TaskHandle_t core_task_handle;
TaskHandle_t irq_controller_task_handle;

double vref = 0.256000f;
double divider = 101.000000f;
double max_adc_val = 32768.000000f;
double mistake = 0.020000f;

double voltage = 5.000000f;
double current = 1.000000f;

bool is_running = false;

bool voltage_select_mode = true;
bool current_select_mode = false;
bool mistake_select_mode = false;

core::core()
{
  __asm__( "nop" );
}

void core::load_drivers()
{
  this->_fpga = new fpga_driver;
  this->_usart = new usart_driver;
  this->_current_sources = new current_sources_driver;
  this->_adc_spi = new adc_spi_driver;
  this->_display = new display_driver;
  this->_buttons = new buttons_driver;
}

void core::run()
{
  xTaskCreate( &irq_controller_wrapper, "irq_controller", configMINIMAL_STACK_SIZE, nullptr, 5, &irq_controller_task_handle );
  xTaskCreate( &core_task_wrapper, "core", configMINIMAL_STACK_SIZE, nullptr, 5, &core_task_handle );
  vTaskStartScheduler();
}

void core::irq_controller( void * param_ptr )
{
  char irq_buttons_buffer = '\0';
  
  /* Button press handlers */
  std::function< void( void )> button_plus_handler = [ = ]( void ) mutable -> void {
	if(( current_select_mode ) && !( is_running )){

	  if( current < 12.0f ) current += 1.0f;

	  for( unsigned int i = 0; i < CHANNELS_NUMBER - 1; i ++ ){
		
		this->_fpga->fpga_red_led_off( i );
		this->_fpga->fpga_green_led_off( i );
		
	  }

	  for( unsigned int i = 0; i < static_cast< unsigned int >( current ); i ++ ) this->_fpga->fpga_green_led_on( i );
	  
	} else if(( voltage_select_mode ) && !( is_running )){

	  if( voltage < 24.0f ) voltage += 1.0f;

	  for( unsigned int i = 0; i < CHANNELS_NUMBER - 1; i ++ ){
		
		this->_fpga->fpga_red_led_off( i );
		this->_fpga->fpga_green_led_off( i );
		
	  }

	  for( unsigned int i = 0; i < static_cast< unsigned int >( voltage ); i ++ ) this->_fpga->fpga_red_led_on( i );
	  
	} else if(( mistake_select_mode ) && !( is_running )){

	  if( mistake < 0.1f ) mistake += 0.01f;

	  for( unsigned int i = 0; i < CHANNELS_NUMBER - 1; i ++ ){
		
		this->_fpga->fpga_red_led_off( i );
		this->_fpga->fpga_green_led_off( i );
		
	  }

	  for( unsigned int i = 0; i < static_cast< unsigned int >( mistake * 100.0f ); i ++ ){

		this->_fpga->fpga_red_led_on( i );
		this->_fpga->fpga_green_led_on( i );

	  }
	  
	} else __asm__( "nop" );
  };

  std::function< void( void )> button_minus_handler = [ = ]( void ) mutable -> void {
	if(( current_select_mode ) && !( is_running )){

	  if( current > 0.0f ) current -= 1.0f;

	  for( unsigned int i = 0; i < CHANNELS_NUMBER - 1; i ++ ){
		
		this->_fpga->fpga_red_led_off( i );
		this->_fpga->fpga_green_led_off( i );
		
	  }

	  for( unsigned int i = 0; i < static_cast< unsigned int >( current ); i ++ ) this->_fpga->fpga_green_led_on( i );
	  
	  } else if(( voltage_select_mode ) && !( is_running )){

		  if( voltage > 0.0f ) voltage -= 1.0f;

		  for( unsigned int i = 0; i < CHANNELS_NUMBER - 1; i ++ ){
		
			this->_fpga->fpga_red_led_off( i );
			this->_fpga->fpga_green_led_off( i );
		
		  }

		  for( unsigned int i = 0; i < static_cast< unsigned int >( voltage ); i ++ ) this->_fpga->fpga_red_led_on( i );
	  
		} else if(( mistake_select_mode ) && !( is_running )){

		  if( mistake > 0.01f ) mistake -= 0.01f;

		  for( unsigned int i = 0; i < CHANNELS_NUMBER - 1; i ++ ){
		
			this->_fpga->fpga_red_led_off( i );
			this->_fpga->fpga_green_led_off( i );
		
		  }

		  for( unsigned int i = 0; i < static_cast< unsigned int >( mistake * 100.0f ); i ++ ){

			this->_fpga->fpga_red_led_on( i );
			this->_fpga->fpga_green_led_on( i );

		  }
	  
		} else __asm__( "nop" );
	};

	std::function< void( void )> button_run_handler = [ = ]( void ) mutable -> void {
	  unsigned  int i = 0;
	
	  for( i = 0; i < 2000; i ++ ){

		vTaskDelay( 1 );
		if( this->_buttons->is_run_pressed() ) continue;
		else break;
	  
	  }

	  if( i == 2000 ){

		if( is_running ){

		  is_running = false;
		  this->_current_sources->current_source_turn_off();
		  
		} else {

		  is_running = true;
		  this->_current_sources->current_source_turn_on();
		  this->_current_sources->current_source_set_current( current );
		  
		}
	  } else {

		if( current_select_mode == true ){

		  voltage_select_mode = true;
		  current_select_mode = false;
		  mistake_select_mode = false;
		  
		} else if( voltage_select_mode == true ){

		  mistake_select_mode = true;
		  voltage_select_mode = false;
		  current_select_mode = false;
		  
		} else if( mistake_select_mode == true ){

		  mistake_select_mode = false;
		  voltage_select_mode = false;
		  current_select_mode = true;
		  
		}
	  }
	};

	bool is_busy = false;
	while( true ){
	  /* Handle buttons interrupts */
	  if( xQueueReceive( buttons_irq_queue, &irq_buttons_buffer, portMAX_DELAY )){

		if( irq_buttons_buffer == '+' ){

		  button_plus_handler();
			
		} else if( irq_buttons_buffer == '-' ){

		  button_minus_handler();
		
		} else if( irq_buttons_buffer == '=' ){

		  button_run_handler();
		
		} else __asm__( "nop" );
	  }	
	}
	vTaskDelete( nullptr );
}
  
void core::task( void * param_ptr )
{
#define CORE_QUEUE_LENGTH 64

  /* Init ADC structures */
  adc_spi_driver::ADS1118_ConfRegInitTypeDef ADS1118_ConfRegInitStruct;
  
  std::function< uint16_t( unsigned int )>
	adc_get_conversion_value = [ = ]( unsigned int channel ) mutable -> uint16_t
	{
	  /* Select ADC */
	  this->_fpga->fpga_adc_select( channel );

	  /* Init comfiguration register structure */
	  ADS1118_ConfRegInitStruct.OS = ADC_SS_START_MODE;
	  ADS1118_ConfRegInitStruct.PGA = ADC_PGA_FSR_256_MODE;
	  ADS1118_ConfRegInitStruct.MODE = ADC_OPMODE_SS_PWRDWN_MODE;
	  ADS1118_ConfRegInitStruct.DR = ADC_DR_860_MODE;
	  ADS1118_ConfRegInitStruct.TS_MODE = ADC_TSMODE_ADC_MODE;
	  ADS1118_ConfRegInitStruct.PULL_UP_EN = ADC_PULLUP_EN_MODE;
	  ADS1118_ConfRegInitStruct.NOP = ADC_NOP_VALID_DATA_MODE;
	  ADS1118_ConfRegInitStruct.RDY_FLAG = ADC_RDY_FLAG_MODE;
	  ADS1118_ConfRegInitStruct.MUX = ( channel % 2 ) ? ADC_MUX_2P_3N_MODE : ADC_MUX_0P_1N_MODE;

	  if( channel == CHANNELS_NUMBER - 1 ) ADS1118_ConfRegInitStruct.MUX = ADC_MUX_0P_1N_MODE;
	  
	  this->_adc_spi->adc_transmit_data( &ADS1118_ConfRegInitStruct );
	  this->_adc_spi->adc_transmit_data( &ADS1118_ConfRegInitStruct );
	  while( GPIO_ReadInputDataBit( ADC_SPI_GPIO_PORT, ADC_SPI_GPIO_MISO_PIN ));
	  this->_adc_spi->adc_transmit_data( &ADS1118_ConfRegInitStruct );
	  uint16_t data = SPI_I2S_ReceiveData( ADC_SPI_PERIPH );
	  this->_adc_spi->adc_transmit_data( &ADS1118_ConfRegInitStruct );

	  return data;
	};

  /* Enable current sources */
  this->_current_sources->current_source_turn_off();

  /* Wait for FPGA configured */
  vTaskDelay( 50 );  
  this->_fpga->fpga_reset();
   
  while( true ){

	if( is_running ){
	  
	  this->_usart->send_msg( "\x1b[H" );
	  for( unsigned int i = 0; i < CHANNELS_NUMBER; i ++ ){

		double result = 0.0f;
		double temp = 0.0f;
		double volts = 0.0f;
	  
		for( unsigned int j = 0; j < 100; j ++ ) adc_get_conversion_value( i );
		result = adc_get_conversion_value( i );
		volts = static_cast< double >(( result / max_adc_val ) * vref * divider );
		this->_usart->send_msg( "ADC channel %u voltage = %lf Volts\r\n", i, voltage );

		if(( volts < voltage - voltage * mistake ) || ( volts > voltage + voltage * mistake )){

		  this->_fpga->fpga_green_led_off( i );
		  this->_fpga->fpga_red_led_on( i );
		
		} else {

		  this->_fpga->fpga_green_led_on( i );
		  this->_fpga->fpga_red_led_off( i );
		
		}
	  }
	} else {

	  this->_fpga->fpga_reset();
	  
	  while( !is_running ){

		if( current_select_mode ){

		  for( unsigned int i = 0; i < CHANNELS_NUMBER - 1; i ++ ){
		
			this->_fpga->fpga_red_led_off( i );
			this->_fpga->fpga_green_led_off( i );
		
		  }

		  for( unsigned int i = 0; i < static_cast< unsigned int >( current ); i ++ ) this->_fpga->fpga_green_led_on( i );

		  vTaskDelay( 100 );
		  this->_fpga->fpga_green_led_on( CHANNELS_NUMBER - 1 );
		  vTaskDelay( 100 );
		  this->_fpga->fpga_green_led_off( CHANNELS_NUMBER - 1 );
		  
		} else if( voltage_select_mode ){
		  
		  for( unsigned int i = 0; i < CHANNELS_NUMBER - 1; i ++ ){
		
			this->_fpga->fpga_red_led_off( i );
			this->_fpga->fpga_green_led_off( i );
		
		  }

		  for( unsigned int i = 0; i < static_cast< unsigned int >( voltage ); i ++ ) this->_fpga->fpga_red_led_on( i );

		  vTaskDelay( 100 );
		  this->_fpga->fpga_red_led_on( CHANNELS_NUMBER - 1 );
		  vTaskDelay( 100 );
		  this->_fpga->fpga_red_led_off( CHANNELS_NUMBER - 1 );

		} else if( mistake_select_mode ){

		  for( unsigned int i = 0; i < CHANNELS_NUMBER - 1; i ++ ){
		
			this->_fpga->fpga_red_led_off( i );
			this->_fpga->fpga_green_led_off( i );
		
		  }

		  for( unsigned int i = 0; i < static_cast< unsigned int >( mistake * 100.0f ); i ++ ){

			this->_fpga->fpga_red_led_on( i );
			this->_fpga->fpga_green_led_on( i );

		  }

		  vTaskDelay( 100 );
		  this->_fpga->fpga_red_led_on( CHANNELS_NUMBER - 1 );
		  this->_fpga->fpga_green_led_on( CHANNELS_NUMBER - 1 );
		  vTaskDelay( 100 );
		  this->_fpga->fpga_red_led_off( CHANNELS_NUMBER - 1 );
		  this->_fpga->fpga_green_led_off( CHANNELS_NUMBER - 1 );

		  
		} else __asm__( "nop" );
	  }

	  this->_fpga->fpga_reset();
	}
  }
  
  vTaskDelete( nullptr );
}

void core_task_wrapper( void * param_ptr )
{
  reinterpret_cast< core * >( param_ptr )->task( param_ptr );
}

void irq_controller_wrapper( void * param_ptr )
{
  reinterpret_cast< core * >( param_ptr )->irq_controller( param_ptr );
}

core::~core()
{
  __asm__( "nop" );
}

extern "C" void vApplicationTickHook()
{
  __asm__( "nop" );
}

extern "C" void vApplicationMallocFailedHook()
{
  __asm__( "nop" );
}

extern "C" void vApplicationIdleHook()
{
  __asm__( "nop" );
}

extern "C" void vApplicationStackOverflowHook( TaskHandle_t xTask, char * pcTaskName )
{
  __asm__( "nop" );
}
