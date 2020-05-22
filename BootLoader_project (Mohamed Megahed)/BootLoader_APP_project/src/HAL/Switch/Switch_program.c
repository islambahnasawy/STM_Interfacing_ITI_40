/********************************************************/
/*Author : Mohamed Megahed ,Islam El-Bahnasawy          */
/*Date   : 18 April 2020								*/
/*Version : V01											*/
/********************************************************/

#include "../../LIB/STD_TYPES.h"

#include "../../RTOS/RTOS_interface.h"
#include "Switch_interface.h"
#include "../../MCAL/DIO/DIO_interface.h"


#define STABLE_STATE_MS			20
#define TASK_PERIOD_MS 			5

static u8 switchState[NUM_OF_SWITCH];

const task_t UpdatingSwitchesTask = {.handler=Switch_updateValue,.preodicityMS=TASK_PERIOD_MS};

void switch_init(void)
{
	for(u8 i=0;i<NUM_OF_SWITCH;i++)
	{
		if(switchMap[i].state==PIN_MODE_IP_PULLUP)
		{
			DIO_SetPinVal((GPIO_Prepihral*)switchMap[i].GPIO_pinPort,switchMap[i].GPIO_pinNum,GPIO_SET);
		}
		else if(switchMap[i].state==PIN_MODE_IP_PULLDOWN)
		{
			DIO_SetPinVal((GPIO_Prepihral*)switchMap[i].GPIO_pinPort,switchMap[i].GPIO_pinNum,GPIO_RESET);
		}
		DIO_SetPinMode((GPIO_Prepihral*)switchMap[i].GPIO_pinPort,switchMap[i].GPIO_pinNum,PIN_MODE_IP_PULLDOWN);
	}
}

u8 getSwitchState(u8 switchNum)
{
	return switchState[switchNum];
}

void Switch_updateValue(void)
{
	static u8 counter[NUM_OF_SWITCH] , lastValue[NUM_OF_SWITCH];
	u8 currentValue;
	for(u8 i=0;i<NUM_OF_SWITCH;i++)
	{
		 GPIO_GetPinVal((GPIO_Prepihral*)switchMap[i].GPIO_pinPort,switchMap[i].GPIO_pinNum,&currentValue);
		if(currentValue == lastValue[i])
		{
			counter[i]++;
		}
		else 
		{
			counter[i] = 0;
		}
		if(counter[i] == STABLE_STATE_MS/TASK_PERIOD_MS)
		{
			if(switchMap[i].state==PIN_MODE_IP_PULLUP)
				switchState[i] = currentValue^1;
			else 
				switchState[i] = currentValue;
			counter[i]=0;
		}
		lastValue[i]=currentValue;
	}
}

