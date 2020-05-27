/********************************************************/
/*Author : Islam El-Bahnasawy					 		*/
/*Date   : 20 MAY 2020									*/
/*Version : V01											*/
/********************************************************/

#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/USART/USART_MYTYPES.h"
#include "../../LIB/Delay/DELAY_interface.h"

#include "../../RTOS/RTOS_interface.h"
#include "../../HAL/USART_Handler/USRTH_interface.h"
#include "../../HAL/LCD/C_LCD_interface.h"
#include "../../HAL/Switch/Switch_interface.h"
#include "../../MCAL/USART/USART_interface.h"
#include <stdio.h>
#include "../../BL_Com_Protocol/BL_Protocol.h"
#include "../../MCAL/Flash/Flash_interface.h"
#include "BL_APP_interface.h"


#define SCB_AIRCR *((volatile u32*)0xE000ED0C)
#define SOFTWARE_RESET	0x05fa0004

static void RCC_SWReset(void)
{
	u32 temp=0;
	temp |= SOFTWARE_RESET;
	SCB_AIRCR = temp;
}

static u8 TxDataBuffer[1030];
static u8 RxDataBuffer[1030];

static u8 TxACKBuffer[8];

volatile u8 Header1_data[1024]={0};
static BL_Frame_info_t current_frame;
static u16 *flash_PhyAddress;
static u16 *ram_startAddress;

static u32 sizeToReceive;
//static u8	currentHeader=0;


__attribute__((section(".myvars"))) volatile u32 marker = 0xAABBCCDD;
__attribute__((section(".myvars"))) volatile u32 entrypoint = 0x11223344;

volatile u32* ptr_marker = &marker;
volatile u32* ptr_entrypoint = &entrypoint;

u32 marker_Entry[2]={0};

typedef void (*jump)(void);




static void extract_SF_info(void)
{
	u8 local_Status = OK;
	u8 *ptrRxDataBuffer = RxDataBuffer;
	u8 ** pptrRxDataBuffer = &ptrRxDataBuffer;

	u16 dataLen=0;
	u16 ackFrameLength=0;
	local_Status = BL_Protocol_extractData(BL_STARTFRAME_ReqNum, pptrRxDataBuffer, &dataLen);

	if (local_Status == OK)
	{
		BL_Frame_info_t* Frame_info = (BL_Frame_info_t *)ptrRxDataBuffer;


		current_frame.key 			= Frame_info->key;
		current_frame.address 		= Frame_info->address;
		current_frame.entryPoint 	= Frame_info->entryPoint;
		current_frame.size 			= Frame_info->size;

		trace_printf(" key = %x", current_frame.key);
		trace_printf(" address = %x", current_frame.address);
		trace_printf(" entryPoint = %x", current_frame.entryPoint);
		trace_printf(" size = %x", current_frame.size);
		sizeToReceive = current_frame.size;
		flash_PhyAddress = (u16*)current_frame.address;
		trace_printf("\nsizeToReceive = %d\n",sizeToReceive);
		if((sizeToReceive>0) && (flash_PhyAddress!=0))
		{

			/*we send an ACK frame to trigger the PC to send another frame*/
			ackFrameLength = BL_Protocol_ConstructACKFrame(ACK, 0, TxACKBuffer);
			delay_mSec(3000);
			local_Status = USRTH_DMASend(USART1, TxACKBuffer, ackFrameLength, 0);
			receiveDataFrame();
			if (local_Status == OK)
			{
				trace_printf("\nay7agaaaa\n");

			}
			/*send receive request before sending ACK*/

			//trace_printf("ACK Sent\n");
		}
		else
		{
			if(current_frame.key == 0x44224422)
			{
				trace_printf("Last frame\n");
				/*jump to Entry point*/

				/*flash application exist value for marker
				 * flash entry point value
				 * then reset */
				marker_Entry[0]=current_frame.key;
				marker_Entry[1]=current_frame.entryPoint;
				//Flash_programWrite((u16*)ptr_marker, (u16*)marker_Entry, (2*4));

				Flash_programWrite((u16*)&marker, (u16*)marker_Entry, (2*4));
				/*reset micro-controller*/

				/*SCB_AIRCR&=0x0000FFFF;
				SCB_AIRCR|=0x5FA0000;
				SCB_AIRCR|=0b100;
				*/
				RCC_SWReset();
			}
		}
	}
	else
	{
		trace_printf("Error extracting First frame");
	}

}





