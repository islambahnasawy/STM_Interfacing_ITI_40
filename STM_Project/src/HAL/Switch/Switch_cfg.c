/******************************************************/
/*Author   : Islam A. El-Bahnasawy, Mohammed Anwar    */
/*Date     : 15 Mar 2020                              */
/*Version  : V.01                                     */
/******************************************************/

#include "../../LIB/STD_TYPES.h"
#include "Switch_interface.h"
#include "../../MCAL/DIO/DIO_interface.h"


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


