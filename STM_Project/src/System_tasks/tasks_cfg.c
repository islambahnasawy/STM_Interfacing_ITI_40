#include "../LIB/STD_TYPES.h"

#include "../RTOS/RTOS_interface.h"
#include "../HAL/LCD/C_LCD_interface.h"
#include "../HAL/Switch/Switch_interface.h"
#include "../Application/App1_task/App1_interface.h"
#include "tasks_cfg.h"
#include "../Other_Drivers/LED/LED_interface.h"




/*Architect should configure the first delays(MS) and priority in this array */
baseTaskInfo_t baseTaskInfo[NUM_OF_TASKS] ;

void TaskingInit(void)
{

	baseTaskInfo[0].task=&UpdatingSwitchesTask;
	baseTaskInfo[0].firstdelay=0;

	baseTaskInfo[1].task=&App1_task;
	baseTaskInfo[1].firstdelay=0;

	baseTaskInfo[2].task=&LCD_Task;
	baseTaskInfo[2].firstdelay=0;
}

