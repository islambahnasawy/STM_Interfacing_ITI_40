#include "../LIB/STD_TYPES.h"
#include "RTOS_interface.h"
#include "RTOS_private.h"
#include "RTOS_config.h"

#include "../Application/OS_Tasking_cfg/OS_Tasking_cfg.h"

/*Architect should configure the first delays(MS) in this array */
const baseTaskInfo_t baseTaskInfo[NUM_OF_TASKS] =
{
	{.task=&tasks[3],.firstdelay=0},
	{.task=&tasks[0],.firstdelay=0},
	{.task=&tasks[2],.firstdelay=0},
	{.task=&tasks[1],.firstdelay=0},
	{.task=&tasks[4],.firstdelay=0}
};
