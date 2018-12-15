#ifndef STARTUP_HPP
#define STARTUP_HPP

#include <stdint.h>
#include <sys/types.h>
#include <functional>
#include "stm32f10x_conf.h"

#define OS_INCLUDE_STARTUP_GUARD_CHECKS ( 1 )

/* Defined from ld-script */
extern unsigned int _sidata;
extern unsigned int _sdata;
extern unsigned int _edata;
extern unsigned int __bss_start__;
extern unsigned int __bss_end__;
extern unsigned int _estack;

/* Application entry-point */
extern int main( int argc, char * argv[] );

void __initialize_args( int *, char *** );
void _exit( int );

/* Entry point */
void _start( void );

/* Fill SRAM data section from flash */
void __initialize_data( unsigned int * from, unsigned int * section_begin, unsigned int * section_end );

/* Fill zero-bss constants */
void __initialize_bss( unsigned int * section_begin, unsigned int * section_end );

/* Run c++ constructors */
void __run_init_array( void );

/* Run c++ destructors */
void __run_fini_array( void );

/* Init hardware function prototype */
void __initialize_rcc( void );

/* Irq handlers */
extern "C" void __attribute__(( weak )) Reset_Handler( void ){ _start(); };
extern "C" void __attribute__(( weak )) NMI_Handler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) HardFault_Handler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) MemManage_Handler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) BusFault_Handler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) UsageFault_Handler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) SVC_Handler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) DebugMon_Handler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) PendSV_Handler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) SysTick_Handler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) WWDG_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) PVD_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) TAMPER_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) FLASH_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) RCC_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) EXTI0_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) EXTI1_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) EXTI2_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) EXTI3_IRQHandler( void ){ while( true ); }
extern "C" void __attribute__(( weak )) EXTI4_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) DMA1_Channel1_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) DMA1_Channel2_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) DMA1_Channel3_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) DMA1_Channel4_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) DMA1_Channel5_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) DMA1_Channel6_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) DMA1_Channel7_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) ADC1_2_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) USB_HP_CAN1_TX_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) USB_LP_CAN1_RX0_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) CAN1_RX1_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) CAN1_SCE_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) EXTI9_5_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) TIM1_BRK_TIM9_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) TIM1_UP_TIM10_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) TIM1_TRG_COM_TIM11_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) TIM1_CC_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) TIM2_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) TIM3_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) TIM4_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) I2C1_EV_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) I2C1_ER_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) I2C2_EV_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) I2C2_ER_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) SPI1_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) SPI2_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) USART1_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) USART2_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) USART3_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) EXTI15_10_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) RTCAlarm_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) RTC_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) USBWakeUp_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) TIM8_BRK_TIM12_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) TIM8_UP_TIM13_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) TIM8_TRG_COM_TIM14_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) TIM8_CC_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) ADC3_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) FSMC_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) SDIO_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) TIM5_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) SPI3_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) UART4_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) UART5_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) TIM6_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) TIM7_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) DMA2_Channel1_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) DMA2_Channel2_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) DMA2_Channel3_IRQHandler( void ){ while( true ); };
extern "C" void __attribute__(( weak )) DMA2_Channel4_5_IRQHandler( void ){ while( true ); };

#endif /* STARTUP_HPP */
