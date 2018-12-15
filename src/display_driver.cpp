#include "display_driver.hpp"

double num = 0.0f;

display_driver::display_driver()
{
  this->display_driver_gpio_init();
  this->display_driver_periph_init();
}

void display_driver::display_driver_gpio_init()
{
  GPIO_InitTypeDef * GPIO_InitStruct = new GPIO_InitTypeDef;

  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin = DISPLAY_GPIO_DIG_1_PIN | DISPLAY_GPIO_DIG_2_PIN | DISPLAY_GPIO_DIG_3_PIN;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( DISPLAY_DIG_1_3_GPIO_PORT, GPIO_InitStruct );
  
  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin =  DISPLAY_GPIO_SEG_A_PIN | DISPLAY_GPIO_SEG_B_PIN | DISPLAY_GPIO_SEG_C_PIN | DISPLAY_GPIO_SEG_D_PIN | DISPLAY_GPIO_SEG_E_PIN | DISPLAY_GPIO_SEG_F_PIN | DISPLAY_GPIO_SEG_G_PIN;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( DISPLAY_SEG_DIG_0_GPIO_PORT, GPIO_InitStruct );

  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin =  DISPLAY_GPIO_SEG_A_PIN;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( DISPLAY_SEG_DIG_0_GPIO_PORT, GPIO_InitStruct );
  
  delete GPIO_InitStruct;
}

void display_driver::display_driver_gpio_deinit()
{
  GPIO_InitTypeDef * GPIO_InitStruct = new GPIO_InitTypeDef;

  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin = DISPLAY_GPIO_DIG_1_PIN | DISPLAY_GPIO_DIG_2_PIN | DISPLAY_GPIO_DIG_3_PIN;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( DISPLAY_DIG_1_3_GPIO_PORT, GPIO_InitStruct );
  
  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin =  DISPLAY_GPIO_SEG_A_PIN | DISPLAY_GPIO_SEG_B_PIN | DISPLAY_GPIO_SEG_C_PIN | DISPLAY_GPIO_SEG_D_PIN | DISPLAY_GPIO_SEG_E_PIN | DISPLAY_GPIO_SEG_F_PIN | DISPLAY_GPIO_SEG_G_PIN;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( DISPLAY_SEG_DIG_0_GPIO_PORT, GPIO_InitStruct );

  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin =  DISPLAY_GPIO_SEG_A_PIN;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( DISPLAY_SEG_DIG_0_GPIO_PORT, GPIO_InitStruct );
  
  delete GPIO_InitStruct;
}

void display_driver::display_driver_periph_init()
{
  TIM_TimeBaseInitTypeDef * TIM_TimeBaseInitStruct = new TIM_TimeBaseInitTypeDef;

  TIM_TimeBaseStructInit( TIM_TimeBaseInitStruct );
  TIM_TimeBaseInitStruct->TIM_ClockDivision = 1000;
  TIM_TimeBaseInitStruct->TIM_Prescaler = 1000;
  TIM_TimeBaseInitStruct->TIM_Period = 1000;
  TIM_TimeBaseInitStruct->TIM_CounterMode = TIM_CounterMode_Up;
  TIM_ITConfig( DISPLAY_TIM_PERIPH, TIM_IT_Update, ENABLE );
  TIM_TimeBaseInit( DISPLAY_TIM_PERIPH, TIM_TimeBaseInitStruct );
  TIM_Cmd( DISPLAY_TIM_PERIPH, ENABLE );

  NVIC_SetPriority( DISPLAY_TIM_IRQ, 12 );
  NVIC_EnableIRQ( DISPLAY_TIM_IRQ );  
  
  delete TIM_TimeBaseInitStruct;
}

void display_driver::display( double number )
{
  std::string * str_buf = new std::string;
  std::sprintf( const_cast< char * >( str_buf->c_str() ), "%lf", number );
  delete str_buf;
}

void display_driver::display_driver_periph_deinit()
{
  TIM_DeInit( DISPLAY_TIM_PERIPH );
  NVIC_DisableIRQ( DISPLAY_TIM_IRQ );
}

display_driver::~display_driver()
{
  this->display_driver_gpio_deinit();
  this->display_driver_periph_deinit();
}

extern "C" void DISPLAY_TIM_IRQHANDLER()
{
  TIM_ClearITPendingBit( DISPLAY_TIM_PERIPH, TIM_IT_Update );
  if( TIM_GetITStatus( DISPLAY_TIM_PERIPH, TIM_IT_Update )){

	TIM_ClearITPendingBit( DISPLAY_TIM_PERIPH, TIM_IT_Update );
	portBASE_TYPE display_driver_task_irq_woken;
	
  } else TIM_ClearITPendingBit( DISPLAY_TIM_PERIPH, TIM_IT_Update | TIM_IT_Break | TIM_IT_Trigger | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 | TIM_IT_COM );
}
