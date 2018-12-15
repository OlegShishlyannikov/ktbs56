#ifndef DISPLAY_DRIVER_HPP
#define DISPLAY_DRIVER_HPP

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "bsp_macro.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <map>
#include <string>
#include <functional>

class display_driver
{

public:

  explicit display_driver();
  virtual ~display_driver();
  void display( double number );
  
private:
  
  void display_driver_gpio_init();
  void display_driver_gpio_deinit();
  void display_driver_periph_init();
  void display_driver_periph_deinit();

protected:
  
};

extern "C" void DISPLAY_TIM_IRQHANDLER();

#endif /* DISPLAY_DRIVER_HPP */
