#include <stdint.h>
#include <stdio.h>

#define file_name	"STM_Project.elf"

#define FLASH_START_ADDRESS	0x08000000
#define FLASH_END_ADDRESS	0x08010000

#define MAX_HEADER_SIZE		1000000
#define MAX_HEADER_NUM		10

typedef struct elf32_hdr{
  uint8_t	e_ident[16];
  uint16_t	e_type;
  uint16_t	e_machine;
  uint32_t	e_version;
  uint32_t	e_entry;  /* Entry point */
  uint32_t	e_phoff;
  uint32_t	e_shoff;
  uint32_t	e_flags;
  uint16_t	e_ehsize;
  uint16_t	e_phentsize;
  uint16_t	e_phnum;
  uint16_t	e_shentsize;
  uint16_t	e_shnum;
  uint16_t	e_shstrndx;
} Elf32_Ehdr;


typedef struct{
		uint32_t type;
		uint32_t offset;
		uint32_t virtualAddress;
		uint32_t phyAddress;
		uint32_t fileSize;
		uint32_t memSize;
		uint32_t flags;
		uint32_t align;
}pheader_t;

void main(void)
{
	/*A buffer to hold the entire file */
	uint8_t fileBuffer[MAX_HEADER_NUM][MAX_HEADER_SIZE] = {0};
	/*A buffer that holds the data that will be sent*/
	uint8_t dataBuffer[15000] = {0};
	uint32_t headerSize=0;
	uint32_t i=0;
	uint32_t j=0;
	uint32_t headerNum=0;
	Elf32_Ehdr * elfHeader = (Elf32_Ehdr *) fileBuffer ;
	
	FILE* fd = fopen(file_name, "rb");
		
	pheader_t * pHeader; // first Prog header
		
		if(fd)
		{
			/*Getting the total size of the elf file*/
			fseek(fd,0,SEEK_END);
			int size = ftell(fd);
			fseek(fd,0,SEEK_SET);			
			/*Saving the entire elf file in the buffer*/
			fread(fileBuffer, 1, size, fd);	
			/*Assining number of headers to headerNum*/
			headerNum = elfHeader->e_phnum;
			/*Initializing pHeader with the first header offset*/
			pHeader = ((pheader_t *) (&fileBuffer[elfHeader->e_phoff]));
			
			
			for(i=0;i<headerNum;i++)
			{
				printf("7amada of %d\n\n\n\n\n\n\n\n",i);
				headerSize = pHeader[i].memSize;
				for(uint32_t k=0;k<headerSize;j++,k++)
				{
					dataBuffer[j] = fileBuffer[(pHeader[i].offset)+k];
					printf("%02x ",fileBuffer[(pHeader[i].offset)+k]);
					//if(j%50 == 0) printf("\n");
				}
				//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
				//printf("the value of j is : %d\n" , j);
			}
			fclose(fd);
		}
		else
		{
			printf("file not found");
		}
}

