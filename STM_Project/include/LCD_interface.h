#ifndef LCD_INTERFACE_H
#define LCD_INTERFACE_H

void LCD_voidRunnable(void);
Status LCD_write(u8 copy_u8Char);
Status LCD_write_string(u8* copy_u8Char);
Status LCD_CLR(void);
Status LCD_MOVE_CRS(u8 x, u8 y);
#endif
