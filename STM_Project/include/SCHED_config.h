#ifndef SCHED_CONFIG_H
#define SCHED_CONFIG_H

#define TASKS_NUM 3
#define TICKS_MS 5

typedef struct
{
	task_t * appTask;
	u32 first_Delay_Tick;

}BaseTaskInfo_t;

#endif
