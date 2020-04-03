#ifndef _RTOS_CONFIG_H_
#define _RTOS_CONFIG_H_

#define NUM_OF_TASKS 	3
#define TICKTIME_MS		5

//extern task_t tasks[];

typedef struct
{
	task_t* task;
	u32 firstdelay;
}baseTaskInfo_t;

extern baseTaskInfo_t baseTaskInfo[] ;
void TaskingInit(void);

#endif

