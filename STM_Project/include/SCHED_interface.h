#ifndef SCHED_INTERFACE_H
#define SCHED_INTERFACE_H

#include "STD_TYPES.h"
#include "BIT_MATH.h"


typedef void (*Task_Runnable_t)(void);

typedef struct
{
	Task_Runnable_t Runnable;
	u32 peridicity_ms;
	
}task_t;


Status SCHED_voidInit(void);
Status SCHED_Start(void);
void SCHED_Suspend(void);

#endif
