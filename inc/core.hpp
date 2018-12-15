#ifndef CORE_HPP
#define CORE_HPP

#include "usart_driver.hpp"
#include "fpga_driver.hpp"
#include "current_sources_driver.hpp"
#include "adc_spi_driver.hpp"
#include "display_driver.hpp"
#include "buttons_driver.hpp"

#include "startup.hpp"

class core
{
public:
  
  explicit core();
  virtual ~core();

  void load_drivers();
  void run();
  void task( void * param_ptr );
  void irq_controller( void * param_ptr );
  
private:

  fpga_driver * _fpga;
  usart_driver * _usart;
  current_sources_driver * _current_sources;
  adc_spi_driver * _adc_spi;
  display_driver * _display;
  buttons_driver * _buttons;

protected:
  
};

extern TaskHandle_t core_task_handle;
extern TaskHandle_t irq_controller_handle;

extern "C" void core_task_wrapper( void * param_ptr );
extern "C" void irq_controller_wrapper( void * param_ptr );

#endif /* CORE_HPP */
