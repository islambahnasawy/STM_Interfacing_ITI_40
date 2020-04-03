#include "../../LIB/STD_TYPES.h"

#include "../../RTOS/RTOS_interface.h"
#include "Switch_interface.h"
#include "../../MCAL/DIO/DIO_interface.h"



u8 switchState[NUM_OF_SWITCH];

const task_t UpdatingSwitchesTask = {.handler=Switch_updateValue,.preodicityMS=5};

void switch_init(void)
{
	for(u8 i=0;i<NUM_OF_SWITCH;i++)
	{
		if(switchMap[i].state==PIN_MODE_IP_PULLUP)
		{
			DIO_SetPinVal(switchMap[i].GPIO_pinPort,switchMap[i].GPIO_pinNum,GPIO_SET);
		}
		else if(switchMap[i].state==PIN_MODE_IP_PULLDOWN)
		{
			DIO_SetPinVal(switchMap[i].GPIO_pinPort,switchMap[i].GPIO_pinNum,GPIO_RESET);
		}
		DIO_SetPinMode(switchMap[i].GPIO_pinPort,switchMap[i].GPIO_pinNum,PIN_MODE_IP_PULLDOWN);
	}
}

u8 getSwitchState(u8 switchNum)
{
	return switchState[switchNum];
}
#define STABLE_STATE_MS 20
#define TASK_PERIOD_MS		5
void Switch_updateValue(void)
{
	//trace_printf("switch");
	static u8 counter[NUM_OF_SWITCH] , lastValue[NUM_OF_SWITCH];
	u8 currentValue;
	for(u8 i=0;i<NUM_OF_SWITCH;i++)
	{
		currentValue = DIO_GetPinVal(switchMap[i].GPIO_pinPort,switchMap[i].GPIO_pinNum);
		if(currentValue == lastValue[i])
		{
			counter[i]++;
		}
		else 
		{
			counter[i] = 0;
		}
		if(counter[i] == 4)
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

