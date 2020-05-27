/****************************************************/
/* Author : ANWAR									*/
/* Date : 8 MARCH 2020								*/
/* Version : V01									*/
/****************************************************/

#ifndef STK_INTERFACE_H_
#define STK_INTERFACE_H_

#define STK_SOURCE_AHB		0x10000004
#define STK_SOURCE_AHB_8	0x10000000

#define STK_INT_ENABLE		0x10000002
#define STK_INT_DISABLE		0x10000000


typedef void (*sysTickcbf_t)(void);


/******************************************************************************************
* function name: sysTick_init
*
* parameters:  Input:  NA
*              Output: NA
*              In/out: NA
*
* return: OK , NOT_OK
* Description: This function is used to initialize the SYStick peripheral according to user
*			   static configurations
********************************************************************************************/
extern u8 sysTick_init(void);


/******************************************************************************************
* function name: sysTick_start
*
* parameters:  Input:  NA
*              Output: NA
*              In/out: NA
*
* return: OK , NOT_OK
* Description: This function starts the SYStick counting
********************************************************************************************/
extern u8 sysTick_start(void);


/******************************************************************************************
* function name: sysTick_stop
*
* parameters:  Input:  NA
*              Output: NA
*              In/out: NA
*
* return: OK , NOT_OK
* Description: This function stops the SYStick counting
********************************************************************************************/
extern u8 sysTick_stop(void);


/******************************************************************************************
* function name: sysTick_setTime
*
* parameters:  Input:
*                 timeUs
*                     type: u32
*                     Description: this variable allows the change in time between each
*                     				interrupt , its value is in Microsecond
*                 input_freqMHZ
*                     type: const u32
*                     Description: Pass the value of the system clock's frequency in MHZ
*                     				in this variable
*              Output: NA
*              In/out: NA
*
* return: OK , NOT_OK
* Description: This function configures the time between each interrupt
********************************************************************************************/
extern void sysTick_setTime(u32 timeUs , const u32 input_freqMHZ);



/******************************************************************************************
* function name: sysTick_setcallback
*
* parameters:  Input:
*                 cbf
*                     type: sysTickcbf_t
*                     Description: pass the callback function to this variable
*                     				(a function that is performed in every interrupt)
*              Output: NA
*              In/out: NA
*
* return: OK , NOT_OK
* Description: This function configures the call back function of the SYStick
********************************************************************************************/
extern u8 sysTick_setcallback(sysTickcbf_t cbf);


#endif
