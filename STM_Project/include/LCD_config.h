/************************************************/
/*AUthor	: Islam EL-Bahnasawy				*/
/*Version	: V01								*/
/*Date 		: 14 Jan 2020						*/
/************************************************/
#ifndef LCD_CONFIH_H
#define LCD_CONFIH_H


#define CLCD_u8_RS_PORT	'A'
#define CLCD_u8_RS_PIN	5

#define CLCD_u8_RW_PORT	'A'
#define CLCD_u8_RW_PIN	6

#define CLCD_u8_E_PORT	'A'
#define CLCD_u8_E_PIN	7
	
//#define CLCD_u8_DATA_PORT 'A'

#define CLCD_u8_D0_PORT 'A'
#define CLCD_u8_D1_PORT 'A'
#define CLCD_u8_D2_PORT 'A'
#define CLCD_u8_D3_PORT 'A'

#define CLCD_u8_D4_PORT 'B'
#define CLCD_u8_D5_PORT 'B'
#define CLCD_u8_D6_PORT 'B'
#define CLCD_u8_D7_PORT 'B'

#define CLCD_u8_D0	11
#define CLCD_u8_D1	10
#define CLCD_u8_D2	9
#define CLCD_u8_D3	8

#define CLCD_u8_D4	15
#define CLCD_u8_D5	14
#define CLCD_u8_D6	13
#define CLCD_u8_D7	12

#define FOUR_BITS 0
#define EIGHT_BITS 1
#define CLCD_u8_DATA_LENGTH	EIGHT_BITS

#define IDLE_RUNNABEL 		0
#define WRITE_LCD_RUNNABLE 	1
#define CLR_LCD_RUNNABLE	2
#define MOVE_LCD_RUNNABLE 	3

#define CLEAR_CMD	0b00000001
#define LINE_0		0b10000000
#define LINE_1		0b11000000

#define CLCD_u8_FUNCTION_SET_8BITS_2LINES	0b00111000
#define CLCD_u8_FUNCTION_SET_4BITS_2LINES	0b00101000
#define CLCD_u8_FUNCTION_SET_8BITS_1LINES	0b00110000
#define CLCD_u8_FUNCTION_SET_4BITS_1LINES	0b00100000

#define CLCD_u8_DISPLAY_ON_CONTROL	0b00001100
#define CLCD_u8_CLR_SCREEN	0b00000001

#define CLCD_u8_CURSOR_SHIFT_LEFT	0b00010000
#define CLCD_u8_CURSOR_SHIFT_RIGHT	0b00010100
#define CLCD_u8_DISPLAY_SHIFT_LEFT	0b00011000
#define CLCD_u8_DISPLAY_SHIFT_RIGHT	0b00011100


#define CLCD_u8_CGRAM_ADD_0	0b01000000
#define CLCD_u8_DDRAM_ADD_0	0b10000000


#endif
