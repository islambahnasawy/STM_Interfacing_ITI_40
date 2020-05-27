/********************************************************/
/*Author : Mohamed Megahed , Islam El-Bahnasawy 		*/
/*Date   : 8 May 2020									*/
/*Version : V01											*/
/********************************************************/

#ifndef MCAL_FLASH_FLASH_REG_H_
#define MCAL_FLASH_FLASH_REG_H_

#define FLASH_BASEADDRESS		(0x40022000)

#define FLASH					((volatile Flash_t*)FLASH_BASEADDRESS)

typedef struct
{
	u32 FLASH_ACR;
	u32 FLASH_KEYR;
	u32 FLASH_OPTKEYR;
	u32 FLASH_SR;
	u32 FLASH_CR;
	u32 FLASH_AR;
	u32 reserved;
	u32 FLASH_OBR;
	u32 FLASH_WRPR;
}Flash_t;



#endif /* MCAL_FLASH_FLASH_REG_H_ */
