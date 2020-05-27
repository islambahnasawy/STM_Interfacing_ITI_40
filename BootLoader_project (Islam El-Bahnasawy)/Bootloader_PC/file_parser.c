#include <stdint.h>
#include <stdio.h>

#include "LIB/STD_TYPES.h"
#include "BL_Protocol/BL_Protocol.h"
#include "serial_Com/serial_Com.h"
#include "LIB/DELAY_interface.h"

#define file_name	"Applications_elf/STM_Project.elf"
#define file_name	"Applications_elf/LED_Toggle.elf"
#define file_name	"Applications_elf/BootLoader_APP_project.elf"
#define file_name	"Applications_elf/BootLoader_APP_project_1.elf"
#define file_name	"Applications_elf/LED_Toggle.elf"


#define DELAY 1000000

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
	uint8_t fileBuffer[1000000] = {0};
	/*A buffer that holds the data that will be sent*/
	uint8_t dataBuffer[1000000] = {0};
	u8 ACK_Buffer[8];
	uint32_t entryPoint=0;
	uint32_t headerSize=0;
	uint32_t PhysicalAddress=0;
	uint8_t i=0;
	uint32_t j=0;
	uint8_t headerNum=0;
	static u8 TxDataBuffer[1030];
	static u16 H_idx=0;
	
	u8* ptr_dataBuffer = TxDataBuffer;
	
	u8 *ptrTxDataBuffer = TxDataBuffer;
	u8 **pptrTxDataBuffer = &ptrTxDataBuffer;
	
	BL_Protocol_ACK_t* ptr_ackBuffer = (BL_Protocol_ACK_t*)ACK_Buffer;
	u16 frame_Length;
	
	uint8_t filename[50]= file_name;
	
	Elf32_Ehdr * elfHeader = (Elf32_Ehdr *) fileBuffer ;
	//printf("Enter .elf File path: ");
	//scanf("%s", &filename);
	
	FILE* fd = fopen(filename, "rb");

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
			entryPoint = elfHeader->e_entry;
			/*Initializing pHeader with the first header offset*/
			pHeader = ((pheader_t *) (&fileBuffer[elfHeader->e_phoff]));

			
			serialCom_init();

			for(i=0;i<headerNum;i++)
			{
				if(0x08000000 <pHeader[i].phyAddress && pHeader[i].phyAddress< 0x08010000)
				{
					printf("\n\n\n\n\n\n");
					//printf("Header (%d)\n", i);
					ptrTxDataBuffer = TxDataBuffer;
					pptrTxDataBuffer = &ptrTxDataBuffer;
					
					frame_Length = BL_Protocol_ConstructStartFrame(0x66666666, pHeader[i].phyAddress, entryPoint, pHeader[i].memSize, pptrTxDataBuffer);
					delay_mSec(DELAY);
					serialCom_sendData(ptr_dataBuffer, frame_Length);
					printf("Header (%d) sent, waiting for ACK\n", i);
					serialCom_receiveData(ACK_Buffer, 6);
					printf("ACK Received\n");
					
					while((ptr_ackBuffer->ack) != ACK)
					{
						printf("request(%d) ack failed", ptr_ackBuffer->header.REQ_No);
						ptrTxDataBuffer = TxDataBuffer;
						pptrTxDataBuffer = &ptrTxDataBuffer;
						
						frame_Length = BL_Protocol_ConstructStartFrame(0x66666666, pHeader[i].phyAddress, entryPoint, pHeader[i].memSize, pptrTxDataBuffer);
						delay_mSec(DELAY);
						serialCom_sendData(ptr_dataBuffer, frame_Length);
						printf("receiving new ACK : \n");
						serialCom_receiveData(ACK_Buffer, 6);
					}
					
					/*CHECK ACK*/
					
					
					headerSize = pHeader[i].memSize;
					PhysicalAddress = pHeader[i].phyAddress;
					/*resetting the header indexing value*/
					H_idx = 0;
					
					for(s32 k=headerSize; k>=0; H_idx+=1024, k-=1024)
					{
						printf("\n\n");
						printf("k= %d",k);
						printf("\n\n");
						
						ptrTxDataBuffer = TxDataBuffer;
						pptrTxDataBuffer = &ptrTxDataBuffer;
						if (k >= 1024)
						{
							frame_Length = BL_Protocol_ConstructDataFrame(1024, pptrTxDataBuffer);
							for(u16 B_idx=0; B_idx<1024; j++, B_idx++)
							{
								(ptr_dataBuffer)[B_idx+BL_PROTOCOL_HEADER_LENGTH] = fileBuffer[(pHeader[i].offset)+(B_idx+H_idx)];
								//printf("0x%02x ",(*pptrTxDataBuffer)[B_idx]);
								printf("0x%02x ",(ptr_dataBuffer)[B_idx+BL_PROTOCOL_HEADER_LENGTH]);
								dataBuffer[j] = fileBuffer[(pHeader[i].offset)+(B_idx+H_idx)];
							}
							delay_mSec(DELAY);
							serialCom_sendData(ptr_dataBuffer, frame_Length);
							printf("receiving ACK : \n");
							serialCom_receiveData(ACK_Buffer, 6);
							
							while((ptr_ackBuffer->ack) != ACK)
							{
								printf("request(%d) ack failed", ptr_ackBuffer->header.REQ_No);
								delay_mSec(DELAY);
								serialCom_sendData(ptr_dataBuffer, frame_Length);
								printf("receiving new ACK : \n");
								serialCom_receiveData(ACK_Buffer, 6);
							}
							
						}
						else if (k < 1024)
						{
							frame_Length = BL_Protocol_ConstructDataFrame((u16)k, pptrTxDataBuffer);
							for(u16 B_idx=0; B_idx<k; j++, B_idx++)
							{
								(ptr_dataBuffer)[B_idx+BL_PROTOCOL_HEADER_LENGTH] = fileBuffer[(pHeader[i].offset)+(B_idx+H_idx)];
								//printf("0x%02x ",(*pptrTxDataBuffer)[B_idx]);
								printf("0x%02x ",(ptr_dataBuffer)[B_idx+BL_PROTOCOL_HEADER_LENGTH]);
								dataBuffer[j] = fileBuffer[(pHeader[i].offset)+(B_idx+H_idx)];
							}
							delay_mSec(DELAY);
							serialCom_sendData(ptr_dataBuffer, frame_Length);
							printf("receiving ACK : \n");
							serialCom_receiveData(ACK_Buffer, 6);
							
							while((ptr_ackBuffer->ack) != ACK)
							{
								printf("request(%d) ack failed", ptr_ackBuffer->header.REQ_No);
								delay_mSec(DELAY);
								serialCom_sendData(ptr_dataBuffer, frame_Length);
								printf("receiving new ACK : \n");
								serialCom_receiveData(ACK_Buffer, 6);
							}						
						}
					}
				}			
			}
			ptrTxDataBuffer = TxDataBuffer;
			pptrTxDataBuffer = &ptrTxDataBuffer;
			
			frame_Length = BL_Protocol_ConstructStartFrame(0x44224422, 0x0, entryPoint, 0x0, pptrTxDataBuffer);
			delay_mSec(DELAY);
			serialCom_sendData(ptr_dataBuffer, frame_Length);
					
		}
			
		fclose(fd);
}

//http://www.thomasloven.com/blog/2013/08/Loading-Elf/
