#include "usart_driver.hpp"

QueueHandle_t usart_irq_queue;

usart_driver::usart_driver()
{
#define USART_IRQ_QUEUE_LENGTH 64
  usart_irq_queue = xQueueCreate( USART_IRQ_QUEUE_LENGTH, sizeof( char ));
  this->usart_driver_gpio_init();
  this->usart_driver_periph_init();
}

void usart_driver::usart_driver_gpio_init()
{
  GPIO_InitTypeDef * GPIO_InitStruct = new GPIO_InitTypeDef;
  
  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin = USART_GPIO_TX_PIN;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( USART_GPIO_PORT, GPIO_InitStruct );

  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin = USART_GPIO_RX_PIN;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( USART_GPIO_PORT, GPIO_InitStruct );

  delete GPIO_InitStruct;
}

void usart_driver::usart_driver_gpio_deinit()
{
  GPIO_InitTypeDef * GPIO_InitStruct = new GPIO_InitTypeDef;
  
  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin = USART_GPIO_TX_PIN;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( USART_GPIO_PORT, GPIO_InitStruct );

  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin = USART_GPIO_RX_PIN;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( USART_GPIO_PORT, GPIO_InitStruct );

  delete GPIO_InitStruct;
}

void usart_driver::usart_driver_periph_init()
{
  USART_InitTypeDef * USART_InitStruct = new USART_InitTypeDef;
 
  USART_StructInit( USART_InitStruct );
  USART_InitStruct->USART_BaudRate = 115200;
  USART_InitStruct->USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct->USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStruct->USART_Parity = USART_Parity_No;
  USART_InitStruct->USART_StopBits = USART_StopBits_1;
  USART_InitStruct->USART_WordLength = USART_WordLength_8b;
  USART_ITConfig( USART_PERIPH, USART_IT_RXNE, ENABLE );
  USART_Init( USART_PERIPH, USART_InitStruct );
  USART_Cmd( USART_PERIPH, ENABLE );
  
  NVIC_SetPriority( USART_IRQ, 12 );
  NVIC_EnableIRQ( USART_IRQ );
  
  delete USART_InitStruct;  
}

void usart_driver::usart_driver_periph_deinit()
{
  USART_DeInit( USART_PERIPH );  
}

void usart_driver::send_msg( std::string str_fmt, ... )
{
  std::va_list arg;
  char * temp_str_ptr = new char[ 1024 ];
  va_start( arg, str_fmt );
  std::vsprintf( temp_str_ptr, str_fmt.c_str(), arg );
  va_end( arg );

  for( unsigned int i = 0; i < std::strlen( temp_str_ptr ); i ++ ){

	while( !USART_GetFlagStatus( USART_PERIPH, USART_FLAG_TC ));
	USART_SendData( USART_PERIPH, temp_str_ptr[ i ]);
	
  }  
  delete[] temp_str_ptr;
}

usart_driver::~usart_driver()
{
  this->usart_driver_gpio_deinit();
  this->usart_driver_periph_deinit();
}

extern "C" void USART_IRQHANDLER()
{
  if( USART_GetITStatus(USART_PERIPH, USART_IT_RXNE )){
	
	char temp = USART_ReceiveData( USART_PERIPH );
	portBASE_TYPE usart_driver_task_irq_woken;
	xQueueSendToBackFromISR( usart_irq_queue, &temp, &usart_driver_task_irq_woken );
	if( usart_driver_task_irq_woken == pdTRUE ) taskYIELD();
	
  } else USART_ClearITPendingBit( USART_PERIPH, USART_IT_RXNE | USART_IT_TC | USART_IT_TXE );
}  
