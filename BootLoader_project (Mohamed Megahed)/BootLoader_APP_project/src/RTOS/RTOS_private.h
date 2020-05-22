#ifndef _RTOS_PRIAVTE_H_
#define _RTOS_PRIVATE_H_

#define READY_STATE 		0x00
#define SUSPENDED_STATE 	0x01

typedef struct
{
	baseTaskInfo_t* baseTaskInfo;
	u32 ticksToExec;
	u32 periodTicks;
	u32 state;
}sysTask_t;

static void setOSFlag(void);
static void schedular(void);


#endif
