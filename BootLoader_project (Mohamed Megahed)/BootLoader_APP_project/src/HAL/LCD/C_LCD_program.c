/********************************************************/
/*Author : Anwar				 						*/
/*Version : V01					 						*/
/*Date : 9 MARCH 2020								    */
/********************************************************/

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../../LIB/Delay/DELAY_interface.h"

#include "C_LCD_interface.h"
#include "C_LCD_private.h"
#include "C_LCD_config.h"

#include "../../MCAL/DIO/DIO_interface.h"








void CLCD_voidIntialzie(void)
{

	DIO_SetPinMode(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);
	DIO_SetPinMode(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);
	DIO_SetPinMode(CLCD_u8_E_PORT,CLCD_u8_E_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);

	DIO_SetPinMode(CLCD_u8_D0_PORT,CLCD_u8_D0_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);
	DIO_SetPinMode(CLCD_u8_D1_PORT,CLCD_u8_D1_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);
	DIO_SetPinMode(CLCD_u8_D2_PORT,CLCD_u8_D2_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);
	DIO_SetPinMode(CLCD_u8_D3_PORT,CLCD_u8_D3_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);
	DIO_SetPinMode(CLCD_u8_D4_PORT,CLCD_u8_D4_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);
	DIO_SetPinMode(CLCD_u8_D5_PORT,CLCD_u8_D5_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);
	DIO_SetPinMode(CLCD_u8_D6_PORT,CLCD_u8_D6_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);
	DIO_SetPinMode(CLCD_u8_D7_PORT,CLCD_u8_D7_PIN,PIN0_MODE_OP_PUSHPULL_SPEED_10MHZ);
	delay_mSec(40);
#if CLCD_u8_DATA_LENGTH == FOUR_BITS_MODE
	/*first 4 bits */
	CLCD_voidsethalfDataPort(0b0010);
	CLCD_Kick();
	/*second 4 bits */
	CLCD_voidsethalfDataPort(0b0010);
	CLCD_Kick();
	/*third 4 bits */
	CLCD_voidsethalfDataPort(0b1000);
	CLCD_Kick();
#elif CLCD_u8_DATA_LENGTH == EIGHT_BITS_MODE

#else #error "foo2 ya beh"
#endif
	//set two lines command
	CLCD_voidWriteCmd(0b00111000);
	//_delay_ms(4);
	//dispay on command
	CLCD_voidWriteCmd(0b00001100);

	/*Clears the screen*/
	CLCD_voidWriteCmd(0b00000001);
	delay_mSec(4);
	/*Setting entry mode*/
	CLCD_voidWriteCmd(0b00000110);
}

/*Description : This API shall display data on LCD 		*/
void CLCD_voidWriteData(u8 Copy_u8Data)
{
	/*Set RS = 1*/
	DIO_SetPinVal(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,1);

	/*Set RW = 0*/
	DIO_SetPinVal(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,0);

	/*Set Data on Data port*/
#if CLCD_u8_DATA_LENGTH == FOUR_BITS_MODE
	CLCD_voidsethalfDataPort(Copy_u8Data>>4);
	CLCD_Kick();
	CLCD_voidsethalfDataPort(Copy_u8Data);
	CLCD_Kick();
#elif CLCD_u8_DATA_LENGTH == EIGHT_BITS_MODE
	/*Set Data on Data port*/
	CLCD_voidSetDataPort(Copy_u8Data);
	CLCD_Kick();
#else #error "foo2 ya zboon"
#endif
}

/*Description : This API shall execute command on LCD 	*/
void CLCD_voidWriteCmd(u8 Copy_u8Cmd)
{
	/*Set RS = 0*/
	DIO_SetPinVal(CLCD_u8_RS_PORT,CLCD_u8_RS_PIN,0);

	/*Set RW = 0*/
	DIO_SetPinVal(CLCD_u8_RW_PORT,CLCD_u8_RW_PIN,0);
#if CLCD_u8_DATA_LENGTH == FOUR_BITS_MODE
	CLCD_voidsethalfDataPort(Copy_u8Cmd>>4);
	CLCD_Kick();
	CLCD_voidsethalfDataPort(Copy_u8Cmd);
	CLCD_Kick();
#elif CLCD_u8_DATA_LENGTH == EIGHT_BITS_MODE
	/*Set Data on Data port*/
	CLCD_voidSetDataPort(Copy_u8Cmd);
	CLCD_Kick();
#else #error "foo2 ya zboon"
#endif
}

static void CLCD_voidSetDataPort(u8 Copy_u8Data)
{
	u8 Local_u8BitVal;

	Local_u8BitVal = GET_BIT(Copy_u8Data,0);
	DIO_SetPinVal(CLCD_u8_D0_PORT,CLCD_u8_D0_PIN,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data,1);
	DIO_SetPinVal(CLCD_u8_D1_PORT,CLCD_u8_D1_PIN,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data,2);
	DIO_SetPinVal(CLCD_u8_D2_PORT,CLCD_u8_D2_PIN,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data,3);
	DIO_SetPinVal(CLCD_u8_D3_PORT,CLCD_u8_D3_PIN,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data,4);
	DIO_SetPinVal(CLCD_u8_D4_PORT,CLCD_u8_D4_PIN,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data,5);
	DIO_SetPinVal(CLCD_u8_D5_PORT,CLCD_u8_D5_PIN,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data,6);
	DIO_SetPinVal(CLCD_u8_D6_PORT,CLCD_u8_D6_PIN,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data,7);
	DIO_SetPinVal(CLCD_u8_D7_PORT,CLCD_u8_D7_PIN,Local_u8BitVal);
}
static void CLCD_voidsethalfDataPort(u8 Copy_u8Data)
{
	u8 Local_u8BitVal;

	Local_u8BitVal = GET_BIT(Copy_u8Data,0);
	DIO_SetPinVal(CLCD_u8_D0_PORT,CLCD_u8_D4_PIN,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data,1);
	DIO_SetPinVal(CLCD_u8_D1_PORT,CLCD_u8_D5_PIN,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data,2);
	DIO_SetPinVal(CLCD_u8_D2_PORT,CLCD_u8_D6_PIN,Local_u8BitVal);

	Local_u8BitVal = GET_BIT(Copy_u8Data,3);
	DIO_SetPinVal(CLCD_u8_D3_PORT,CLCD_u8_D7_PIN,Local_u8BitVal);
}

static void CLCD_Kick()
{
	/*Enable pulse*/
	DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,1);
	delay_mSec(4);
	DIO_SetPinVal(CLCD_u8_E_PORT,CLCD_u8_E_PIN,0);
	delay_mSec(4);
}

void lcd_move_cursor(u8 row,u8 colomn){
	if(row==0){
		if(colomn<16)
			CLCD_voidWriteCmd(0x80+colomn);
	}
	else if (row==1){
		if(colomn<16)
			CLCD_voidWriteCmd(0xC0+colomn);
	}
}


void CLCD_voidClearScreen(void)
{
	CLCD_voidWriteCmd(0b00000001);
	delay_mSec(4);
}



void CLCD_voidWriteString(const char* Copy_pchString)
{
	while(*(Copy_pchString)!=0)
	{
		CLCD_voidWriteData(*(Copy_pchString++));
	}
}


void CLCD_voidCGRAM(void)
{
	CLCD_voidWriteCmd(0b01000000);
}

void CLCD_voidDDRAM(void)
{
	CLCD_voidWriteCmd(0b10000000);
}
