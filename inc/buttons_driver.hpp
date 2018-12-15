#ifndef BUTTONS_DRIVER_HPP
#define BUTTONS_DRIVER_HPP

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "bsp_macro.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <string>
#include <map>
#include <functional>

class buttons_driver
{
public:

  explicit buttons_driver();
  virtual ~buttons_driver();

  bool is_minus_pressed();
  bool is_plus_pressed();
  bool is_run_pressed();
  
private:
  
  void buttons_driver_gpio_init();
  void buttons_driver_gpio_deinit();
  void buttons_driver_periph_init();
  void buttons_driver_periph_deinit();
  
};

extern QueueHandle_t buttons_irq_queue;

extern "C" void BUTTON_PLUS_IRQHANDLER();
extern "C" void BUTTON_RUN_MINUS_IRQHANDLER();
extern "C" void BUTTONS_TIM_IRQHANDLER();

#endif /* BUTTONS_DRIVER_HPP */
