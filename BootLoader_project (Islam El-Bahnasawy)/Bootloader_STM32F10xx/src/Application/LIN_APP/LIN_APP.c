#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/USART/USART_MYTYPES.h"

#include "../../RTOS/RTOS_interface.h"
#include "../../HAL/LIN/LIN_interface.h"
#include "../../HAL/LCD/C_LCD_interface.h"
#include "../../HAL/Switch/Switch_interface.h"
#include "LIN_APP.h"
#include <stdio.h>

void LIN_APP_task(void);

const task_t LIN_demo = {LIN_APP_task,50};

void LIN_APP_task(void)
{
	u8 data = 12;
	setSDU(0x35,&data);
	suspend_Task();
}
