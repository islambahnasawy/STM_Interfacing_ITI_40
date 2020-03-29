#ifndef _RTOS_INTERFACE_H_
#define _RTOS_INTERFACE_H_


typedef void(*Handler)(void);
/*User should specify the function that will be called and its periodicity */
typedef struct
{
	Handler handler;
	u32 preodicityMS;
}task_t;

//should this struct be here ? or should we put the baseTaskInfo in program.c?
typedef struct
{
	task_t* task;
	u32 firstdelay;
}baseTaskInfo_t;

extern const baseTaskInfo_t baseTaskInfo[] ;

/******************************************************************************************
* function name: RTOS_init
*
* parameters:  Input:  NA
*              Output: NA
*              In/out: NA
*
* return: void
* Description: This function is used to initialize the OS according to user and Architect
*			    configurations
********************************************************************************************/
void RTOS_init(void);

/******************************************************************************************
* function name: RTOS_start
*
* parameters:  Input:  NA
*              Output: NA
*              In/out: NA
*
* return: void
* Description: This function is used to starts the OS
********************************************************************************************/
void RTOS_start(void);


/******************************************************************************************
* function name: suspend_Task
*
* parameters:  Input:  NA
*              Output: NA
*              In/out: NA
*
* return: void
* Description: This function is used to suspend an OS task
********************************************************************************************/
void suspend_Task(void);

#endif
