/********************************************************/
/*Author : Mohamed Megahed 								*/
/*Date   : 16 May 2020									*/
/*Version : V01											*/
/********************************************************/

#include "LIB/STD_TYPES.h"
#include <stdio.h>
#include "Communication/PCcom.h"
#include "BL_protocol/BL_protocol.h"
#include "LIB/DELAY_interface.h"

#define SECTOR_SIZE 	1024

#define DATA_SIZE 		1088

#define SEND_DELAY 		500000

#define MAX_FAULT_NUM	5

#define FAULT_OCCURED	0x16
#define NO_FAULT		0x17

static void send_WriteReqFrame(void);
static u8 recieve_ResponseFrame(void);
static void send_StartFrame(u32 startAdress,u32 entryPoint , u32 size);
static void send_LastHeaderFrame(u8 header_status);
static void send_DataFrame(u8* data,u16 length);

static u8 buffer[1029];
static u16 length;
static u16 rec_length;
static packet_t pack;
static u8 fault_counter;
static u8 fault_flag=NO_FAULT;

#include <stdint.h>
#include <stdio.h>

#define LOAD 1
#define FLASH_START_ADDRESS 0x08001000
#define FLASH_END_ADDRESS   0x08010000

#define file_name	"D:/ARM_Training/workSpace/BootLoader_APP_project/Debug/BootLoader_APP_project.elf"
//#define file_name "LED_Toggle.elf"
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

/*A buffer to hold the entire file */
uint8_t fileBuffer[1000000] = {0};
/*A buffer that holds the data that will be sent*/
uint8_t dataBuffer[100000] = {0};

void main(void)
{

	uint32_t headerSize=0;
	uint32_t Header_lastAdress;
	uint8_t header_idx=0;
	uint32_t j=0;
	uint8_t headerNum=0;
	Elf32_Ehdr * elfHeader = (Elf32_Ehdr *) fileBuffer ;
	FILE* fd = fopen(file_name, "rb");
	pheader_t * pHeader; // first Prog header
	u8 sectorNum=0;
	u8 local_status=OK;
	u16 rem_length=0;
	serial_Init();
	if(fd)
	{
		/*Getting the total size of the elf file*/
		fseek(fd,0,SEEK_END);
		u32 size = ftell(fd);
		fseek(fd,0,SEEK_SET);
		/*Saving the entire elf file in the buffer*/
		fread(fileBuffer, 1, size, fd);
		/*Assining number of headers to headerNum*/
		headerNum = elfHeader->e_phnum;
		/*Initializing pHeader with the first header offset*/
		pHeader = ((pheader_t *) (&fileBuffer[elfHeader->e_phoff]));
		
		
		/*Sending the write request frame till a positive response is received */
		while(1)
		{
			send_WriteReqFrame();
			local_status=recieve_ResponseFrame();
			if(local_status==OK)
			{
				/*Checking for poitive response*/
				if(pack.response == POSITIVE_RESPONSE)
				{
					fault_counter=0;
					break;
				}
				else if (pack.response == NEGATIVE_RESPONSE)
				{
					/*Send the frame again and wait for the response*/
					fault_counter++;
					if(fault_counter==MAX_FAULT_NUM)
					{
						/*Reset in case of MAX_FAULT_NUM of faults*/
						fault_flag=FAULT_OCCURED;
						break;
					}					
				}
				printf("response for write request frame = 0x%x\n",pack.response);
			}
		}
		//printf("first step done\n");
		if(fault_flag==NO_FAULT)
		{
			for(header_idx=0;header_idx<headerNum;header_idx++)
			{
				Header_lastAdress = pHeader[header_idx].phyAddress + pHeader[header_idx].memSize;
				if(pHeader[header_idx].type==LOAD &&\
				pHeader[header_idx].phyAddress>=FLASH_START_ADDRESS\
				&&Header_lastAdress<FLASH_END_ADDRESS)
				{
					for(uint32_t k=0;k<pHeader[header_idx].memSize;k++)
					{	if(k%4 == 0) printf(" ");
						if(k%16 == 0) printf("\n");
						dataBuffer[k] = fileBuffer[(pHeader[header_idx].offset)+k];
						printf("%02x",dataBuffer[k]);

						if(k%1025 == 0) printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
					}
					//printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
					/************************************************************************************/
					/*Sending the start frame till a positive response is received */
					while(1)
					{
						send_StartFrame(pHeader[header_idx].phyAddress,elfHeader->e_entry,pHeader[header_idx].memSize);
						local_status=recieve_ResponseFrame();
						if(local_status==OK)
						{
							/*Checking for poitive response*/
							if(pack.response == POSITIVE_RESPONSE)
							{
								fault_counter=0;
								break;
							}
							else if (pack.response == NEGATIVE_RESPONSE)
							{
								fault_counter++;
								if(fault_counter==MAX_FAULT_NUM)
								{
									/*Reset in case of MAX_FAULT_NUM of faults*/
									fault_flag=FAULT_OCCURED;
									break;
								}
							}
							printf("response for start frame = 0x%x\n",pack.response);
						}	
					}
					if(fault_flag==FAULT_OCCURED)
					{
						break;
					}
					//printf("second step done\n");
					
					sectorNum=(u8)((pHeader[header_idx].memSize-1)/SECTOR_SIZE) + 1;
					rem_length=pHeader[header_idx].memSize%SECTOR_SIZE;
					/*Sending the data frames and waiting for the positive response*/
					for(s8 sectorIdx=0;sectorIdx<sectorNum;sectorIdx++)
					{
						printf("sector idx %d\n",sectorIdx);
						if(sectorIdx==(sectorNum-1))
						{
							send_DataFrame(dataBuffer+(sectorIdx*SECTOR_SIZE),rem_length);
						}
						else
						{
							send_DataFrame(dataBuffer+(sectorIdx*SECTOR_SIZE),SECTOR_SIZE);
						}
						local_status=recieve_ResponseFrame();
						if(local_status!=OK || pack.response != POSITIVE_RESPONSE)
						{
							//handeled only negative response, the wrong response not handeled yet
							if(pack.response==NEGATIVE_RESPONSE)
							{
								fault_counter++;
								if(fault_counter==MAX_FAULT_NUM)
								{
									/*Reset in case of MAX_FAULT_NUM of faults*/
									fault_flag=FAULT_OCCURED;
									break;
								}
							}
							sectorIdx--;
						}
						else
						{
							fault_counter=0;
						}
						printf("response for data frame = 0x%x\n",pack.response);
					}
					if(fault_flag==FAULT_OCCURED)
					{
						break;
					}
				}
				/*In case the next header is not the last one, and is not valid , skipt it*/
				for(u8 i=1;i<headerNum;i++)
				{
					Header_lastAdress = pHeader[header_idx+1].phyAddress + pHeader[header_idx+1].memSize;
					if(header_idx<(headerNum-1)&&\
					(pHeader[header_idx+1].type!=LOAD ||\
					pHeader[header_idx+1].phyAddress<FLASH_START_ADDRESS||\
					Header_lastAdress>FLASH_END_ADDRESS))
					{
						header_idx++;
					}
					else
					{
						break;
					}
				}
				/*Sending the write request frame till a positive response is received */
				while(1)
				{
					if(header_idx != (headerNum-1))
					{
						send_LastHeaderFrame(HEADER_FRAME_INCOMMING);
					}
					else
					{
						send_LastHeaderFrame(HEADER_FRAME_END);
					}
					local_status=recieve_ResponseFrame();
					if(local_status==OK)
					{
						/*Checking for poitive response*/
						if(pack.response == POSITIVE_RESPONSE)
						{
							fault_counter=0;
							break;
						}
						else if (pack.response == NEGATIVE_RESPONSE)
						{
							fault_counter++;
							if(fault_counter==MAX_FAULT_NUM)
							{
								/*Reset in case of MAX_FAULT_NUM of faults*/
								fault_flag=FAULT_OCCURED;
								break;
							}
						}
						printf("response for last header frame = 0x%x\n",pack.response);
					}
				}
				if(fault_flag==FAULT_OCCURED)
				{
					break;
				}
			}
		}
		fclose(fd);
	}
	else
	{
		printf("file not found");
	}
}


