#include "../LIB/STD_TYPES.h"


#include "RTOS_interface.h"
#include "../System_tasks/tasks_cfg.h"
#include "RTOS_private.h"
#if MCU == STM32
#include "../MCAL/SysTick/STK_interface.h"
#include "../MCAL/RCC/RCC_interface.h"
#elif MCU == AVR
#include "Timer0_interface.h"
#endif







 u8 OSFlag;
static u32 index;

static sysTask_t sysTask[NUM_OF_TASKS];

void RTOS_init(void)
{
#if MCU == STM32
	u32 SYSTIC_freqMHZ = RCC_u32ReadAHBFreq()/1000000;
#elif MCU == AVR
	u32 SYSTIC_freqMHZ = 8;
#endif
	sysTick_init();
	sysTick_setTime(TICKTIME_MS*1000,SYSTIC_freqMHZ);
	sysTick_setcallback(setOSFlag);
	
	/*initializing the app tasks*/
	TaskingInit();

	for(u32 i=0;i<NUM_OF_TASKS;i++)
	{
		sysTask[i].baseTaskInfo = &baseTaskInfo[i];
		sysTask[i].ticksToExec = (baseTaskInfo[i].firstdelay)/TICKTIME_MS;
		sysTask[i].periodTicks = ((baseTaskInfo[i].task)->preodicityMS)/TICKTIME_MS;
	}
}

static void setOSFlag(void)
{
	/*Incrementing the OSFlag so that we can know that something went wrong if its value is more than 1*/
	OSFlag ++;
	//trace_printf("hello\n");
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
			if(OSFlag > 1)
			{
				/*Notify user that something went wrong*/
			}
			/*Execute only once even if the OSFlag's value is more than one*/
			OSFlag = 0;
			schedular();
		}
	}
}

void suspend_Task(void)
{
	sysTask[index].state = SUSPENDED_STATE;
}
