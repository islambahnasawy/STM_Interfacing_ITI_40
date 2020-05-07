#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/USART/USART_MYTYPES.h"

#include "../../RTOS/RTOS_interface.h"
#include "../../HAL/LIN/LIN_interface.h"
#include "../../HAL/LCD/C_LCD_interface.h"
#include "../../HAL/Switch/Switch_interface.h"
#include "LIN_APP.h"
#include <stdio.h>

void LIN_APP_task(void);

const task_t LIN_demo = {LIN_APP_task,10};

void LIN_APP_task(void)
{
	u8 data[3];
	u8 led[2]= {15,19};
	getSDU(0x35,data);
	if(data[0] == 12)
	{
		LCD_OS_write("hamada");
	}
	getSDU(0x3,data);
	if(data[1] == 'b')
	{
		LCD_OS_write("3aash");
	}
	setSDU(0x13, led);
	//suspend_Task();
}
