/********************************************************/
/*Author : Mohamed Megahed , Islam El-Bahnasawy 		*/
/*Date   : 8 May 2020									*/
/*Version : V01											*/
/********************************************************/


#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "Flash_reg.h"
#include "Flash_interface.h"
#include "Flash_private.h"




void Flash_unlock(void)
{
	/*Performing Unlocking sequence according to data sheet */
	FLASH->FLASH_KEYR = KEY_1;
	FLASH->FLASH_KEYR = KEY_2;
}

void Flash_lock(void)
{
	if((FLASH->FLASH_CR & FLASH_CR_LOCK_MASK) != FLASH_CR_LOCK_MASK)
	{
		FLASH->FLASH_CR |= FLASH_CR_LOCK_MASK;
	}
}



u8 Flash_program(u32 address, u16 data)
{
	u16 data_RAM[SECTOR_SIZE_16BLKSIZE];
	u32 sector_base_address=0;
	u32 local_address = address;
	u16 counter =1;
	u8 local_status= OK;
	u16* local_data=&data;
	u16 idx =0;
	if((address%2) == 0)
	{
		/* Check if Flash interface is locked*/
		if((FLASH->FLASH_CR & FLASH_CR_LOCK_MASK) == FLASH_CR_LOCK_MASK)
		{
			/*Performing Unlocking sequence */
			Flash_unlock();
		}
		while((FLASH->FLASH_SR & FLASH_SR_BSY_MASK) == FLASH_SR_BSY_MASK);
		/* check if the desired address is erased before program */
		if(*((u16*)address) != 0xFFFF)
		{

			/*save data by transfer to RAM*/
			sector_base_address = address;
			sector_base_address &= FLASH_SECTOR_BASE_ADD_MASK;
			for (u16 i=0; i<SECTOR_SIZE_16BLKSIZE; i++)
			{
				data_RAM[i] = *(((u16*)sector_base_address)+i);
			}
			/*erase address */
			Flash_pageErase(address);

			/*modify the data in RAM*/
			idx = (address-sector_base_address)/2;
			data_RAM[idx] = data;

			counter = 512;
			local_address=sector_base_address;
			local_data = data_RAM;
		}
		for (u16 i=0; i<counter; i++)
		{
			if(*((u16*)local_address) == TWO_BYTES_CLEAR)
			{
				//trace_printf("Failing at %d\n",i);
				/*Write FLASH_CR_PG to 1*/
				FLASH->FLASH_CR |= FLASH_CR_PG_MASK;
				/* passing the address to Flash interface to allow program access */
				FLASH->FLASH_AR = local_address;
				/* assign input value to the desired address */
				*((u16*)local_address) = local_data[i];
				/*waiting for busy*/
				while((FLASH->FLASH_SR & FLASH_SR_BSY_MASK) == FLASH_SR_BSY_MASK);

				/*verify that data has been programmed successfully*/
				if(*((u16*)local_address) != local_data[i])
				{
					//trace_printf("Not ok at %d",i);
					local_status = NOT_OK;
					break;
				}
				local_address = (u32)((u16*)local_address+1);
			}
			else
			{
				//trace_printf("%x\n",local_address);
			}
		}
	}
	else
	{
		local_status = NOT_OK;
	}

	/*Clearing the PG bit*/
	FLASH->FLASH_CR &= ~FLASH_CR_PG_MASK;
	return local_status;

}

u8 Flash_massErase(void)
{
	u8 local_status = OK;
	u16 timeOut=0;
	if((FLASH->FLASH_CR & FLASH_CR_LOCK_MASK) != FLASH_CR_LOCK_MASK)
	{
		/* checking if Flash is ongoing operation by watching busy flag*/
		while((FLASH->FLASH_SR & FLASH_SR_BSY_MASK) == FLASH_SR_BSY_MASK);
		/*choose page erase option*/
		FLASH->FLASH_CR |= FLASH_CR_MER_MASK;
		/*start erasing operation*/
		FLASH->FLASH_CR |= FLASH_CR_STRT_MASK;
		/*waiting for busy flag to make sure that operation is done*/
		while(((FLASH->FLASH_SR & FLASH_SR_BSY_MASK) == FLASH_SR_BSY_MASK) && timeOut<TIMEOUT_LIMIT)
		{
			timeOut++;
		}
		if(timeOut>=TIMEOUT_LIMIT)
		{
			local_status = NOT_OK;
		}
		FLASH->FLASH_CR &= ~FLASH_CR_MER_MASK;
	}
	else
	{
		local_status = NOT_OK;
	}
	return local_status;
}


