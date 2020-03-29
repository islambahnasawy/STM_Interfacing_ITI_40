#include "../LIB/STD_TYPES.h"

#include "RTOS_interface.h"
#include "RTOS_config.h"
#include "RTOS_private.h"
#include "../MCAL/SysTick/STK_interface.h"
#include "../MCAL/RCC/RCC_interface.h"


u8 OSFlag;
u32 index;

sysTask_t sysTask[NUM_OF_TASKS];

void RTOS_init(void)
{
	u32 SYSTIC_freqMHZ = RCC_u32ReadAHBFreq()/1000000;
	sysTick_init();
	sysTick_setTime(TICKTIME_MS*1000,SYSTIC_freqMHZ);
	sysTick_setcallback(setOSFlag);
	
	for(u32 i=0;i<NUM_OF_TASKS;i++)
	{
		sysTask[i].baseTaskInfo = &baseTaskInfo[i];
		sysTask[i].ticksToExec = (baseTaskInfo[i].firstdelay)/TICKTIME_MS;
		sysTask[i].periodTicks = ((baseTaskInfo[i].task)->preodicityMS)/TICKTIME_MS;
	}
}

static void setOSFlag(void)
{
	OSFlag ++;
}

static void schedular(void)
{
			for(index=0;index<NUM_OF_TASKS;index++)
			{
				if(sysTask[index].ticksToExec==0&&sysTask[index].state==READY_STATE)
				{
					(((sysTask[index].baseTaskInfo)->task)->handler)();
					sysTask[index].ticksToExec = sysTask[index].periodTicks;
				}
				sysTask[index].ticksToExec--;
			}
}

void RTOS_start(void)
{
	sysTick_start();
	while(1)
	{
		if(OSFlag)
		{
			OSFlag = 0;
			schedular();
		}
	}
}

void suspend_Task(void)
{
	sysTask[index].state = SUSPENDED_STATE;
}
