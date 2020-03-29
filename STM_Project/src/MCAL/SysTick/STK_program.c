/****************************************************/
/* Author : ANWAR									*/
/* Date : 8 MARCH 2020								*/
/* Version : V01									*/
/****************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "STK_interface.h"
#include "STK_reg.h"
#include "STK_config.h"
#include "STK_private.h"

static sysTickcbf_t callBack = NULL;

u8 sysTick_init(void)
{
	u8 state = OK;
	u32 temp;
	if( (CLOCKSOURCE & INTERRUPT_STATE & STK_MASK) == STK_MASK)
	{
		temp = (STK_CTRL |(CLOCKSOURCE | INTERRUPT_STATE)) & (~STK_MASK);
		//STK_CTRL &= STK_INIT_MASK;
		STK_CTRL = temp;
	}
	else
	{
		state = NOT_OK;
	}
	return state;
}

u8 sysTick_start(void)
{
	u8 state = OK;
	u32 temp;
	if( (STK_ENABLE & STK_MASK) == STK_MASK)
	{
		temp = (STK_CTRL|STK_ENABLE) & (~STK_MASK);
		STK_CTRL = temp;
	}
	else 
		state = NOT_OK;
	return state;
}

u8 sysTick_stop(void)
{
	u8 state = OK;
	u32 temp;
	if( (STK_ENABLE & STK_MASK) == STK_MASK)
	{
		temp = STK_CTRL & (~STK_ENABLE) &(~STK_MASK);
		STK_CTRL = temp;

	}
	else 
		state = NOT_OK;
	return state;

}

void sysTick_setTime(u32 timeUs , const u32 input_freqMHZ)
{
#if CLOCKSOURCE == STK_SOURCE_AHB_8
	/*An equation to calculate the number of ticks needed to get the needed time*/
	STK_LOAD = timeUs * (input_freqMHZ/8);
#elif CLOCKSOURCE == STK_SOURCE_AHB
	STK_LOAD = timeUs * (input_freqMHZ/1);
#endif
}

u8 sysTick_setcallback(sysTickcbf_t cbf)
{
	u8 state = OK;
	if(cbf)
	{
		callBack = cbf;
	}
	else 
	{
		state = NOT_OK;
	}
	return state;
}

void SysTick_Handler(void)
{

	if(callBack)
	{
		callBack();
	}
}
