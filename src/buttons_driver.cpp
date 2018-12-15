#include "buttons_driver.hpp"

QueueHandle_t buttons_irq_queue;

buttons_driver::buttons_driver()
{
#define BUTTONS_QUEUE_LENGTH 64
  buttons_irq_queue = xQueueCreate( BUTTONS_QUEUE_LENGTH, sizeof( char ));
  this->buttons_driver_gpio_init();
  this->buttons_driver_periph_init();
}

void buttons_driver::buttons_driver_gpio_init()
{
  GPIO_InitTypeDef * GPIO_InitStruct = new GPIO_InitTypeDef;

  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin = BUTTON_MINUS_GPIO_PIN | BUTTON_PLUS_GPIO_PIN | BUTTON_RUN_GPIO_PIN;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( BUTTONS_GPIO_PORT, GPIO_InitStruct );
  
  delete GPIO_InitStruct;
}

void buttons_driver::buttons_driver_gpio_deinit()
{
  GPIO_InitTypeDef * GPIO_InitStruct = new GPIO_InitTypeDef;

  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin = BUTTON_MINUS_GPIO_PIN | BUTTON_PLUS_GPIO_PIN | BUTTON_RUN_GPIO_PIN;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( BUTTONS_GPIO_PORT, GPIO_InitStruct );
  
  delete GPIO_InitStruct;
}

void buttons_driver::buttons_driver_periph_init()
{
  EXTI_InitTypeDef * EXTI_InitStruct = new EXTI_InitTypeDef;
  TIM_TimeBaseInitTypeDef * TIM_TimeBaseInitStruct = new TIM_TimeBaseInitTypeDef;
  
  EXTI_StructInit( EXTI_InitStruct );
  EXTI_InitStruct->EXTI_Line = BUTTON_MINUS_EXTI_LINE | BUTTON_PLUS_EXTI_LINE | BUTTON_RUN_EXTI_LINE;
  EXTI_InitStruct->EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct->EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStruct->EXTI_LineCmd = ENABLE;
  EXTI_ClearITPendingBit( BUTTON_MINUS_EXTI_LINE | BUTTON_PLUS_EXTI_LINE | BUTTON_RUN_EXTI_LINE );
  EXTI_Init( EXTI_InitStruct );
  
  GPIO_EXTILineConfig( BUTTONS_GPIO_PORTSOURCE, BUTTON_MINUS_GPIO_PINSOURCE );
  GPIO_EXTILineConfig( BUTTONS_GPIO_PORTSOURCE, BUTTON_PLUS_GPIO_PINSOURCE );
  GPIO_EXTILineConfig( BUTTONS_GPIO_PORTSOURCE, BUTTON_RUN_GPIO_PINSOURCE );

  TIM_TimeBaseStructInit( TIM_TimeBaseInitStruct );
  TIM_TimeBaseInitStruct->TIM_ClockDivision = 1000;
  TIM_TimeBaseInitStruct->TIM_Prescaler = 1000;
  TIM_TimeBaseInitStruct->TIM_Period = 1000;
  TIM_TimeBaseInitStruct->TIM_CounterMode = TIM_CounterMode_Up;
  TIM_ITConfig( BUTTONS_TIM_PERIPH, TIM_IT_Update, ENABLE );
  TIM_TimeBaseInit( BUTTONS_TIM_PERIPH, TIM_TimeBaseInitStruct );
  TIM_Cmd( BUTTONS_TIM_PERIPH, ENABLE );

  NVIC_SetPriority( BUTTON_RUN_PLUS_IRQ, 12 );
  NVIC_SetPriority( BUTTON_MINUS_IRQ, 12 );
  NVIC_SetPriority( BUTTONS_TIM_IRQ, 12 );
  
  NVIC_EnableIRQ( BUTTON_RUN_PLUS_IRQ );
  NVIC_EnableIRQ( BUTTON_MINUS_IRQ );
  NVIC_EnableIRQ( BUTTONS_TIM_IRQ );
  
  delete EXTI_InitStruct;  
  delete TIM_TimeBaseInitStruct;
}

void buttons_driver::buttons_driver_periph_deinit()
{
  EXTI_DeInit();
  NVIC_DisableIRQ( BUTTON_RUN_PLUS_IRQ );
  NVIC_DisableIRQ( BUTTON_MINUS_IRQ );
  NVIC_DisableIRQ( BUTTONS_TIM_IRQ );
}

bool buttons_driver::is_minus_pressed()
{
  return ( GPIO_ReadInputDataBit( BUTTONS_GPIO_PORT, BUTTON_MINUS_GPIO_PIN )) ? false : true;
}

bool buttons_driver::is_plus_pressed()
{
  return ( GPIO_ReadInputDataBit( BUTTONS_GPIO_PORT, BUTTON_PLUS_GPIO_PIN )) ? false : true;
}

bool buttons_driver::is_run_pressed()
{
  return ( GPIO_ReadInputDataBit( BUTTONS_GPIO_PORT, BUTTON_RUN_GPIO_PIN )) ? false : true;
}

buttons_driver::~buttons_driver()
{
  this->buttons_driver_gpio_deinit();
  this->buttons_driver_periph_deinit();
}

extern "C" void BUTTON_MINUS_IRQHANDLER()
{  
  if( EXTI_GetITStatus( BUTTON_MINUS_EXTI_LINE )){

	EXTI_ClearITPendingBit( BUTTON_MINUS_EXTI_LINE );
	portBASE_TYPE buttons_driver_task_irq_woken;
	const char msg = '-';
	xQueueSendToBackFromISR( buttons_irq_queue, &msg, &buttons_driver_task_irq_woken );
	if( buttons_driver_task_irq_woken == pdTRUE ) taskYIELD();
	
  } else EXTI_ClearITPendingBit( BUTTON_MINUS_EXTI_LINE | BUTTON_PLUS_EXTI_LINE | BUTTON_RUN_EXTI_LINE );
}

extern "C" void BUTTON_RUN_PLUS_IRQHANDLER()
{
  if( EXTI_GetITStatus( BUTTON_PLUS_EXTI_LINE )){

	EXTI_ClearITPendingBit( BUTTON_PLUS_EXTI_LINE );
	portBASE_TYPE buttons_driver_task_irq_woken;
	const char msg = '+';
	xQueueSendToBackFromISR( buttons_irq_queue, &msg, &buttons_driver_task_irq_woken );
	if( buttons_driver_task_irq_woken == pdTRUE ) taskYIELD();
	
  } else if( EXTI_GetITStatus( BUTTON_RUN_EXTI_LINE )){

	EXTI_ClearITPendingBit( BUTTON_RUN_EXTI_LINE );
	portBASE_TYPE buttons_driver_task_irq_woken;
	const char msg = '=';
	xQueueSendToBackFromISR( buttons_irq_queue, &msg, &buttons_driver_task_irq_woken );
	if( buttons_driver_task_irq_woken == pdTRUE ) taskYIELD();

  } else EXTI_ClearITPendingBit( BUTTON_MINUS_EXTI_LINE | BUTTON_PLUS_EXTI_LINE | BUTTON_RUN_EXTI_LINE );  
}

extern "C" void BUTTONS_TIM_IRQHANDLER()
{
  if( TIM_GetITStatus( BUTTONS_TIM_PERIPH, TIM_IT_Update )){

	TIM_ClearITPendingBit( BUTTONS_TIM_PERIPH, TIM_IT_Update );
	
  } else TIM_ClearITPendingBit( BUTTONS_TIM_PERIPH, TIM_IT_Update | TIM_IT_Break | TIM_IT_Trigger | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 | TIM_IT_COM );
}
