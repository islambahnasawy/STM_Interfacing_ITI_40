#ifndef _RTOS_CONFIG_H_
#define _RTOS_CONFIG_H_

#define MCU STM32

#define NUM_OF_TASKS 	1
#define TICKTIME_MS		1


typedef struct
{
	task_t* task;
	u32 firstdelay;
}baseTaskInfo_t;

extern baseTaskInfo_t baseTaskInfo[] ;
void TaskingInit(void);

#endif

