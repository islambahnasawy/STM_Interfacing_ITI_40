#include "STD_TYPES.h"
#include "Switch_interface.h"
#include "DIO_interface.h"


typedef struct
{
	u32 GPIO_pinNum;
	u32 GPIO_pinPort;
	u32 state;
}switch_t;


const switch_t switchMap[NUM_OF_SWITCH] = 
{
{PIN11,PORTB,PIN_MODE_IP_PULLUP}
};


