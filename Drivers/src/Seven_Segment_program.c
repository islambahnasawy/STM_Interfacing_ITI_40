#include "STD_TYPES.h"
#include "BIT_MATH.h"


#include "Seven_Segment_interface.h"
#include "Seven_Segment_private.h"
#include "Seven_Segment_config.h"
#include "DIO_interface.h"
u8 Lastvalue = SEVEMSEG_INTIAL_VALUE;

u8 SEVENSEG_Enable_ports[SEVENSEG_NUMBER] = {SEVENSEG1_ENABLE_PORT , SEVENSEG2_ENABLE_PORT};
u8 SEVENSEG_Enable_pins[SEVENSEG_NUMBER] = {SEVENSEG1_ENABLE_PIN,SEVENSEG2_ENABLE_PIN};
u8 counter ;



void sevenseg_setNumber(u8 number)
{
	u8 arr[10] = {0b00111111,0b00000110,0b01011011,0b01001111,0b01100110,
							0b01101101,0b01111101,0b00000111,0b01111111,0b01101111};
	Lastvalue = number ;
	/*checks if common Cathode*/
	#if SEVENSEG_MODE == 1
	DIO_SetPinVal(SEVENSEG_PIN_A_PORT , SEVENSEG_PIN_A_NUMBER , GET_BIT(arr[number],0));
	DIO_SetPinVal(SEVENSEG_PIN_B_PORT , SEVENSEG_PIN_B_NUMBER , GET_BIT(arr[number],1));
	DIO_SetPinVal(SEVENSEG_PIN_C_PORT , SEVENSEG_PIN_C_NUMBER , GET_BIT(arr[number],2));
	DIO_SetPinVal(SEVENSEG_PIN_D_PORT , SEVENSEG_PIN_D_NUMBER , GET_BIT(arr[number],3));
	DIO_SetPinVal(SEVENSEG_PIN_E_PORT , SEVENSEG_PIN_E_NUMBER , GET_BIT(arr[number],4));
	DIO_SetPinVal(SEVENSEG_PIN_F_PORT , SEVENSEG_PIN_F_NUMBER , GET_BIT(arr[number],5));
	DIO_SetPinVal(SEVENSEG_PIN_G_PORT , SEVENSEG_PIN_G_NUMBER , GET_BIT(arr[number],6));
	DIO_SetPinVal(SEVENSEG_PIN_H_PORT , SEVENSEG_PIN_H_NUMBER , GET_BIT(arr[number],7));
	/*checks if common Anode*/
	#elif SEVENSEG_MODE == 0
	DIO_SetPinVal(SEVENSEG_PIN_A_PORT , SEVENSEG_PIN_A_NUMBER , GET_BIT(arr[number],0)^0x01);
	DIO_SetPinVal(SEVENSEG_PIN_B_PORT , SEVENSEG_PIN_B_NUMBER , GET_BIT(arr[number],1)^0x01);
	DIO_SetPinVal(SEVENSEG_PIN_C_PORT , SEVENSEG_PIN_C_NUMBER , GET_BIT(arr[number],2)^0x01);
	DIO_SetPinVal(SEVENSEG_PIN_D_PORT , SEVENSEG_PIN_D_NUMBER , GET_BIT(arr[number],3)^0x01);
	DIO_SetPinVal(SEVENSEG_PIN_E_PORT , SEVENSEG_PIN_E_NUMBER , GET_BIT(arr[number],4)^0x01);
	DIO_SetPinVal(SEVENSEG_PIN_F_PORT , SEVENSEG_PIN_F_NUMBER , GET_BIT(arr[number],5)^0x01);
	DIO_SetPinVal(SEVENSEG_PIN_G_PORT , SEVENSEG_PIN_G_NUMBER , GET_BIT(arr[number],6)^0x01);
	DIO_SetPinVal(SEVENSEG_PIN_H_PORT , SEVENSEG_PIN_H_NUMBER , GET_BIT(arr[number],7)^0x01);
	#endif
	
}

void sevenseg_Enable(u8 sevenseg_num)
{  
	
 /*checks if common Anode*/
	#if SEVENSEG_MODE == 0
	DIO_SetPinVal(SEVENSEG_Enable_ports[sevenseg_num],SEVENSEG_Enable_pins[sevenseg_num],1);
	/*checks if common Cathode*/
	#elif SEVENSEG_MODE == 1
	DIO_SetPinVal(SEVENSEG_Enable_ports[sevenseg_num],SEVENSEG_Enable_pins[sevenseg_num],0);
	#endif
	sevenseg_setNumber(Lastvalue);
	
	
}

void sevenseg_Disable(u8 sevenseg_num)
{   /*checks if common Anode*/
	#if SEVENSEG_MODE == 0
	DIO_SetPinVal(SEVENSEG_Enable_ports[sevenseg_num],SEVENSEG_Enable_pins[sevenseg_num],0);
	/*checks if common Cathode*/
	#elif SEVENSEG_MODE == 1
	DIO_SetPinVal(SEVENSEG_Enable_ports[sevenseg_num],SEVENSEG_Enable_pins[sevenseg_num],1);
	#endif
}
