/********************************************************/
/*Author : Mohamed Megahed ,Islam El-Bahnasawy          */
/*Date   : 25 March 2020								*/
/*Version : V01											*/
/********************************************************/

#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/USART/USART_MYTYPES.h"


#include "../../MCAL/USART/USART_interface.h"
#include "../../MCAL/RCC/RCC_interface.h"
#include "../../MCAL/DIO/DIO_interface.h"
#include "../../MCAL/NVIC/NVIC_interface.h"

#include "USRTH_interface.h"
#include "USRTH_private.h"



Req_t TxReqs[USRTH_QUEUE_LENGTH];
Req_t RxReqs[USRTH_QUEUE_LENGTH];


Queue_t SendQueue;
Queue_t RecQueue;
static void Queue_voidInit(void)
{
	/*initializing the SendQueue*/
	SendQueue.Length = USRTH_QUEUE_LENGTH;
	SendQueue.Available = USRTH_QUEUE_LENGTH;
	SendQueue.Head = TxReqs;
	SendQueue.Tail = TxReqs;
	SendQueue.Base = TxReqs;

	/*initializing the RecQueue*/
	RecQueue.Length = USRTH_QUEUE_LENGTH;
	RecQueue.Available = USRTH_QUEUE_LENGTH;
	RecQueue.Head = RxReqs;
	RecQueue.Tail = RxReqs;
	RecQueue.Base = RxReqs;
}

static u8 Queue_u8IsFull(Queue_t* queue)
{
	return (queue->Available==0);
}


static u8 Queue_u8IsEmpty(Queue_t* queue)
{
	return (queue->Available == queue->Length);
}

static u8 URTM_QueuePush(Queue_t* queue,Req_t* Copy_JobData)
{
	u8 Local_Status = OK;
	if(!Queue_u8IsFull(queue))
	{

		*queue->Head = *Copy_JobData;
		queue->Head ++;
		if(queue->Head == (queue->Base + queue->Length))
		{
			queue->Head = queue->Base;
		}
		queue->Available--;
	}
	else
	{
		Local_Status = NOT_OK;
	}
	return Local_Status;
}

static u8 URTM_QueuePop(Queue_t* queue,Req_t* Copy_JobData)
{
	u8 Local_Status = OK;
	if(!Queue_u8IsEmpty(queue))
	{
		*Copy_JobData = *queue->Tail;
		queue->Tail++;
		if(queue->Tail == (queue->Base + queue->Length))
		{
			queue->Tail = queue->Base;
		}
		queue->Available++;
	}
	else
	{
		Local_Status = NOT_OK;
	}
	return Local_Status;
}

void USRTH_voidInit(USART_t* USART)
{
	/*Enabling the USART clock through RCC*/
	RCC_Peripheral_Config(RCC_USART1,PERIPHERAL_ON);
	/*Enabling the GPIOA clock through RCC*/
	RCC_Peripheral_Config(RCC_IOPA,PERIPHERAL_ON);
	/*Configuring the Tx and Rx GPIO pins*/
	GPIO_t gpio = {GPIO_PIN_9&GPIO_PIN_10,PIN9_MODE_AF_PUSHPULL_SPEED_10MHZ|PIN10_MODE_IP_FLOATING,GPIO_PORTA};
	GPIO_Config(&gpio);
	/*Enabling the USART1 interrupt in NVIC*/
	NVIC_EnableIRQ(NVIC_USART1);
	/*Getting the current APB2 frequency in HZ*/
	u32 freq = RCC_u32ReadAPB2Freq();
	USART_voidInit(USART,freq);
	Queue_voidInit();
}


u8 USRTH_u8SendingReq(USART_t* USART,u8* Copy_Data , u32 Copy_Length , Handler_t handler)
{
	u8 state;
	Req_t tempdataBuffer;
	state = USART_u8SendingDataASync(USART,Copy_Data,Copy_Length,handler,Tx_Handler);
	/*Checks if USART is busy sending another array of bytes*/
	if(state == NOT_OK)
	{
		/*save the sent data in a structure of type Req_t*/
		tempdataBuffer.DataBuffer = Copy_Data;
		tempdataBuffer.Length = Copy_Length;
		tempdataBuffer.Callback = handler;
		/*if the SendQueue is not full push the structure in it*/
		if(!Queue_u8IsFull(&SendQueue))
		{
			URTM_QueuePush(&SendQueue,&tempdataBuffer);
			state = OK;
		}
		else
		{
			state = NOT_OK;
		}
	}
	return state;
}


u8 USRTH_u8ReceivingReq(USART_t* USART,u8* Copy_Data , u32 Copy_Length , Handler_t handler)
{
	u8 state;
	Req_t tempdataBuffer;
	state = USART_u8RecDataAsync(USART,Copy_Data,Copy_Length,handler,Rx_Handler);
	/*Checks if USART is busy receiving another array of bytes*/
	if(state == NOT_OK)
	{
		/*save the sent data in a structure of type Req_t*/
		tempdataBuffer.DataBuffer = Copy_Data;
		tempdataBuffer.Length = Copy_Length;
		tempdataBuffer.Callback = handler;
		/*if the RecQueue is not full push the structure in it*/
		if(!Queue_u8IsFull(&RecQueue))
		{
			URTM_QueuePush(&RecQueue,&tempdataBuffer);
			state = OK;
		}
		else
		{
			state = NOT_OK;
		}
	}
	return state;
}



static void Tx_Handler(USART_t* USART)
{
	Req_t tempdataBuffer;
	if(!Queue_u8IsEmpty(&SendQueue))
	{
		URTM_QueuePop(&SendQueue,&tempdataBuffer);
		USRTH_u8SendingReq(USART,tempdataBuffer.DataBuffer,tempdataBuffer.Length,tempdataBuffer.Callback);
	}
}

static void Rx_Handler(USART_t* USART)
{
	Req_t tempdataBuffer;
	if(!Queue_u8IsEmpty(&RecQueue))
	{
		URTM_QueuePop(&RecQueue,&tempdataBuffer);
		USRTH_u8ReceivingReq(USART,tempdataBuffer.DataBuffer,tempdataBuffer.Length,tempdataBuffer.Callback);
	}
}

