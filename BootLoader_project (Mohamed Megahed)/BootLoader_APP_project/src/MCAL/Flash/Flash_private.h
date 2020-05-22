/********************************************************/
/*Author : Mohamed Megahed , Islam El-Bahnasawy 		*/
/*Date   : 8 May 2020									*/
/*Version : V01											*/
/********************************************************/

#ifndef MCAL_FLASH_FLASH_PRIVATE_H_
#define MCAL_FLASH_FLASH_PRIVATE_H_


#define RDPRT_KEY					0x00A5
#define KEY_1						0x45670123
#define KEY_2						0xCDEF89AB

#define FLASH_CR_LOCK_MASK			0x00000080
#define FLASH_CR_PG_MASK			0x1
#define FLASH_CR_PER_MASK			0x2
#define FLASH_CR_MER_MASK			0x4
#define FLASH_CR_STRT_MASK			0x00000040
#define FLASH_SR_BSY_MASK			0x1
#define FLASH_SECTOR_BASE_ADD_MASK 	0xFFFFFC00
#define FLASH_SECTOR_LAST_ADD_MASK	0x3ff
#define SECTOR_SIZE_16BLKSIZE 		512
#define SECTOR_SIZE					1024
#define TWO_BYTES_CLEAR				0xFFFF

#define SECTOR_EMPTY				20
#define SECTOR_FULL					25

#define TIMEOUT_LIMIT				10000

#define ADD_MASK_GEN(SHIFT_VAL) (0xFFFF0000>>(SHIFT_VAL))

typedef struct
{
	u16  buffer[512];
	u16* startAddress;
	u16* endAddress;
	u16	 size_fixedData;
	u16  size_modifiedData;
}block_t;


static u8 flash_write(u16* address,u16 data);



#endif /* MCAL_FLASH_FLASH_PRIVATE_H_ */
