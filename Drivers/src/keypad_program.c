#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "keypad_interface.h"
#include "keypad_config.h"
#include "keypad_private.h"
#include "DIO_interface.h"




u8 keypad_value(void)
{
	u8 key_values[ROWS_NUM][COLS_NUM] = KEYPAD_VALUES;
	u8 rows_ports [] = KEYPAD_ROWS_PORTS;
	u8 rows_pins [] = KEYPAD_ROWS_PINS;
	u8 cols_ports [] = KEYPAD_COLS_PORTS;
	u8 cols_pins [] = KEYPAD_COLS_PINS;
	u8 R_index , C_index;
	u8 pin_state;
	
	for(R_index = 0 ; R_index < ROWS_NUM ; R_index++)
	{
		DIO_SetPinVal(rows_ports[R_index],rows_pins[R_index],0);
		for(C_index = 0; C_index < COLS_NUM ; C_index++)
		{
			if (C_index == 2)continue;
			pin_state = DIO_GetPinVal(cols_ports[C_index],cols_pins[C_index]);
			if(0 == pin_state)
			{
				while(0 == pin_state) pin_state =DIO_GetPinVal(cols_ports[C_index],cols_pins[C_index]);
				return key_values[R_index][C_index];
			}
		}
		DIO_SetPinVal(rows_ports[R_index],rows_pins[R_index],1);
	}
	return DEFAULT_VALUE;
}