u8 Flash_pageErase(u32 address)
{

	u8 local_status = OK;
	if((FLASH->FLASH_CR & FLASH_CR_LOCK_MASK) != FLASH_CR_LOCK_MASK)
	{
		/* checking if Flash is ongoing operation by watching busy flag*/
		while((FLASH->FLASH_SR & FLASH_SR_BSY_MASK) == FLASH_SR_BSY_MASK);
		/*choose page erase option*/
		FLASH->FLASH_CR |= FLASH_CR_PER_MASK;
		/*assign the address to erase the page in which it locates*/
		FLASH->FLASH_AR = (address & FLASH_SECTOR_BASE_ADD_MASK);
		/*start erasing operation*/
		FLASH->FLASH_CR |= FLASH_CR_STRT_MASK;
		/*waiting for busy flag to make sure that operation is done*/
		while((FLASH->FLASH_SR & FLASH_SR_BSY_MASK) == FLASH_SR_BSY_MASK);

		/*verify that page/sector has been erased successfully*/
		for (u16 i=0; i<(SECTOR_SIZE_16BLKSIZE); i++)
		{
			if(*((u16*)address) != TWO_BYTES_CLEAR)
			{
				local_status = NOT_OK;
				//trace_printf("flash erase failed\n");
				break;
			}
		}
		/*Clearing the PER bit*/
		FLASH->FLASH_CR &= ~FLASH_CR_PER_MASK;
	}
	else
	{
		local_status=NOT_OK;
	}

	return local_status;
}

