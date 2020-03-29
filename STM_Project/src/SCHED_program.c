#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "SCHED_interface.h"
#include "SCHED_config.h"
#include "STK_interface.h"
#include "DIO_interface.h"

#define STATE_READY 		0
#define STATE_SUSBENDED 	1
extern const BaseTaskInfo_t SYS_Tasks_Info[TASKS_NUM];
static u32 i;
u8 OS_flag = 0;

typedef struct 
{
	BaseTaskInfo_t * task_info;
	u32 periodicity_Ticks;
	u32 ticksToExec;
	u32 state;

} SYS_Task_t;


SYS_Task_t SYS_Tasks[TASKS_NUM];

static void setOSFlag(void)
{
	OS_flag++;
}

Status SCHED_voidInit(void)
{
	Status status;
	SysTick_voidInit();
	SysTick_voidSetCallBack(setOSFlag);
	status = SysTick_voidSetTime_us((TICKS_MS*1000), 8);
	if(status == OK)
	{
		for(i=0; i<TASKS_NUM; i++)
		{
			SYS_Tasks[i].task_info = &SYS_Tasks_Info[i];
			SYS_Tasks[i].periodicity_Ticks = (SYS_Tasks_Info[i].appTask->peridicity_ms)/TICKS_MS;
			SYS_Tasks[i].ticksToExec = SYS_Tasks_Info[i].first_Delay_Tick;
		}
	}

	return status;
}

static void SCHED(void)
{

	for(i=0; i<TASKS_NUM; i++)
	{
		if((SYS_Tasks[i].ticksToExec == 0) && SYS_Tasks[i].state == STATE_READY)
		{
			SYS_Tasks[i].task_info->appTask->Runnable();
			SYS_Tasks[i].ticksToExec = SYS_Tasks[i].periodicity_Ticks;
		}
		(SYS_Tasks[i].ticksToExec)--;
	}
}


Status SCHED_Start(void)
{
	Status status;


	status = SysTick_voidStart();
	if (status == OK)
	{
		DIO_SetPinVal('A',1,1);
	}
	while(1)
	{
		if(OS_flag)
		{
			//DIO_SetPinVal('A',1,1);
			OS_flag=0;
			SCHED();
		}
		//DIO_SetPinVal('A',1,0);
	}
	return status;
}

void SCHED_Suspend(void)
{
	SYS_Tasks[i].state = STATE_SUSBENDED;
}





