/********************************************************/
/*Author : Anwar				 						*/
/*Version : V01					 						*/
/*Date : 9 MARCH 2020								    */
/********************************************************/




#define CLCD_u8_LINE_TWO_ADD_OFFSET 0x40
#define CLCD_u8_SET_DDRAM_ADD_COMM 0x80


typedef void (*handler)(void);

static void CLCD_voidSetDataPort(u8 Copy_u8Data);
static void CLCD_voidsethalfDataPort(u8 Copy_u8Data);
static void CLCD_Kick();

#define FOUR_BITS_MODE 		0
#define EIGHT_BITS_MODE 	1
