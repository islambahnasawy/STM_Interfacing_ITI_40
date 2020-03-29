/********************************************************/
/*Author : Mohamed Megahed                              */
/*Date   : 14 March 2020								*/
/*Version : V01											*/
/********************************************************/

#ifndef _URTM_PRIVATE_H_
#define _URTM_PRIVATE_H_


static void Tx_Handler(void);
static void Rx_Handler(void);

#define USRTH_QUEUE_LENGTH	10U

typedef struct
{
	u8 * DataBuffer;
	u8 Length;
	Handler Callback;
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
