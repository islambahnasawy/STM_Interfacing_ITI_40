#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "SCHED_interface.h"
#include "SCHED_config.h"


extern const task_t Tasks[TASKS_NUM];

const BaseTaskInfo_t SYS_Tasks_Info[TASKS_NUM] =
{{.appTask = &Tasks[0], .first_Delay_Tick = 10},
{.appTask = &Tasks[1], .first_Delay_Tick = 4},
{.appTask = &Tasks[2], .first_Delay_Tick = 7}};



