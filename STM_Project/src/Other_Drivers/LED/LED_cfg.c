#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/DIO/DIO_interface.h"
#include "../../Other_Drivers/LED/LED_interface.h"
typedef struct 
{
	u32 GPIO_pinNum;
	u8 GPIO_pinPort;
}ledmap_t;

typedef struct
{
	u8 ledState;
	u8 GPIO_state;
}ledstatemap_t;

const ledmap_t ledmap[LED_NUM] =
{
{PIN13,PORTC},
{PIN0,PORTA},
{PIN1,PORTA}
};

const ledstatemap_t ledstatemap[LED_NUM] =
{
{LED_STATE_ON,GPIO_RESET},
{LED_STATE_ON,GPIO_SET},
{LED_STATE_ON,GPIO_SET}
};