static void send_WriteReqFrame(void)
{
	/*Preparing the frame to be sent*/
	pack.Frame_ID=WRITE_REQ_FRAME_ID;
	pack.buffer =buffer;
	pack.key=FLASH_NEW_PROGRAM;
	BLProtocol_constructFrame(&pack,&length);
	printf("\n");
	delay_mSec(SEND_DELAY);
	/*Sending the buffer*/
	serial_Send(buffer,length);
	printf("write request frame sent\n");
}

static void send_StartFrame(u32 startAdress,u32 entryPoint , u32 size)
{
	/*Preparing the frame to be sent*/
	pack.Frame_ID=START_FRAME_ID;
	pack.startAdress=startAdress;
	pack.entryPoint=entryPoint;
	pack.size = size;
	pack.buffer =buffer;
	BLProtocol_constructFrame(&pack,&length);
	printf("\n");
	/*Sending the buffer*/
	delay_mSec(SEND_DELAY+2000000);
	serial_Send(buffer,length);
	for(u8 i=0;i<20 ;i++)
	{
		printf("%x ",buffer[i]);
	}
	printf("\nstart frame sent\n");
}

static u8 recieve_ResponseFrame(void)
{
	u8 local_status = OK;
	/*Perparing to receive the frame*/
	BLProtocol_getFrameLength(RESPONSE_FRAME_ID,&rec_length);
	/*Receiving the frame*/
	serial_Receive(buffer,rec_length);

	printf("received response\n");
	for(u8 i=0;i<rec_length ;i++)
	{
		printf("%x ",buffer[i]);
	}
	printf("\n");
	pack.Frame_ID=RESPONSE_FRAME_ID;
	pack.buffer=buffer;
	/*Checking if the frame is valid*/
	local_status = BLProtocol_extractData(&pack);
	//printf("response ID is :0x%x\n",pack.Frame_ID);
	return local_status;
}

static void send_DataFrame(u8* data,u16 length)
{
	/*Preparing the frame to be sent*/
	pack.Frame_ID=DATA_FRAME_ID;
	pack.data=data;
	pack.buffer =buffer;
	pack.data_length=length;
	BLProtocol_constructFrame(&pack,&length);
	printf("the length is %d\n",length);
	for(u8 i=0;i<20 ;i++)
	{
		printf("%x ",buffer[i]);
	}
	printf("\n");
	/*Sending the buffer*/
	delay_mSec(SEND_DELAY);
	serial_Send(buffer,length);
	printf("data frame sent\n");
}

static void send_LastHeaderFrame(u8 header_status)
{
	u16 length;
	/*Preparing the frame to be sent*/
	pack.Frame_ID=LAST_HEADER_FRAME_ID;
	pack.header_indicator=header_status;
	pack.buffer =buffer;
	BLProtocol_constructFrame(&pack,&length);
	/*Sending the buffer*/
	delay_mSec(SEND_DELAY);
	serial_Send(buffer,length);
	printf("last header frame sent\n");
}
