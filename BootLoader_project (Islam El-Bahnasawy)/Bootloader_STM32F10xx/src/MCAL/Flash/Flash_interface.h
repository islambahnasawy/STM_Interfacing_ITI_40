/********************************************************/
/*Author : Mohamed Megahed , Islam El-Bahnasawy 		*/
/*Date   : 8 May 2020									*/
/*Version : V01											*/
/********************************************************/

#ifndef MCAL_FLASH_FLASH_INTERFACE_H_
#define MCAL_FLASH_FLASH_INTERFACE_H_


void Flash_unlock(void);
void Flash_lock(void);
u8 Flash_massErase(void);
u8 Flash_pageErase(u32 address);
u8 Flash_program(u32 address, u16 data);
u8 Flash_programWrite(u16*start_address_Flash,u16* start_address_RAM , u32 numberOfBytes);

#endif /* MCAL_FLASH_FLASH_INTERFACE_H_ */
