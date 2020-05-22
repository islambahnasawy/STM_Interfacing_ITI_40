#include "../LIB/STD_TYPES.h"


#include "../RTOS/RTOS_interface.h"
#include "../HAL/LCD/C_LCD_interface.h"
#include "../HAL/Switch/Switch_interface.h"

#include "tasks_cfg.h"


/*Architect should configure the first delays(MS) and priority in this array */
baseTaskInfo_t baseTaskInfo[NUM_OF_TASKS] ;

void TaskingInit(void)
{


}

