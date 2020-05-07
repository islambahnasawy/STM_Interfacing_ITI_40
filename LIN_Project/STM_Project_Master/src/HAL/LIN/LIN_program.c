/********************************************************/
/*Author : Mohamed Megahed ,Islam El-Bahnasawy          */
/*Date   : 5 May 2020									*/
/*Version : V01											*/
/********************************************************/
#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/USART/USART_MYTYPES.h"

#include "../../MCAL/RCC/RCC_interface.h"
#include "../../MCAL/DIO/DIO_interface.h"
#include "../../MCAL/NVIC/NVIC_interface.h"
#include "../../MCAL/USART/USART_interface.h"

#include "../../RTOS/RTOS_interface.h"

#include "LIN_interface.h"
#include "LIN_private.h"
#include "LIN_cfg.h"

const  task_t Master_task = {.handler=LIN_MasterTask , .preodicityMS=6 };
const task_t Slave_task  = {.handler=LIN_SlaveTask , .preodicityMS= 1 };

static LIN_Msg_t LIN_Msg[MESSAGES_NUM];

static USART_t* LIN_USART=USART1_ADDRESS;

static u8 Slave_Header[NUM_OF_HEADER_BYTES];
static u8 Master_Header[NUM_OF_HEADER_BYTES];

static u8 LIN_databuffer[MESSAGES_NUM][8];
static u8 LIN_masterStatus = MASTER_IDLE;
static u8 LIN_slaveStatus =SLAVE_IDLE;
static u8 checksum=0;

void LIN_init(USART_t* USART)
{
	LIN_USART = USART;
	/*Enabling the USART clock through RCC*/
	RCC_Peripheral_Config(RCC_USART1,PERIPHERAL_ON);
	/*Enabling the GPIOA clock through RCC*/
	RCC_Peripheral_Config(RCC_IOPA,PERIPHERAL_ON);
	/*Configuring the Tx and Rx GPIO pins*/
	GPIO_t gpio = {GPIO_PIN_9&GPIO_PIN_10,PIN9_MODE_AF_OPENDRAIN_SPEED_10MHZ|PIN10_MODE_IP_PULLUP,GPIO_PORTA};
	GPIO_Config(&gpio);
	/*Enabling the USART1 interrupt in NVIC*/
	NVIC_EnableIRQ(NVIC_USART1);
	/*Getting the current APB2 frequency in HZ*/
	u32 freq = RCC_u32ReadAPB2Freq();
	/* Setting call back function to change status flags if a break sent */
	set_LIN_BreakCB(LIN_Break_detected);
	//USART_voidInit(USART,freq);
	USART_LINInit(USART,freq);

}



void LIN_masterInit(void)
{
	for(u8 i=0; i<MESSAGES_NUM; i++)
	{
		/* Passing msg ID of LIN_cfg_Arr to LIN_Msg arr */
		LIN_Msg[i].msg_ID = LIN_cfg_Arr[i].ID;

		/* converting frame or Msg slot from msec to Ticks based on the mini slot value
		 * and assign ticks to execute value of msg = the following msg's execution time
		 * in LIN_Msg arr,
		 * the first msg is assigned to the last msg execution time.
		 * */
		if(i==0)
		{
			LIN_Msg[i].TicksToExec 	= (LIN_cfg_Arr[MESSAGES_NUM-1].execTime_ms)/MINI_SLOT;
			LIN_Msg[i].TicksInitValue 	= LIN_Msg[i].TicksToExec;
		}
		else
		{
			LIN_Msg[i].TicksToExec 	= (LIN_cfg_Arr[i-1].execTime_ms)/MINI_SLOT;
			LIN_Msg[i].TicksInitValue 	= LIN_Msg[i].TicksToExec;
		}
	}
}


