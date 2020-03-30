#include "../../LIB/STD_TYPES.h"

#include "../../RTOS/RTOS_interface.h"
#include "../../HAL/Switch/Switch_interface.h"

static void updatingCounter(void);
u32 TransValue;

task_t CounterTask = {.handler=updatingCounter,.preodicityMS=30};
task_t UpdatingSwitchesTask = {.handler=Switch_updateValue,.preodicityMS=5};

//should all the switches be impelemented in this file or every switch should have its own file?

static void updatingCounter(void)
{

	static u8 sw_flag=0;

	if(getSwitchState(SWITCH1)==SWITCH_PRESSED )
	{
		/*Incerement once for long press*/
		if(sw_flag == 0)
		{
			sw_flag =1 ;
			TransValue ++;
		}
	}
	else
	{
		sw_flag = 0 ;
	}
}







