/********************************************************/
/*Author : Anwar				 						*/
/*Version : V01					 						*/
/*Date : 9 MARCH 2020								    */
/********************************************************/


#define CLCD_u8_LINE_ONE	0
#define CLCD_u8_LINE_TWO	1

/*The LCD commands*/
#define LCD_WRITE 			0
#define LCD_CLEAR 			1
#define LCD_MOVE_CURSOR 	2

/*LCD runnable task*/
extern const task_t LCD_Task;

/* The max queue size */
#define MAX_QUEUED_TASKS 10U

/******************************************************************************************
* function name: CLCD_init
*
* parameters:  Input:  NA
*              Output: NA
*              In/out: NA
*
* return: void
* Description: This function initialize the LCD's DIO pins
********************************************************************************************/
void CLCD_init(void);

/******************************************************************************************
* function name: LCD_OS_write
*
* parameters:  Input:
* 					word
* 						type :  u8* const
* 						Description : This parameter takes the address of the word sent by user
*              Output: NA
*              In/out: NA
*
* return: void
* Description: This function writes a word on the LCD screen , should only be used in case of OS
* 				and the LCD_OS_runnable function is a running task
********************************************************************************************/
void LCD_OS_write(u8* const word);

/******************************************************************************************
* function name: LCD_OS_runnable
*
* parameters:  Input:  NA
*              Output: NA
*              In/out: NA
*
* return: void
* Description: This function is used as a task in case of OS and it's periodicity shouldn't be
* 				less than 2Ms
********************************************************************************************/
void LCD_OS_runnable(void);


/******************************************************************************************
* function name: LCD_OS_clear
*
* parameters:  Input:  NA
*              Output: NA
*              In/out: NA
*
* return: void
* Description: This function is used to clear the LCD screen , should only be used in case of OS
* 				and the LCD_OS_runnable function is a running task
********************************************************************************************/
void LCD_OS_clear(void);


/******************************************************************************************
* function name: LCD_OS_clear
*
* parameters:  Input:
* 					row
* 						type : u8
* 						Description : The row at which the cursor will be moved
* 					col
* 						type : u8
* 						Description : The col at which the cursor will be moved
*              Output: NA
*              In/out: NA
*
* return: void
* Description: This function is used to move the cursor on an LCD screen , should only be
* 				used in case of OS and the LCD_OS_runnable function is a running task
********************************************************************************************/
void LCD_OS_move_cursor(u8 row , u8 col);


/*Description : This API shall intialize LCD 			*/
void CLCD_voidIntialzie(void);

/*Description : This API shall display data on LCD 		*/
void CLCD_voidWriteData(u8 Copy_u8Data);

/*Description : This API shall execute command on LCD 	*/
void CLCD_voidWriteCmd(u8 Copy_u8Cmd);

void lcd_move_cursor(u8 row,u8 colomn);

void CLCD_voidWriteString(const char*str) ;

void CLCD_voidClearScreen(void);

void CLCD_VoidGoToXYPos(u8 Copy_u8XPos,u8 Copy_u8YPos);

void CLCD_voidCGRAM(void);

void CLCD_voidDDRAM(void);