static void LIN_MasterTask(void)
{


	static u8 idx=0;
	u16 timeout=0;

	if(LIN_Msg[idx].TicksToExec==0)
	{
		/* Send break condition via USRAT */
		USART_LINsendBreak(LIN_USART);
		//		USART_u8RecDataAsync(LIN_USART,&idx,2,NULL,NULL);
		//		USART_u8SendingDataASync(LIN_USART,&idx,2,NULL,NULL);
		/* Waiting till the break condition is sent successfully or time out */
		while(LIN_masterStatus != MASTER_SENT_BREAK && timeout <= 1000) timeout++;



		if(timeout<1000)
		{
			asm("NOP");
			/* calculate the parity bits and modify the PID (Protected ID)*/
			LIN_Msg[idx].msg_ID |= (((LIN_Msg[idx].msg_ID>>BIT_0) ^ (LIN_Msg[idx].msg_ID>>BIT_1) ^
					(LIN_Msg[idx].msg_ID>>BIT_2) ^ (LIN_Msg[idx].msg_ID>>BIT_4))<<P0)&P1_CLEAR_MASK;

			LIN_Msg[idx].msg_ID |= ~ (  (LIN_Msg[idx].msg_ID>>BIT_1) ^ (LIN_Msg[idx].msg_ID>>BIT_3)  ^
					(LIN_Msg[idx].msg_ID>>BIT_4) ^ (LIN_Msg[idx].msg_ID>>BIT_5) )<<P1;

			Master_Header[SYNC] = SYNC_BYTE;
			Master_Header[PID] = LIN_Msg[idx].msg_ID;

			USART_DMASend(LIN_USART,Master_Header , NUM_OF_HEADER_BYTES , NULL );
		}

		LIN_Msg[idx].TicksToExec = LIN_Msg[idx].TicksInitValue;

		LIN_masterStatus = MASTER_IDLE;

		idx++;
		if(idx==MESSAGES_NUM) idx=0;
	}
	else
	{
		LIN_Msg[idx].TicksToExec--;
	}


}


static void LIN_Break_detected(void)
{
	u8 rec_break;
	LIN_masterStatus = MASTER_SENT_BREAK;
	USART_DMARecieve(LIN_USART,&rec_break , 1 , LIN_REC_HEADER);
}

static void LIN_REC_HEADER(void)
{
	USART_DMARecieve(LIN_USART,Slave_Header , NUM_OF_HEADER_BYTES , Header_received);
	LIN_slaveStatus = SLAVE_WAITING_HEADER;
}

static void Header_received(void)
{
	LIN_slaveStatus=SLAVE_HEADER_RECEIVED;
}

static void Response_received(void)
{
	LIN_slaveStatus=SLAVE_CHECKING_CHECKSUM;
}

static void Send_Checksum(void)
{
	USART_DMASend(LIN_USART,&checksum , 1 , NULL);
}


