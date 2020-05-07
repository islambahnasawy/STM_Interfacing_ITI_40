/********************************************************/
/*Author : Mohamed Megahed ,Islam El-Bahnasawy          */
/*Date   : 25 March 2020								*/
/*Version : V01											*/
/********************************************************/

#ifndef _USART_INTERFACE_H_
#define _USART_INTERFACE_H_

#include "../DMA/DMA_interface.h"

/*UART Configuration Options*/
#define UE_ENABLED			0x00002000
#define UE_DISABLED			0x00000000

#define TE_ENABLED			0x00000008
#define TE_DISABLED			0x00000000

#define RE_ENABLED			0x00000004
#define RE_DISABLED			0x00000000

/*LIN Configuration options*/
#define LIN_BREAK_INTERRRUPT_ENABLED	0x00000040
#define LIN_BREAK_INTERRRUPT_DISABLED	0x00000000

#define LIN_BREAK_DETECTION_LENGTH_11	0x00000020
#define LIN_BREAK_DETECTION_LENGTH_10	0x00000000

/*Used as input parameters to define which USART is being used*/
#define USART1_ADDRESS 		((USART_t*)(0x40013800))
#define USART2_ADDRESS
#define USART3_ADDRESS
#define USART4_ADDRESS
#define USART5_ADDRESS


/******************************************************************************************
* function name: USART_voidInit
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
*                fclk
*                     type: u32
*                     Description: this variable carries the current APB2 bus clock frequency in HZ
*              Output: NA
*              In/out: NA
*
* return: void
* Description: This function is used to initialize the USART peripheral according to user
*			   configurations
********************************************************************************************/
void USART_voidInit(USART_t* USART,u32 fclk);

/******************************************************************************************
* function name: USART_voidChangeStatus
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
*                status
*                     type: u8
*                     Description: this variable carries the status of the USART
*					  Options:
*								UE_ENABLED,
*								UE_DISABLED
*              Output: NA
*              In/out: NA
*
* return: void
* Description: This function is used to change the status of the USART between enabled and 
*				disabled
********************************************************************************************/
void USART_voidChangeStatus(USART_t* USART,u32 status);

/******************************************************************************************
* function name: USART_voidChangeTransStatus
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
*                status
*                     type: u8
*                     Description: this variable carries the status of the USART transmission
*					  Options:
*								TE_ENABLED,
*								TE_DISABLED
*              Output: NA
*              In/out: NA
*
* return: void
* Description: This function is used to change the status of the USART transmission between 
*				enabled and disabled
********************************************************************************************/
void USART_voidChangeTransStatus(USART_t* USART,u32 status);

/******************************************************************************************
* function name: USART_voidChangeRecStatus
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
*                status
*                     type: u8
*                     Description: this variable carries the status of the USART reception
*					  Options:
*								RE_ENABLED,
*								RE_DISABLED
*              Output: NA
*              In/out: NA
*
* return: void
* Description: This function is used to change the status of the USART reception between 
*				enabled and disabled
********************************************************************************************/
void USART_voidChangeRecStatus(USART_t* USART,u32 status);


/******************************************************************************************
* function name: USART_u8SendingDataASync
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
*				manger_handler
*                     type: Handler
*                     Description: this variable carries the address of the manager's callback function
*              Output: NA
*              In/out: NA
*
* return: OK , NOT_OK
* Description: This function sends data via USART with a specific length (Copy_length)
*			   from a specific address (Copy_Data) , and executes a function sent by user 
*			   when it is done sending (handler)
********************************************************************************************/

u8 USART_u8SendingDataASync(USART_t* USART,u8* Copy_Data , u32 Copy_Length , Handler_t user_handler , M_handler_t manger_handler);

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
*				manger_handler
*                     type: Handler
*                     Description: this variable carries the address of the manager's callback function
*              Output: 
*                Copy_Data
*                     type: u8*
*                     Description: this variable carries the address of the data array in which
*								   data will be received
*              In/out: NA
*
* return: OK , NOT_OK
* Description: This function receives data via USART with a specific length (Copy_length)
*			   into a specific address (Copy_Data) , and executes a function sent by user 
*			   when it is done receiving (handler)
********************************************************************************************/

u8 USART_u8RecDataAsync(USART_t* USART,u8* Copy_Data,u32 Copy_length,Handler_t handler , M_handler_t manger_handler);

//extern u8 USART_DMAState;

void USART_DMAEnable(USART_t* USART);

void USART_DMADisable(USART_t* USART);

u8 set_TransmissionCompleteCB(Handler_t handler);


u8 USART_DMASend(USART_t* USART,u8* Copy_Data , u32 Copy_Length , Handler_t handler);

u8 USART_DMARecieve(USART_t* USART,u8* Copy_Data , u32 Copy_Length , Handler_t handler);


void USART_LINInit(USART_t* USART,u32 fclk);

void USART_LINsendBreak(USART_t* USART);

u8 set_LIN_BreakCB(Handler_t handler);

#endif
