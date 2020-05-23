/********************************************************/
/*Author : Mohamed Megahed 								*/
/*Date   : 16 May 2020									*/
/*Version : V01											*/
/********************************************************/

#include "LIB/STD_TYPES.h"
#include "LIB/USART_MYTYPES.h"
#include "LIB/Delay/DELAY_interface.h"

#include "HAL/USART_Handler/USRTH_interface.h"
#include "MCAL/DMA/DMA_interface.h"
#include "MCAL/RCC/RCC_interface.h"
#include "HAL/LED/LED_interface.h"
#include "Com_Protocols/BL_protocol/BL_protocol.h"
#include "MCAL/Flash/Flash_interface.h"

#define SCB_VTOR *((u32*)0xE000ED08)
#define DONE 			5
#define RECEIVING		7

#define WAITING 		20
#define PROGRAM_FLASHED	25

#define MAX_BUFFER_SIZE	1029
#define MAX_SECTOR_SIZE 1024

#define MAX_FAULT_NUM	5


static u8 buffer[MAX_BUFFER_SIZE];
static packet_t pack;
static u8 status=WRITE_REQ_FRAME_ID;
static s8 sector_idx=0;
static u8 numOfSectors=0;
static u8 datRecFlag=DONE;

static u8 fault_counter;
static u8 header_counter;

/*The beginning of the BootLoader data section in Linkerscript*/
extern unsigned int _myData;

keyData_t * pKeyData = (keyData_t *)&_myData;
/*Initializing the BootLoader data section with NO_PORGRAM_EXISTS values*/
__attribute__ ((section(".BL_Data"),used))
keyData_t keyData =
{
	.marker=NO_PORGRAM_EXISTS,
	.startAdress=NO_PORGRAM_EXISTS,
	.entryPoint=NO_PORGRAM_EXISTS
};

/*Prototypes of used call back functions*/
static void BL_receiveDataFrame(void);
static void BL_receiveStartFrame(void);
static void BL_receiveReqFrame(void);
static void BL_receiveLastHeaderFrame(void);
static void send_Response(u8 response);
void BL_ComSequence(void);

int
main(int argc, char* argv[])
{
	/*if an application exists jump to it*/
	if(pKeyData->marker==NEW_PROGRAM_FLASHED)
	{
		trace_printf("jumping  app\n");
		/*Initializing stack pointer to the top of the stack*/
		//asm("MSR MSP,r0");
		/*Setting the vector offset to the start of the application's vector table*/
		//SCB_VTOR = *((u32*)START_ADDRESS);
		SCB_VTOR = pKeyData->startAdress;
		pack.entryPoint=pKeyData->entryPoint;
		trace_printf("entry point is %x",pack.entryPoint);
		/*Jumping to the application's entry point (startup code)*/
		((Handler_t)pack.entryPoint)();
	}
	/*if no application exists initialize the communication and flash drivers and start the BootLoader sequence*/
	else
	{
		/*System clock initialization*/
		RCC_u8SwitchSystemClock(HSE);
		/*Flash driver initialization*/
		Flash_unlock();
		/*Communication initialization*/
		USRTH_voidInit(USART1);
		DMA_voidInit();
		USRTH_DMAEnable(USART1);
		while (1)
		{
			BL_ComSequence();
		}
	}
}

