#include "startup.hpp"

inline void __attribute__(( always_inline )) __initialize_data( unsigned int * from, unsigned int * section_begin, unsigned int * section_end )
{
  unsigned int * p = section_begin;
  while( p < section_end ) * p ++ = * from ++;
}

inline void __attribute__(( always_inline )) __initialize_bss( unsigned int * section_begin, unsigned int * section_end )
{
  unsigned int * p = section_begin;
  while( p < section_end ) * p ++ = 0;
}

extern void ( * __preinit_array_start[] )( void ) __attribute__(( weak ));
extern void ( * __preinit_array_end[] )( void ) __attribute__(( weak ));
extern void ( * __init_array_start[] )( void ) __attribute__(( weak ));
extern void ( * __init_array_end[] )( void ) __attribute__(( weak ));
extern void ( * __fini_array_start[] )( void ) __attribute__(( weak ));
extern void ( * __fini_array_end[] )( void ) __attribute__(( weak ));

inline void __attribute__(( always_inline )) __run_init_array( void )
{
  int count;
  int i;
  count = __preinit_array_end - __preinit_array_start;
  
  for( i = 0; i < count; i ++ ) __preinit_array_start[ i ]();

  count = __init_array_end - __init_array_start;

  for (i = 0; i < count; i++) __init_array_start[i]();
}

inline void __attribute__(( always_inline )) __run_fini_array( void )
{
  int count;
  int i;
  count = __fini_array_end - __fini_array_start;

  for( i = count; i > 0; i -- ) __fini_array_start[ i - 1 ]();
}

#if defined( DEBUG ) && defined( OS_INCLUDE_STARTUP_GUARD_CHECKS )

#define BSS_GUARD_BAD_VALUE ( 0xCADEBABA )

static unsigned int volatile __attribute__ (( section( ".bss_begin" ))) __bss_begin_guard;
static unsigned int volatile __attribute__ (( section( ".bss_end" ))) __bss_end_guard;

#define DATA_GUARD_BAD_VALUE ( 0xCADEBABA )
#define DATA_BEGIN_GUARD_VALUE ( 0x12345678 )
#define DATA_END_GUARD_VALUE ( 0x98765432 )

static unsigned int volatile __attribute__ (( section( ".data_begin" ))) __data_begin_guard = DATA_BEGIN_GUARD_VALUE;
static unsigned int volatile __attribute__ (( section( ".data_end" ))) __data_end_guard = DATA_END_GUARD_VALUE;

#endif /* defined( DEBUG ) && defined( OS_INCLUDE_STARTUP_GUARD_CHECKS ) */

void __initialize_rcc( void )
{
  RCC_DeInit(); /* Clear prevous RCC configuration */
  // RCC_HSEConfig( RCC_HSE_ON ); /* Enable HSE */
  // RCC_WaitForHSEStartUp(); /* Wait for HSE startup */
  // RCC_HSICmd( DISABLE ); /* Disable HSI */
  RCC_PLLConfig( RCC_PLLSource_HSI_Div2, 16 ); /* Setup HSE */
  RCC_PLLCmd( ENABLE ); /* Enable PLL */

  while( !RCC_GetFlagStatus( RCC_FLAG_PLLRDY )); /* Wait for PLL startup */

  RCC_SYSCLKConfig( RCC_SYSCLKSource_PLLCLK ); /* Set SYSCLK clock source */

  while( RCC_GetSYSCLKSource() != RCC_CFGR_SWS_PLL ); /* Wait for sysclk source */
  
  RCC_HCLKConfig( RCC_SYSCLK_Div1 ); /* Set HCLK clock source */
  RCC_PCLK1Config( RCC_HCLK_Div2 ); /* Set PCLK1 clock source */
  RCC_PCLK2Config( RCC_HCLK_Div1 ); /* Set PCLK2 clock source */

  /* Enable peripherial clocking */
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_DAC | RCC_APB1Periph_PWR | RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE );
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1 | RCC_APB2Periph_SPI1, ENABLE );
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1, ENABLE );
  SystemCoreClockUpdate();
}

void __initialize_args( int *, char *** )
{
  __asm__( "nop" );
}

void _exit( int )
{
  __asm__( "nop" );
}

