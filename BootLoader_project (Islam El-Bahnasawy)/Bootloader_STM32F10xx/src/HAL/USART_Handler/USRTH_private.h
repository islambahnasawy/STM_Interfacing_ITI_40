/********************************************************/
/*Author : Mohamed Megahed ,Islam El-Bahnasawy          */
/*Date   : 25 March 2020								*/
/*Version : V01											*/
/********************************************************/

#ifndef _URTM_PRIVATE_H_
#define _URTM_PRIVATE_H_
/*The call back function that will be performed after transmitting the needed number of bytes*/
static void Tx_Handler(USART_t* USART);
/*The call back function that will be performed after receiving  the needed number of bytes */
static void Rx_Handler(USART_t* USART);

/*The max number of requests that can be put in the receive or transmit queue*/
#define USRTH_QUEUE_LENGTH	10U

#define IDLE									0U
#define BUSY									13U

typedef struct
{
	u8 * DataBuffer;
	u8 Length;
	Handler_t Callback;
}Req_t;


typedef struct
{
	u8 Length;
	u8 Available ;
	Req_t * Head;
	Req_t * Tail;
	Req_t * Base;
}Queue_t;

static u8 Queue_u8IsFull(Queue_t* queue);
static u8 Queue_u8IsEmpty(Queue_t* queue);
static u8 URTM_QueuePush(Queue_t* queue,Req_t* Copy_JobData);
static u8 URTM_QueuePop(Queue_t* queue,Req_t* Copy_JobData);

#endif