void BL_ComSequence(void)
{
	u16 length=0;
	switch(status)
	{
	case WRITE_REQ_FRAME_ID:
		/*
		 * Checking if we reached the boot loader via receiving the write request frame from APP
		 * In this case skip to the START_FRAME_ID status
		 * */
		if(pKeyData->marker == FLASH_NEW_PROGRAM)
		{
			status = START_FRAME_ID;
		}
		else
		{
			trace_printf("request frame\n");
			/*Change status to waiting till the whole frame is received at the callback function is executed*/
			status = WAITING;
			/*Getting the length of the write request frame*/
			BLProtocol_getFrameLength(WRITE_REQ_FRAME_ID,&length);
			/*Receiving a number of (length) bytes via uart*/
			USRTH_DMARecieve(USART1,buffer,length,BL_receiveReqFrame);
		}
		break;
	case START_FRAME_ID:
		trace_printf("start frame\n");
		/*Change status to waiting till the whole frame is received at the callback function is executed*/
		status = WAITING;
		/*Getting the length of the start frame*/
		BLProtocol_getFrameLength(START_FRAME_ID,&length);
		/*Receiving a number of (length) bytes via uart*/
		USRTH_DMARecieve(USART1,buffer,length,BL_receiveStartFrame);
		break;
	case DATA_FRAME_ID :
		trace_printf("Data frame\n");
		/*Change status to waiting till the whole frame is received at the callback function is executed*/
		status = WAITING;
		/*Getting the length of the overhead of the data frame*/
		BLProtocol_getFrameLength(DATA_FRAME_ID,&length);
		/*length of the last data sector*/
		length += pack.size%MAX_SECTOR_SIZE;
		/*We have the total size of the data so we can calculate the number of sectors it will occupy*/
		if(pack.size!=0)
		{
			numOfSectors=((pack.size-1)/MAX_SECTOR_SIZE)+1;
		}
		else
		{
			numOfSectors =  1;
		}
		for(sector_idx=0;sector_idx<numOfSectors;sector_idx++)
		{
			datRecFlag=RECEIVING;
			if(sector_idx == numOfSectors-1)
			{
				USRTH_DMARecieve(USART1,buffer,length,BL_receiveDataFrame);
			}
			else
			{
				USRTH_DMARecieve(USART1,buffer,MAX_BUFFER_SIZE,BL_receiveDataFrame);
			}
			/*wait tell data is received*/
			while(datRecFlag==RECEIVING);
		}
		break;
	case LAST_HEADER_FRAME_ID :
		trace_printf("Last header frame\n");
		/*Change status to waiting till the whole frame is received at the callback function is executed*/
		status = WAITING;
		/*Getting the length of the start frame*/
		BLProtocol_getFrameLength(LAST_HEADER_FRAME_ID,&length);
		/*Receiving a number of (length) bytes via uart*/
		USRTH_DMARecieve(USART1,buffer,length,BL_receiveLastHeaderFrame);
		break;
	case PROGRAM_FLASHED:
		pack.key=NEW_PROGRAM_FLASHED;
		/*change the marker and reset*/
		Flash_programWrite((u16*)&pKeyData->marker,(u16*)&pack.key,4);
		delay_mSec(10000);
		RCC_SWReset();
		break;
	case WAITING:
		/*Do Nothing*/
		break;

	}
}

static void BL_receiveReqFrame(void)
{
	trace_printf("req int\n");
	u8 local_status;
	pack.Frame_ID=WRITE_REQ_FRAME_ID;
	pack.buffer=buffer;
	/*Verifying the correct frame has been received*/
	local_status = BLProtocol_extractData(&pack);
	/*In case the wrong ID was received try to receive it again*/
	if(local_status == NOT_OK)
	{
		trace_printf("failed\n");

		status = WRITE_REQ_FRAME_ID;
		USRTH_DMADisable(USART1);
		USRTH_DMAStop(USART1);
		USRTH_DMAEnable(USART1);

		/*Send negative response*/
		send_Response(NEGATIVE_RESPONSE);
		fault_counter++;
		if(fault_counter==MAX_FAULT_NUM)
		{
			/*Reset in case of MAX_FAULT_NUM of faults*/
			RCC_SWReset();
		}
	}
	/*if the ID was right go to the next step*/
	else
	{
		fault_counter=0;
		trace_printf("success\n");
		status = START_FRAME_ID;
		/*Send positive response*/
		send_Response(POSITIVE_RESPONSE);
		trace_printf("The value of the key is : 0x%x",pack.key);
	}
}


static void BL_receiveStartFrame(void)
{
	trace_printf("start int\n");
	u8 local_status;
	pack.Frame_ID=START_FRAME_ID;
	pack.buffer=buffer;
	/*Verifying the correct frame has been received*/
	local_status = BLProtocol_extractData(&pack);
	/*In case the wrong ID was received try to receive it again*/
	if(local_status == NOT_OK)
	{
		trace_printf("failed\n");
		status = START_FRAME_ID;
		/*Reseting the DMA*/
		USRTH_DMADisable(USART1);
		USRTH_DMAStop(USART1);
		USRTH_DMAEnable(USART1);
		/*Send negative response*/
		send_Response(NEGATIVE_RESPONSE);
		fault_counter++;
		if(fault_counter==MAX_FAULT_NUM)
		{
			/*Reset in case of MAX_FAULT_NUM of faults*/
			RCC_SWReset();
		}
	}
	/*if the ID was right go to the next step*/
	else
	{
		trace_printf("success\n");
		fault_counter=0;
		status = DATA_FRAME_ID;
		/*Send positive response*/
		send_Response(POSITIVE_RESPONSE);
		if(header_counter==0)
		{
			/*Saving the first startAdress in flash so it won't be erased after  the reset*/
			Flash_programWrite((u16*)&pKeyData->startAdress,(u16*)&pack.startAdress,4);
			/*Saving the first entryPoint in flash so it won't be erased after  the reset*/
			Flash_programWrite((u16*)&pKeyData->entryPoint,(u16*)&pack.entryPoint,4);
		}
		header_counter++;
		trace_printf("add:%lx entry:%lx size:%lx",pack.startAdress,pack.entryPoint,pack.size);
	}
}

