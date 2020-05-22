/********************************************************/
/*Author : Anwar				 						*/
/*Version : V01					 						*/
/*Date : 9 MARCH 2020								    */
/********************************************************/

typedef unsigned char u8;

/*Description : This API shall intialize LCD 			*/
void CLCD_voidIntialzie(void);

/*Description : This API shall display data on LCD 		*/
void CLCD_voidWriteData(u8 Copy_u8Data);

/*Description : This API shall execute command on LCD 	*/
void CLCD_voidWriteCmd(u8 Copy_u8Cmd);

void lcd_move_cursor(u8 row,u8 colomn);

void CLCD_voidWriteString(const char*str) ;

void CLCD_voidClearScreen(void);


void CLCD_voidCGRAM(void);

void CLCD_voidDDRAM(void);
