#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/DIO/DIO_interface.h"
#include "LED_interface.h"

typedef struct
{
	u32 GPIO_pinNum;
	u32 GPIO_pinPort;
}ledmap_t;
typedef struct
{
	u8 ledState;
	u8 GPIO_state;
}ledstatemap_t;
extern ledmap_t ledmap[LED_NUM];
extern ledstatemap_t ledstatemap[LED_NUM];
u8 LED_Init(u8 ledNum)
{
	u8 localStatus = OK;
	if(ledNum>=LED_NUM)
	{
		localStatus=NOT_OK;
	}
	else
	{
	DIO_SetPinMode(ledmap[ledNum].GPIO_pinPort,ledmap[ledNum].GPIO_pinNum,PIN_MODE_OP_PUSHPULL);
	}
	return localStatus;
}

u8 LED_setLedState(u8 ledNum , u8 state)
{
	u8 localStatus = OK;
	if(ledNum>=LED_NUM)
	{
		localStatus = NOT_OK;
	}
	else
	{
	if(ledstatemap[ledNum].ledState == LED_STATE_ON && ledstatemap[ledNum].GPIO_state==GPIO_SET)
	{
		DIO_SetPinVal(ledmap[ledNum].GPIO_pinPort,ledmap[ledNum].GPIO_pinNum,state);
	}
	else if(ledstatemap[ledNum].ledState == LED_STATE_ON && ledstatemap[ledNum].GPIO_state==GPIO_RESET)
	{
		DIO_SetPinVal(ledmap[ledNum].GPIO_pinPort,ledmap[ledNum].GPIO_pinNum,state^1);
	}
	else if(ledstatemap[ledNum].ledState == LED_STATE_OFF && ledstatemap[ledNum].GPIO_state==GPIO_RESET)
	{
		DIO_SetPinVal(ledmap[ledNum].GPIO_pinPort,ledmap[ledNum].GPIO_pinNum,state);
	}
	else if(ledstatemap[ledNum].ledState == LED_STATE_OFF && ledstatemap[ledNum].GPIO_state==GPIO_SET)
	{
		DIO_SetPinVal(ledmap[ledNum].GPIO_pinPort,ledmap[ledNum].GPIO_pinNum,state^1);
	}
	else
	{
		localStatus = NOT_OK;
	}
	}
	return localStatus;
}
