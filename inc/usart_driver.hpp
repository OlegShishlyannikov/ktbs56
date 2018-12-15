#ifndef USART_DRIVER_HPP
#define USART_DRIVER_HPP

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

class usart_driver
{
public:

  explicit usart_driver();
  virtual ~usart_driver();
  void send_msg( std::string str_fmt, ... );  

private:

  void usart_driver_gpio_init();
  void usart_driver_gpio_deinit();
  void usart_driver_periph_init();
  void usart_driver_periph_deinit();
};

extern QueueHandle_t usart_irq_queue;
extern "C" void USART_IRQHANDLER();

#endif /* USART_DRIVER_HPP */
