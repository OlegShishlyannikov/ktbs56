#ifndef BASIC_TASK_HPP
#define BASIC_TASK_HPP

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

class basic_task
{
public:
  
  explicit basic_task(){ __asm__( "nop" ); };
  virtual ~basic_task(){ __asm__( "nop" ); };
  
  typedef struct {

	/* Usart driver globals */
	uint32_t stack_size;
	uint32_t task_priority;
	std::string * task_name_ptr;

  } task_settings;
  
private:
  
protected:
  
};

#endif /* BASIC_TASK_HPP */