u8 Flash_programWrite(u16*start_address_Flash,u16* start_address_RAM , u32 numberOfBytes)
{
	u8 local_status=OK;
	if((FLASH->FLASH_CR & FLASH_CR_LOCK_MASK) != FLASH_CR_LOCK_MASK)
	{
		volatile block_t first_sector;
		volatile block_t last_sector;

		/*The last address where the data will be written*/
		u16* end_address_flash = start_address_Flash + (numberOfBytes/2)-1 ;
		/*The start_address_Flash sector base address */
		first_sector.startAddress= (u16*)((u32)start_address_Flash&FLASH_SECTOR_BASE_ADD_MASK);
		/*End address of the first sector*/
		first_sector.endAddress = (u16*)((u32)first_sector.startAddress | FLASH_SECTOR_LAST_ADD_MASK);
		/*The number of sectors where the data will be written*/
		u8 sector_num = ((end_address_flash - first_sector.startAddress)/SECTOR_SIZE_16BLKSIZE) +1;
		/*The size of the start sector that will be saved*/
		first_sector.size_fixedData=  (start_address_Flash - first_sector.startAddress);

		if(sector_num == 1)
		{
			/*The number of (2*bytes) that will be written in that single sector*/
			first_sector.size_modifiedData= numberOfBytes/2;
			/*The number of (2*bytes) that will be written in that single sector (won't be used)*/
			last_sector.size_modifiedData= numberOfBytes/2;
			/*The address from where we will start writing the data in the last sector (won't be used)*/
			last_sector.startAddress = start_address_Flash;
		}
		else
		{
			/*base address of the last sector*/
			last_sector.startAddress = (u16*)((u32)(end_address_flash)&FLASH_SECTOR_BASE_ADD_MASK);
			/*The number of (2*bytes) that will be written in the first sector*/
			first_sector.size_modifiedData= SECTOR_SIZE_16BLKSIZE-first_sector.size_fixedData;
			/*The number of (2*bytes) that will be written in the last sector*/
			last_sector.size_modifiedData= (end_address_flash - last_sector.startAddress)+1;

		}
		/*The end_address_flash sector last address*/
		last_sector.endAddress = (u16*)((u32)last_sector.startAddress|FLASH_SECTOR_LAST_ADD_MASK);
		/*The size of the end sector that will be saved*/
		last_sector.size_fixedData = (last_sector.endAddress-end_address_flash);

		u8 start_sector_flag = SECTOR_EMPTY;
		u8 end_sector_flag	= SECTOR_EMPTY;
		u16 * local_address = first_sector.startAddress;
		u16 RAM_idx =0;
		u16 j=0;


		/*Checking if the start sector is empty*/
		for (u16 i=0; i<first_sector.size_modifiedData; i++)
		{
			if(start_address_Flash[i] != TWO_BYTES_CLEAR)
			{
				start_sector_flag = SECTOR_FULL;
				//trace_printf("first sector flag\n");
				break;
			}
		}
		/*Checking if the end sector is empty*/
		for (u16 i=0; i<last_sector.size_modifiedData; i++)
		{
			if(last_sector.startAddress[i] != TWO_BYTES_CLEAR)
			{
				end_sector_flag = SECTOR_FULL;
				//trace_printf("the value of i :%d",i);
				//trace_printf("last sector flag\n");
				break;
			}
		}
		if(start_sector_flag == SECTOR_FULL)
		{
			/*Saving the data of the start sector*/
			for(u16 i=0;i<first_sector.size_fixedData;i++)
			{
				first_sector.buffer[i]=first_sector.startAddress[i];
			}
		}
		if(end_sector_flag == SECTOR_FULL)
		{
			/*Saving the data of the end sector*/
			for(u16 i=0;i<last_sector.size_fixedData;i++)
			{
				last_sector.buffer[i]=end_address_flash[i+1];
			}
		}
		/*Loop on sectors : erase them if they are not empty then write the fixed and modified data*/
		for(u8 i=0;i<sector_num;i++)
		{
			if(i==0 || i==(sector_num-1))
			{

				if(i==0)
				{
					/*in case of first sector is full*/
					if(start_sector_flag ==SECTOR_FULL)
					{
						/* data is already saved in RMA, so we can erase the sector*/
						local_status = Flash_pageErase((u32)local_address);
						if (local_status == OK)
						{
							/*restore the saved data of first sector from RAM into the flash Mem.*/
							for(j=0;j<first_sector.size_fixedData;j++)
							{
								local_status = flash_write(first_sector.startAddress+j,first_sector.buffer[j]);
								if(local_status==NOT_OK)
								{
									break;
								}
							}
						}
					}
					else
					{
						/*Do Nothing*/
					}
					/*in case the first sector was empty just write the passed data*/

					/*flash program the user data into the flash Mem.*/
					if(local_status == OK)
					{
						for(j=0;j<first_sector.size_modifiedData;j++)
						{
							local_status =flash_write(start_address_Flash+j,start_address_RAM[RAM_idx]);
							if(local_status==NOT_OK)
							{
								break;
							}
							RAM_idx++;
						}
					}
				}
				if(i==(sector_num-1))
				{
					/*in case of last sector*/
					if(end_sector_flag ==SECTOR_FULL)
					{
						/*In case of a single sector , we have already erased the sector in the previous condition if(i==0)*/
						if(sector_num != 1)
						{
							/* data is already saved in RMA, so we can erase the sector*/
							local_status = Flash_pageErase((u32)local_address);
						}
						if (local_status == OK)
						{
							/*restore the saved data of last sector from RAM into the flash Mem.*/
							for(j=0;j<last_sector.size_fixedData;j++)
							{
								local_status =flash_write(end_address_flash+j+1,last_sector.buffer[j]);
								if(local_status==NOT_OK)
								{
									break;
								}
							}
						}
					}
					else
					{
						/*Do nothing*/
					}
					if (local_status == OK)
					{
						/*In case of one sector , we have already written the modified data in the previous condition if(i==0)*/
						if(sector_num != 1)
						{
							/*continue flash programming the user data into the flash Mem.*/
							for(j=0;j<last_sector.size_modifiedData;j++)
							{
								local_status =flash_write(last_sector.startAddress+j,start_address_RAM[RAM_idx]);
								if(local_status==NOT_OK)
								{
									break;
								}
								RAM_idx++;
							}
						}
					}
				}
			}
			else
			{
				/*In case of a middle sector*/

				/*erase the current sector, to be able to flash program the user data */
				local_status = Flash_pageErase((u32)local_address);
				if (local_status == OK)
				{
					/*continue flash programming the user data into the flash Mem.*/
					for(j=0;j<SECTOR_SIZE_16BLKSIZE;j++)
					{
						local_status =flash_write(local_address+j,start_address_RAM[RAM_idx]);
						if(local_status==NOT_OK)
						{
							break;
						}
						RAM_idx++;
					}
				}
				if(local_status==NOT_OK)
				{
					break;
				}
			}
			local_address += SECTOR_SIZE_16BLKSIZE;
		}
	}
	else
	{
		local_status = NOT_OK;
	}
	return local_status;
}


static u8 flash_write(u16* address,u16 data)
{
	u8 local_status = OK;
	/*waiting for busy*/
	while((FLASH->FLASH_SR & FLASH_SR_BSY_MASK) == FLASH_SR_BSY_MASK);
	/*Write FLASH_CR_PG to 1*/
	FLASH->FLASH_CR |= FLASH_CR_PG_MASK;
	/* passing the address to Flash interface to allow program access */
	FLASH->FLASH_AR = (u32)address;
	/* assign input value to the desired address */
	*(address) = data;
	/*waiting for busy*/
	while((FLASH->FLASH_SR & FLASH_SR_BSY_MASK) == FLASH_SR_BSY_MASK);
	if(*((u16*)address) != data)
	{
		local_status = NOT_OK;
		//trace_printf("flash write failed\n");
	}
	/*Clearing the PG bit*/
	FLASH->FLASH_CR &= ~FLASH_CR_PG_MASK;
	return local_status;
}
