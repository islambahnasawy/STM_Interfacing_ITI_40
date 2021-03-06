/********************************************************/
/*Author : Mohamed Megahed ,Islam El-Bahnasawy          */
/*Date   : 9 March 2020									*/
/*Version : V01											*/
/********************************************************/




#define CLCD_u8_LINE_TWO_ADD_OFFSET 0x40
#define CLCD_u8_SET_DDRAM_ADD_COMM 0x80

#define FIRST_STEP	0
#define SECOND_STEP	1
#define DONE		2
#define THIRD_STEP	3
#define FOURTH_STEP	4

typedef void (*handler)(void);

static void CLCD_voidSetDataPort(u8 Copy_u8Data);
static void FSM_task1(void);
static void FSM_task2(void);
static void FSM_task3(void);
static void FSM_task4(void);
static void FSM_task5(void);
static void LCD_apply(void);
static void LCD_wipe(void);
static void LCD_cursor_moving(void);
static void LCD_cursor_blinkingOn(void);
static void LCD_cursor_blinkingOff(void);

static void CLCD_voidSetDataPort(u8 Copy_u8Data);
static void CLCD_voidsethalfDataPort(u8 Copy_u8Data);
static void CLCD_Kick();

#define FOUR_BITS_MODE 		0
#define EIGHT_BITS_MODE 	1
