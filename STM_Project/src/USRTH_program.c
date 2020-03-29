/********************************************************/
/*Author : Mohamed Megahed                              */
/*Date   : 13 March 2020								*/
/*Version : V01											*/
/********************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"


#include "USART_interface.h"
#include "RCC_interface.h"
#include "DIO_interface.h"
#include "NVIC_interface.h"

#include "USRTH_private.h"
#include "USRTH_interface.h"




Req_t TxReqs[USRTH_QUEUE_LENGTH];
Req_t RxReqs[USRTH_QUEUE_LENGTH];


Queue_t SendQueue;
Queue_t RecQueue;
static void Queue_voidInit(void)
{
	SendQueue.Length = USRTH_QUEUE_LENGTH;
	SendQueue.Available = USRTH_QUEUE_LENGTH;
	SendQueue.Head = TxReqs;
	SendQueue.Tail = TxReqs;
	SendQueue.Base = TxReqs;

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

void USRTH_voidInit(void)
{
	/*Enabling the USART clock through RCC*/
	RCC_Peripheral_Config(USART1,PERIPHERAL_ON);
	/*Enabling the GPIOA clock through RCC*/
	RCC_Peripheral_Config(IOPA,PERIPHERAL_ON);
	/*Configuring the Tx and Rx GPIO pins*/
	GPIO_t gpio = {GPIO_PIN_9&GPIO_PIN_10,PIN9_MODE_AF_PUSHPULL_SPEED_10MHZ|PIN10_MODE_IP_FLOATING,GPIO_PORTA};
	GPIO_Config(&gpio);
	/*Enabling the USART1 interrupt in NVIC*/
	NVIC_EnableIRQ(NVIC_USART1);
	u32 freq = RCC_u32ReadAPB2Freq();
	USART_voidInit(freq);
	Queue_voidInit();
}


u8 USRTH_u8SendingReq(u8* Copy_Data , u32 Copy_Length , Handler handler)
{
	u8 state;
	Req_t tempdataBuffer;
	state = USART_u8SendingDataASync(Copy_Data,Copy_Length,handler,Tx_Handler);
	if(state == NOT_OK)
	{
		tempdataBuffer.DataBuffer = Copy_Data;
		tempdataBuffer.Length = Copy_Length;
		tempdataBuffer.Callback = handler;
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


u8 USRTH_u8ReceivingReq(u8* Copy_Data , u32 Copy_Length , Handler handler)
{
	u8 state;
	Req_t tempdataBuffer;
	state = USART_u8RecDataAsync(Copy_Data,Copy_Length,handler,Rx_Handler);
	if(state == NOT_OK)
	{
		tempdataBuffer.DataBuffer = Copy_Data;
		tempdataBuffer.Length = Copy_Length;
		tempdataBuffer.Callback = handler;
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



static void Tx_Handler(void)
{
	Req_t tempdataBuffer;
	if(!Queue_u8IsEmpty(&SendQueue))
	{
		URTM_QueuePop(&SendQueue,&tempdataBuffer);
		USRTH_u8SendingReq(tempdataBuffer.DataBuffer,tempdataBuffer.Length,tempdataBuffer.Callback);
	}
}

static void Rx_Handler(void)
{
	Req_t tempdataBuffer;
	if(!Queue_u8IsEmpty(&RecQueue))
	{
		URTM_QueuePop(&RecQueue,&tempdataBuffer);
		USRTH_u8ReceivingReq(tempdataBuffer.DataBuffer,tempdataBuffer.Length,tempdataBuffer.Callback);
	}
}

