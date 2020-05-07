#include "../LIB/STD_TYPES.h"
#include "../MCAL/USART/USART_MYTYPES.h"

#include "../RTOS/RTOS_interface.h"
#include "../HAL/LCD/C_LCD_interface.h"
#include "../HAL/Switch/Switch_interface.h"
#include "../Application/App1_task/App1_interface.h"
#include "tasks_cfg.h"
#include "../Other_Drivers/LED/LED_interface.h"
#include "../Application/Watch_App/Watch_APP_interface.h"
#include "../HAL/LIN/LIN_interface.h"

#include "../Application/LIN_APP/LIN_APP.h"

/*Architect should configure the first delays(MS) and priority in this array */
baseTaskInfo_t baseTaskInfo[NUM_OF_TASKS] ;

void TaskingInit(void)
{

	/*
	baseTaskInfo[0].task=&UpdatingSwitchesTask;
	baseTaskInfo[0].firstdelay=0;

	baseTaskInfo[1].task=&Watch_App_task;
	baseTaskInfo[1].firstdelay=0;

	baseTaskInfo[2].task=&LCD_Task;
	baseTaskInfo[2].firstdelay=0;
	*/

	baseTaskInfo[0].task=&Master_task;
	baseTaskInfo[0].firstdelay=0;

	baseTaskInfo[1].task=&Slave_task;
	baseTaskInfo[1].firstdelay=0;

	baseTaskInfo[2].task=&LCD_Task;
	baseTaskInfo[2].firstdelay=0;

	baseTaskInfo[3].task=&LIN_demo;
	baseTaskInfo[3].firstdelay=0;
}