static void BL_receiveDataFrame(void)
{
	trace_printf("data int\n");
	u8 local_status;
	/*To breaking the busy loop that is waiting on the receipting of the data*/
	datRecFlag=DONE;
	pack.Frame_ID=DATA_FRAME_ID;
	pack.buffer=buffer;
	/*Verifying the correct frame has been received*/
	local_status = BLProtocol_extractData(&pack);

	if(local_status == OK)
	{
		trace_printf("success\n");
		fault_counter=0;
		/*In case all sectors were flashed*/
		if(sector_idx == numOfSectors-1)
		{
			/*Use flash driver to burn the data (using start address ,size)*/
			local_status=Flash_programWrite((u16*)pack.startAdress,(u16*)pack.data,pack.size);
			status = LAST_HEADER_FRAME_ID;
		}
		/*In case not all sectors were flashed*/
		else
		{
			/*Use flash driver to burn the data (using start address ,size)*/
			local_status=Flash_programWrite((u16*)pack.startAdress,(u16*)pack.data,MAX_SECTOR_SIZE);
			pack.size-=MAX_SECTOR_SIZE;
			pack.startAdress+=MAX_SECTOR_SIZE;
			status = DATA_FRAME_ID;
		}
		/*Send positive response*/
		send_Response(POSITIVE_RESPONSE);
	}
	if(local_status == NOT_OK)
	{
		trace_printf("failed\n");
		fault_counter++;
		status = DATA_FRAME_ID;
		/*Receive that data block again*/
		sector_idx--;
		/*Reseting the DMA*/
		USRTH_DMADisable(USART1);
		USRTH_DMAStop(USART1);
		USRTH_DMAEnable(USART1);
		/*Send negative response*/
		send_Response(NEGATIVE_RESPONSE);
		if(fault_counter==MAX_FAULT_NUM)
		{
			/*Reset in case of MAX_FAULT_NUM of faults*/
			RCC_SWReset();
		}
	}
}

static void BL_receiveLastHeaderFrame(void)
{
	trace_printf("last header int\n");
	u8 local_status;
	pack.Frame_ID=LAST_HEADER_FRAME_ID;
	pack.buffer=buffer;
	/*Verifying the correct frame has been received*/
	local_status = BLProtocol_extractData(&pack);
	/*In case the wrong ID was received try to receive it again*/
	if(local_status == NOT_OK)
	{
		trace_printf("failed\n");

		status = LAST_HEADER_FRAME_ID;
		/*Reseting the DMA*/
		USRTH_DMADisable(USART1);
		USRTH_DMAStop(USART1);
		USRTH_DMAEnable(USART1);
		/*Send negative response*/
		send_Response(NEGATIVE_RESPONSE);
		fault_counter++;
		if(fault_counter==MAX_FAULT_NUM)
		{
			/*Reset in case of MAX_FAULT_NUM of faults*/
			RCC_SWReset();
		}
	}
	/*if the ID was right go to the next step*/
	else
	{
		trace_printf("success\n");
		fault_counter=0;
		/*In case there was another header incoming*/
		if(pack.header_indicator==HEADER_FRAME_INCOMMING)
		{
			status = START_FRAME_ID;
		}
		/*In case this was the last header*/
		else if(pack.header_indicator==HEADER_FRAME_END)
		{
			status = PROGRAM_FLASHED;
		}
		/*In case the byte was corrupted*/
		else
		{
			send_Response(NEGATIVE_RESPONSE);
		}
		/*Send positive response*/
		send_Response(POSITIVE_RESPONSE);
	}
}

static void send_Response(u8 response)
{
	u16 length;
	pack.Frame_ID=RESPONSE_FRAME_ID;
	pack.response=response;
	pack.buffer = buffer;
	BLProtocol_constructFrame(&pack,&length);
	delay_mSec(1000);
	USRTH_DMASend(USART1,buffer,length,NULL);
	trace_printf("response sent\n");
}