static void extract_DF_info(void)
{
	u8 *ptrRxDataBuffer = RxDataBuffer;
	u8 ** pptrRxDataBuffer = &ptrRxDataBuffer;
	u8 local_Status = OK;
	u16 dataLen=0;
	u16 ackFrameLength=0;
	u8 reqNo=0;



	/*extract_DF_info is called after after reception complete
	 * first step, we get the request number of the received frame
	 * */
	reqNo = BL_Protocol_getReqNo(ptrRxDataBuffer);

	/*call BL_Protocol_extractData to extract the data if the request number as expected
	 * then move the pointer pptrRxDataBuffer to the start of data in the frame
	 * */
	local_Status = BL_Protocol_extractData(reqNo, pptrRxDataBuffer, &dataLen);
	//local_Status = BL_Protocol_extractData(1, pptrRxDataBuffer, &dataLen);

	if (local_Status == OK)
	{
		ram_startAddress = (u16*)ptrRxDataBuffer;
		/*if data is extracted successfully, then flash it on Flash Memory using flash driver*/
		trace_printf("Flash Address: 0x%x\t", flash_PhyAddress);
		trace_printf("current_frame.address: 0x%x\t", current_frame.address);
		trace_printf("RAM Address: 0x%x\n", ram_startAddress);
		for(u16 i=0; i<dataLen; i++)
		{
			/*store the data of the received frame into Header1_data*/
			Header1_data[i] = (ptrRxDataBuffer)[i];
			//trace_printf("0x%02x ", Header1_data[i]);
		}
		trace_printf("0x%02x 0x%02x ", (ptrRxDataBuffer)[0],(ptrRxDataBuffer)[dataLen-1]);
		trace_printf("\n");

		local_Status = Flash_programWrite((u16*)current_frame.address, (u16*)Header1_data, (u32)dataLen);
		if (local_Status == OK)
		{
			trace_printf("\nFlash done\n");
		}
		else
		{
			trace_printf("\nFlash failed\n");
		}

		/*move the flash_PhyAddress to the start of next flashing*/
		flash_PhyAddress+= dataLen;
		current_frame.address+= dataLen;





		/* Here we should flash the received frame in the flash memory using flash driver
		 *
		 *
		 * */


		if(dataLen<1024)
		{
			/*after flashing, we send an ACK frame to trigger the PC to send another frame*/
			ackFrameLength = BL_Protocol_ConstructACKFrame(ACK, reqNo, TxACKBuffer);
			delay_mSec(3000);
			USRTH_DMASend(USART1, TxACKBuffer, ackFrameLength, 0);
			/*send receive request before sending ACK*/
			receiveStartFrame();
			//USRTH_DMARecieve(USART1, RxDataBuffer, BL_STARTFRAME_SIZE, extract_SF_info);
			//trace_printf("Receiving First frame");
		}
		else
		{
			/*after flashing, we send an ACK frame to trigger the PC to send another frame*/
			ackFrameLength = BL_Protocol_ConstructACKFrame(ACK, reqNo, TxACKBuffer);
			delay_mSec(3000);
			USRTH_DMASend(USART1, TxACKBuffer, ackFrameLength, 0);
			/*send receive request before sending ACK*/
			receiveDataFrame();
		}
	}
	else
	{
		/*if extracting failed due to wrong reqNo, then we send NOT_ACK frame,
		 * to trigger the PC to re-send the frame of reqNo again */
		ackFrameLength = BL_Protocol_ConstructACKFrame(ACK, reqNo, TxACKBuffer);
		delay_mSec(3000);
		USRTH_DMASend(USART1, TxACKBuffer, ackFrameLength, 0);
		trace_printf("Error extracting First frame");
	}
	trace_printf("data extracted data frame");

}
void receiveStartFrame(void)
{
	u16 ackFrameLength=0;

	trace_printf("Receiving First frame");
	//currentHeader++;
	//	u8 frame_Length = AProtocol_getFrameLength(SWITCHES_SIGNAL_ID);
	USRTH_DMARecieve(USART1, RxDataBuffer, BL_STARTFRAME_SIZE, extract_SF_info);
	//USART_u8RecDataAsync(USART1,RxDataBuffer , BL_STARTFRAME_SIZE ,extract_DF_info , NULL);
	//USRTH_u8ReceivingReq(USART1, RxDataBuffer, BL_STARTFRAME_SIZE , extract_DF_info);

}

void receiveDataFrame()
{

	u16 frame_length = BL_PROTOCOL_HEADER_LENGTH + BL_PROTOCOL_EOF_LENGTH;

	if (sizeToReceive > 1024)
	{
		trace_printf("Receiving data frame");
		frame_length+= 1024;
		//USART_u8RecDataAsync(USART1,RxDataBuffer , 1024 ,extract_DF_info , NULL);
		//USRTH_u8ReceivingReq(USART1, RxDataBuffer, 1024 , extract_DF_info);
		USRTH_DMARecieve(USART1, RxDataBuffer, frame_length, extract_DF_info);
		sizeToReceive-=1024;
	}
	else
	{
		trace_printf("Receiving data frame");
		frame_length+= sizeToReceive;
		//USRTH_u8ReceivingReq(USART1, RxDataBuffer, sizeToReceive , extract_DF_info);
		//USART_u8RecDataAsync(USART1,RxDataBuffer , sizeToReceive ,extract_DF_info , NULL);
		USRTH_DMARecieve(USART1, RxDataBuffer, frame_length, extract_DF_info);

	}
	//	u8 frame_Length = AProtocol_getFrameLength(SWITCHES_SIGNAL_ID);

}




