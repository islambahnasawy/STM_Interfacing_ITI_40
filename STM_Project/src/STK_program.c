/**************************************/
/*Author   : Islam A. El-Bahnasawy    */
/*Date     : 8 Mar 2020               */
/*Version  : V.01                     */
/**************************************/


#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "STK_private.h"
#include "STK_config.h"
#include "STK_interface.h"
#include "STK_register.h"


/* SysTick_CB_t: Pointer to function used for definition of call back function */

typedef void (*SysTick_CB_t)(void);

static SysTick_CB_t 	App_CBF;
Status 				init_Status = NOT_OK;


extern Status SysTick_voidInit(void)
{
	Status localstate = OK;
	/* Enable Systick Interrupt Clock = AHB/8 */
	/* SysTick exception request enable */
	STK_CTRL |= EN_TIMER_REQ;
	if(CLK_SRC_PRESCALER == PRE_AHB)
	{
		STK_CTRL |= CLK_SRC_AHB;
		init_Status = OK;
	}
	else if(CLK_SRC_PRESCALER == PRE_AHB_div8)
	{
		STK_CTRL &= CLK_SRC_AHB_div_8;
		init_Status = OK;
	}
	else
	{
		localstate = NOT_OK;
		init_Status = OK;
	}

	return localstate;

}

extern Status SysTick_voidStart(void)
{
	Status localstate = NOT_OK;
	if(init_Status == OK)
	{
		/* Start Systick Timer*/
		STK_CTRL |= START_TIMER;
		localstate = OK;
	}

	return localstate;
}

extern Status SysTick_voidStop(void)
{
	Status localstate = NOT_OK;
	if(init_Status == OK)
	{
		/* Stop Systick Timer*/
		STK_CTRL &= STOP_TIMER;
		localstate = OK;
	}

	return localstate;
}

extern Status SysTick_voidSetTime_us(u32 time_us, const u8 SysTick_Input_CLK_MHZ)
{

	Status localstate = NOT_OK;
	if(init_Status == OK)
	{
		u32 reloadVal = (time_us * (SysTick_Input_CLK_MHZ / CLK_SRC_PRESCALER));
		/* Load value into load register*/
		STK_LOAD = reloadVal;
		localstate = OK;
	}

	return localstate;
}


extern Status SysTick_voidSetCallBack(SysTick_CB_t CallBack_Func)
{
	Status localstate = NOT_OK;
	if(CallBack_Func)
	{
		App_CBF =CallBack_Func;
		localstate = OK;
	}
	return localstate;
}



void SysTick_Handler(void)
{
	if(App_CBF)
	{
		App_CBF();	
	}
}