void __attribute__ (( used, section( ".after_vectors" ), noreturn )) _start( void )
{

#if defined( DEBUG ) && defined( OS_INCLUDE_STARTUP_GUARD_CHECKS )
  __bss_begin_guard = BSS_GUARD_BAD_VALUE;
  __bss_end_guard = BSS_GUARD_BAD_VALUE;
#endif

  __initialize_bss( &__bss_start__, &__bss_end__ );

#if defined( DEBUG ) && defined( OS_INCLUDE_STARTUP_GUARD_CHECKS )
  if(( __bss_begin_guard != 0 ) || ( __bss_end_guard != 0 )){
	
	while( true );
	
  }
#endif

#if defined( DEBUG ) && defined( OS_INCLUDE_STARTUP_GUARD_CHECKS )
  __data_begin_guard = DATA_GUARD_BAD_VALUE;
  __data_end_guard = DATA_GUARD_BAD_VALUE;
#endif

  __initialize_data( &_sidata, &_sdata, &_edata);

#if defined( DEBUG ) && defined( OS_INCLUDE_STARTUP_GUARD_CHECKS )
  if(( __data_begin_guard != DATA_BEGIN_GUARD_VALUE )){
	
	while( true );
	
  }
#endif
  
  __initialize_rcc();
  int argc;
  char ** argv;
  __initialize_args( &argc, &argv );
  __run_init_array();
  int code = main( argc, argv );
  __run_fini_array();
  _exit( code );
  while( true );
}

void __attribute__(( used, section( ".isr_vector" ))) ( * const g_pfnVectors[] )( void ) = {

  reinterpret_cast< void ( * )( void )>( &_estack ),
  Reset_Handler,
  NMI_Handler,
  HardFault_Handler,
  MemManage_Handler,
  BusFault_Handler,
  UsageFault_Handler,
  0,
  0,
  0,
  0,
  SVC_Handler,
  DebugMon_Handler,
  0,
  PendSV_Handler,
  SysTick_Handler,
  WWDG_IRQHandler,
  PVD_IRQHandler,
  TAMPER_IRQHandler,
  RTC_IRQHandler,
  FLASH_IRQHandler,
  RCC_IRQHandler,
  EXTI0_IRQHandler,
  EXTI1_IRQHandler,
  EXTI2_IRQHandler,
  EXTI3_IRQHandler,
  EXTI4_IRQHandler,
  DMA1_Channel1_IRQHandler,
  DMA1_Channel2_IRQHandler,
  DMA1_Channel3_IRQHandler,
  DMA1_Channel4_IRQHandler,
  DMA1_Channel5_IRQHandler,
  DMA1_Channel6_IRQHandler,
  DMA1_Channel7_IRQHandler,
  ADC1_2_IRQHandler,
  USB_HP_CAN1_TX_IRQHandler,
  USB_LP_CAN1_RX0_IRQHandler,
  CAN1_RX1_IRQHandler,
  CAN1_SCE_IRQHandler,
  EXTI9_5_IRQHandler,
  TIM1_BRK_TIM9_IRQHandler,
  TIM1_UP_TIM10_IRQHandler,
  TIM1_TRG_COM_TIM11_IRQHandler,
  TIM1_CC_IRQHandler,
  TIM2_IRQHandler,
  TIM3_IRQHandler,
  TIM4_IRQHandler,
  I2C1_EV_IRQHandler,
  I2C1_ER_IRQHandler,
  I2C2_EV_IRQHandler,
  I2C2_ER_IRQHandler,
  SPI1_IRQHandler,
  SPI2_IRQHandler,
  USART1_IRQHandler,
  USART2_IRQHandler,
  USART3_IRQHandler,
  EXTI15_10_IRQHandler,
  RTCAlarm_IRQHandler,
  USBWakeUp_IRQHandler,
  TIM8_BRK_TIM12_IRQHandler,
  TIM8_UP_TIM13_IRQHandler,
  TIM8_TRG_COM_TIM14_IRQHandler,
  TIM8_CC_IRQHandler,
  ADC3_IRQHandler,
  FSMC_IRQHandler,
  SDIO_IRQHandler,
  TIM5_IRQHandler,
  SPI3_IRQHandler,
  UART4_IRQHandler,
  UART5_IRQHandler,
  TIM6_IRQHandler,
  TIM7_IRQHandler,
  DMA2_Channel1_IRQHandler,
  DMA2_Channel2_IRQHandler,
  DMA2_Channel3_IRQHandler,
  DMA2_Channel4_5_IRQHandler,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  reinterpret_cast< void ( * )( void )>( 0xF1E0F85F )
  
};