static void LIN_SlaveTask(void)
{
	u8 parity=0;
	static u8 Local_buffer[9]={};
	static volatile u8 dataLength=0;
	u16 calculated_checksum=0;
	switch (LIN_slaveStatus)
	{
	case SLAVE_IDLE:
		/*Do Nothing*/
		break;
	case SLAVE_WAITING_HEADER:
		/*Do Nothing*/
		break;
	case SLAVE_HEADER_RECEIVED:
		if(Slave_Header[SYNC]==SYNC_BYTE)
		{
			parity |= (((Slave_Header[PID] >>BIT_0) ^ (Slave_Header[PID] >>BIT_1) ^
					(Slave_Header[PID] >>BIT_2) ^ (Slave_Header[PID] >>BIT_4))<<P0)&P1_CLEAR_MASK;

			parity  |= ~ (  (Slave_Header[PID] >>BIT_1) ^ (Slave_Header[PID] >>BIT_3)  ^
					(Slave_Header[PID] >>BIT_4) ^ (Slave_Header[PID] >>BIT_5) )<<P1;
			/*Parity checking with the received ID*/
			if((parity&PARITY_MASK) == (Slave_Header[1]&PARITY_MASK))
			{
				/*Removing the parity bits to get the real ID*/
				Slave_Header[PID]&= PARITY_CLEAR_MASK;
				if(PID_map[Slave_Header[PID]]==INVALID_ID)
				{
					//ignore
					LIN_slaveStatus = SLAVE_IDLE;
				}
				else
				{
					asm("NOP");
					dataLength = LIN_cfg_Arr[PID_map[Slave_Header[PID]]].dataLength;
					/*Checking if the slave should send or receive the response*/
					if(LIN_cfg_Arr[PID_map[Slave_Header[PID]]].response == SEND)
					{
						LIN_slaveStatus = SLAVE_SENDING_RESPONSE;
					}
					else if(LIN_cfg_Arr[PID_map[Slave_Header[PID]]].response == REC)
					{
						LIN_slaveStatus = SLAVE_RECEIVING_RESPONSE;
					}
				}
			}
			else
			{
				/*Parity error*/
				LIN_slaveStatus = SLAVE_IDLE;
			}
		}
		else
		{
			/*Sync byte error*/
			LIN_slaveStatus = SLAVE_IDLE;
		}
		break;
	case SLAVE_RECEIVING_RESPONSE:
		/*Requesting to receive a specified number of data bytes according to configuration , and saving it in the suitable place in the LIN_databuffer array*/
		USART_u8RecDataAsync(LIN_USART,Local_buffer , dataLength+1 , Response_received , NULL);
		/*Go to the waiting state until the response is received*/
		LIN_slaveStatus = SLAVE_WAITING_RESPONSE;
		break;
	case SLAVE_WAITING_RESPONSE:
		/*Do Nothing*/
		break;
	case SLAVE_CHECKING_CHECKSUM :
		/*Assigning the checksum to the last received byte*/
		checksum = Local_buffer[dataLength];
		/*Calculating Checksum*/
		for(u8 i=0;i<dataLength;i++)
		{
			calculated_checksum += Local_buffer[i];
			if(calculated_checksum>255)calculated_checksum-=255;
		}
		calculated_checksum&=0XFF;
		/*Checking if the calculated checksum is valid*/
		if((u8)calculated_checksum + checksum == 0xff)
		{
			/*if the calculated checksum is valid assign the local buffer to the LIN_databuffer*/
			for(u8 i=0;i<dataLength;i++)
				LIN_databuffer[PID_map[Slave_Header[PID]]][i]=Local_buffer[i];

			/*Go Back to the idle state*/
			LIN_slaveStatus = SLAVE_IDLE;
		}
		else
		{
			/*Checksum error*/
			LIN_slaveStatus = SLAVE_IDLE;
		}
		break;
	case SLAVE_SENDING_RESPONSE:
		/*Calculating checksum*/
		for(u8 i=0;i<dataLength;i++)
		{
			calculated_checksum += LIN_databuffer[PID_map[Slave_Header[PID]]][i];
			if(calculated_checksum>255)calculated_checksum-=255;
		}
		/*Getting the inverse of the calculated_checksum*/
		calculated_checksum = ~calculated_checksum;
		checksum = (u8)calculated_checksum;
		/*Read back with call back to check the validity of the checksum*/
		USART_DMARecieve(LIN_USART,Local_buffer , dataLength+1 , Response_received);
		/*Sending the Data bytes from LIN_databuffer with callback to send the checksum byte*/
		USART_DMASend(LIN_USART,LIN_databuffer[PID_map[Slave_Header[PID]]] , dataLength , Send_Checksum);

		/*Go to the waiting state until the response is received*/
		if(LIN_slaveStatus != SLAVE_CHECKING_CHECKSUM)
			LIN_slaveStatus = SLAVE_WAITING_RESPONSE;
		break;

	}
}

u8 getSDU(u8 ID,u8*data)
{
	u8 local_status =OK;
	if(ID<INVALID_ID && data != NULL)
	{
		for(u8 i=0;i<LIN_cfg_Arr[PID_map[ID]].dataLength;i++)
		{
			data[i] = LIN_databuffer[PID_map[ID]][i];
		}
	}
	else
	{
		local_status=NOT_OK;
	}
	return local_status;
}


u8 setSDU(u8 ID,u8*data)
{
	u8 local_status =OK;
	if(ID<INVALID_ID && data != NULL)
	{
		for(u8 i=0;i<LIN_cfg_Arr[PID_map[ID]].dataLength;i++)
		{
			LIN_databuffer[PID_map[ID]][i] = data[i] ;
		}
	}
	else
	{
		local_status=NOT_OK;
	}
	return local_status;
}

