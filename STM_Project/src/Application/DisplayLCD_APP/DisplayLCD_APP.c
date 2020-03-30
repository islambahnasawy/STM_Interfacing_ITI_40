#include "../../LIB/STD_TYPES.h"

#include "../../RTOS/RTOS_interface.h"
#include "../../HAL/LCD/C_LCD_interface.h"


task_t LCD_Task = {.handler=LCD_OS_runnable,.preodicityMS=5};
