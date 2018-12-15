#ifndef CURRENT_SOURCES_DRIVER_HPP
#define CURRENT_SOURCES_DRIVER_HPP

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "bsp_macro.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <map>
#include <functional>

class current_sources_driver
{
public:

  explicit current_sources_driver();
  virtual ~current_sources_driver();
    
  void current_source_set_voltage( double mvolt );
  void current_source_set_current( double milliamperes );
  void current_source_turn_off();
  void current_source_turn_on();
  
private:
  
  void current_sources_driver_gpio_init();
  void current_sources_driver_gpio_deinit();
  void current_sources_driver_periph_init();
  void current_sources_driver_periph_deinit();
    
protected:
  
};

#endif /* CURRENT_SOURCES_DRIVER_HPP */
