/********************************************************/
/*Author : Mohamed Megahed ,Islam El-Bahnasawy          */
/*Date   : 25 March 2020								*/
/*Version : V01											*/
/********************************************************/

#ifndef _URTM_INTERFACE_H_
#define _URTM_INTERFACE_H_

/*Used as input parameters to define which USART is being used*/
#define USART1 		((USART_t*)(0x40013800))
#define USART2
#define USART3
#define USART4
#define USART5

/******************************************************************************************
* function name: USRTH_voidInit
*
* parameters:  Input:
*                 USART
*                     type: USART_t*
*                     Description: this variable carries the address of the USART peripheral
*					  Options:
* 							 	USART1,
* 							 	USART2,
* 							 	USART3,
* 							 	USART4,
* 							 	USART5
*              Output: NA
*              In/out: NA
*
* return: void
* Description: This function is used to initialize the USART peripheral by initializing the
* 				RCC . NVIC . GPIO , USART bits by the values needed for the USART to function
********************************************************************************************/
void USRTH_voidInit(USART_t* USART);

/******************************************************************************************
* function name: USRTH_u8SendingReq
*
* parameters:  Input:
*                 USART
*                     type: USART_t*
*                     Description: this variable carries the address of the USART peripheral
*					  Options:
* 							 	USART1,
* 							 	USART2,
* 							 	USART3,
* 							 	USART4,
* 							 	USART5
*                Copy_Data
*                     type: u8*
*                     Description: this variable carries the address of the data array
*								   that will be sent
*				Copy_length
*                     type: u32
*                     Description: this variable carries the length of the data array
*								   that will be sent
*				handler
*                     type: Handler
*                     Description: this variable carries the address of the user's callback function
*              Output: NA
*              In/out: NA
*
* return: OK , NOT_OK
* Description:  This function is used to send number of bytes (Copy_Length) from an address (Copy_Data)
* 				on the USART and when it is done sending a function is called (handler).
* 				if this function is called and the USART is busy sending , the request will be
* 				put in a queue and will be performed after the current request is performed
********************************************************************************************/
u8 USRTH_u8SendingReq(USART_t* USART,u8* Copy_Data , u32 Copy_Length , Handler_t handler);

/******************************************************************************************
* function name: USART_u8RecDataAsync
*
* parameters:  Input:
*                 USART
*                     type: USART_t*
*                     Description: this variable carries the address of the USART peripheral
*					  Options:
* 							 	USART1,
* 							 	USART2,
* 							 	USART3,
* 							 	USART4,
* 							 	USART5
*				Copy_length
*                     type: u32
*                     Description: this variable carries the number of bytes that will be received
*				handler
*                     type: Handler
*                     Description: this variable carries the address of the user's callback function
*              Output:
*                Copy_Data
*                     type: u8*
*                     Description: this variable carries the address of the data array in which
*								   data will be received
*              In/out: NA
*
* return: OK , NOT_OK
* Description:  This function is used to receive  number of bytes (Copy_Length) into an address (Copy_Data)
* 				on the USART and when it is done receiving a function is called (handler).
* 				if this function is called and the USART is busy receiving , the request will be
* 				put in a queue and will be performed after the current request is performed
********************************************************************************************/
u8 USRTH_u8ReceivingReq(USART_t* USART,u8* Copy_Data , u32 Copy_Length , Handler_t handler);

#endif
