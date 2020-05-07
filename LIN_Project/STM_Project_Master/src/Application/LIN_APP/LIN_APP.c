/********************************************************/
/*Author : Mohamed Megahed ,Islam El-Bahnasawy          */
/*Date   : 5 May 2020									*/
/*Version : V01											*/
/********************************************************/

#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/USART/USART_MYTYPES.h"

#include "../../RTOS/RTOS_interface.h"
#include "../../HAL/LIN/LIN_interface.h"
#include "../../HAL/LCD/C_LCD_interface.h"
#include "../../Other_Drivers/LED/LED_interface.h"
#include "../../HAL/Switch/Switch_interface.h"
#include "LIN_APP.h"
#include <stdio.h>

void LIN_APP_task(void);

const task_t LIN_demo = {LIN_APP_task,1000};

void LIN_APP_task(void)
{
	u8 data[3] = {12,20,40};
	u8 x[]={'a','b'};
	u8 led[2];
	setSDU(0x35,&data);
	setSDU(0x3,&x);
	getSDU(0x13,led);
	//LED_setLedState(LED2,LED_STATE_ON);
	if(led[0] == 15)
	{
		LED_setLedState(LED2,LED_STATE_ON);
	}
	if(led[1]==19)
	{
		LED_setLedState(LED1,LED_STATE_ON);
	}
	//suspend_Task();
}
