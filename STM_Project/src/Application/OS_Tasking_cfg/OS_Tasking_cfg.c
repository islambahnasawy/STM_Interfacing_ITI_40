#include "../../LIB/STD_TYPES.h"

#include "../../RTOS/RTOS_interface.h"
#include "../ReceivingUSART_App/ReceivingUSART_App.h"
#include "../SendingUSART_App/SendingUSART_App.h"
#include "../Switch_IncCounter_APP/Switch_IncCounter_APP.h"
#include "../DisplayLCD_APP/DisplayLCD_APP.h"
#include "OS_Tasking_cfg.h"

//should this file be in the application layer or the OS layer
task_t tasks[NUM_OF_TASKS] ;

/*adding the tasks in the tasks array*/
void OS_TaskingInit(void)
{
	tasks[0]=RecTask;
	tasks[1]=TransTask;
	tasks[2]=CounterTask;
	tasks[3]=UpdatingSwitchesTask;
	tasks[4]=LCD_Task;
}
