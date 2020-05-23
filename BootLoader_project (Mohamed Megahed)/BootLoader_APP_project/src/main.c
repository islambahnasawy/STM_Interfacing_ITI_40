#include "LIB/STD_TYPES.h"
#include "LIB/USART_MYTYPES.h"
#include "LIB/Delay/DELAY_interface.h"

#include "MCAL/RCC/RCC_interface.h"
#include "MCAL/DIO/DIO_interface.h"
#include "MCAL/Flash/Flash_interface.h"

#include "Other_Drivers/LED/LED_interface.h"
#include "HAL/LCD/C_LCD_interface.h"
#include "HAL/USART_Handler/USRTH_interface.h"
#include "Com_Protocols/BL_protocol/BL_protocol.h"

u8 BL_buffer[10];
packet_t BL_pack;

/*The beginning of the BootLoader data section in Linkerscript*/
extern unsigned int _myData;

keyData_t * pKeyData = (keyData_t *)&_myData;

static void send_Response(u8 response);
void BL_receiveReqFrame(void);
void receive_writeReqFrame(void);

int
main(int argc, char* argv[])
{
	/***********************************APP initialization*******************************************/
	trace_printf("App main\n");
	RCC_u8ChangeClockStatus(HSI_CLK,CLK_STATUS_ON);
	RCC_u8SwitchSystemClock(HSE);
	RCC_Peripheral_Config(RCC_IOPC,PERIPHERAL_ON);
	RCC_Peripheral_Config(RCC_IOPA,PERIPHERAL_ON);
	RCC_Peripheral_Config(RCC_IOPB,PERIPHERAL_ON);
	trace_printf("init RCC done\n");
	/****************************boot loader communication initialization****************************/
	USRTH_voidInit(USART1);
	DMA_voidInit();
	USRTH_DMAEnable(USART1);
	trace_printf("init comm done\n");
	LED_Init(LED1);
	LED_setLedState(LED1,LED_STATE_OFF);
	receive_writeReqFrame();
	/***************************************APP Code************************************************/
	while (1)
	{

		LED_setLedState(LED1,LED_STATE_ON);
		delay_mSec(2000);
		LED_setLedState(LED1,LED_STATE_OFF);
		delay_mSec(2000);

	}
}


/****************Code added to be able to flash a new program while this app is running**********************/
void receive_writeReqFrame(void)
{
	u16 length;
	BLProtocol_getFrameLength(WRITE_REQ_FRAME_ID,&length);

	USRTH_DMARecieve(USART1,BL_buffer,length,BL_receiveReqFrame);

}

void BL_receiveReqFrame(void)
{
	u8 local_status=OK;
	BL_pack.Frame_ID=WRITE_REQ_FRAME_ID;
	BL_pack.buffer=BL_buffer;
	/*Verifying the correct frame has been received*/
	local_status=BLProtocol_extractData(&BL_pack);
	if(local_status==OK)
	{
		trace_printf("GOING BACK TO BOOTLOADER\n");
		Flash_unlock();
		Flash_programWrite((u16*)&pKeyData->marker,(u16*)&BL_pack.key,4);
		send_Response(POSITIVE_RESPONSE);
		/*Wait till the response frame is sent before reseting*/
		delay_mSec(10000);
		RCC_SWReset();
	}
	else
	{
		send_Response(NEGATIVE_RESPONSE);
		/*Receive another write request frame (because the one before this failed)*/
		receive_writeReqFrame();
	}
}

static void send_Response(u8 response)
{
	u16 length;
	BL_pack.Frame_ID=RESPONSE_FRAME_ID;
	BL_pack.response=response;
	BL_pack.buffer = BL_buffer;
	BLProtocol_constructFrame(&BL_pack,&length);
	delay_mSec(1000);
	USRTH_DMASend(USART1,BL_buffer,length,NULL);
	trace_printf("response sent\n");
}

