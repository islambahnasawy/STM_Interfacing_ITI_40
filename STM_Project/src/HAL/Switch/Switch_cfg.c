#include "../../LIB/STD_TYPES.h"

#include "../../MCAL/DIO/DIO_interface.h"
#include "Switch_cfg.h"



/*Add a structure of type switch_t as an element to this array for every switch in the system*/
const switch_t switchMap[NUM_OF_SWITCH] = 
{
		{PIN5,PORTB,PIN_MODE_IP_PULLUP},
		{PIN9,PORTB,PIN_MODE_IP_PULLUP},
		{PIN7,PORTB,PIN_MODE_IP_PULLUP},
		{PIN6,PORTB,PIN_MODE_IP_PULLUP},
		{PIN11,PORTB,PIN_MODE_IP_PULLUP}
};


