#ifndef _SWITCH_INTERFACE_H_
#define _SWITCH_INTERFACE_H_

#include "Switch_cfg.h"


#define SWITCH_PRESSED 	1
#define SWITCH_RELESED 	0

/*Switch runnable task*/
extern const task_t UpdatingSwitchesTask;

/******************************************************************************************
* function name: switch_init
*
* parameters:  Input:  NA
*              Output: NA
*              In/out: NA
*
* return: void
* Description: This function is used to initialize the switch pins according to user
*			    configurations
********************************************************************************************/
void switch_init(void);

/******************************************************************************************
* function name: Switch_updateValue
*
* parameters:  Input:  NA
*              Output: NA
*              In/out: NA
*
* return: void
* Description: This function is used in case of OS as a runnable and should be called every
* 				5Ms
********************************************************************************************/
void Switch_updateValue(void);


/******************************************************************************************
* function name: getSwitchState
*
* parameters:  Input:
* 					switchNum
* 						type : u8
* 						Description : Pass the switch number you need to check its status
* 						Options     :
* 									SWITCH1,
* 										.
* 										.
* 										.
* 										.
* 									NUM_OF_SWITCH-1
*              Output: NA
*              In/out: NA
*
* return:
* 			type : u8
* 			Description : returns the state of the switch passed in the input parameters
* 			Options     :
* 							SWITCH_PRESSED,
* 							SWITCH_RELESED
* Description: This function gets the current state of the switch number passed in the argument
********************************************************************************************/
u8 getSwitchState(u8 switchNum);

#endif
