/********************************************************/
/*Author : Mohamed Megahed                              */
/*Date   : 13 March 2020								*/
/*Version : V01											*/
/********************************************************/

#ifndef _USART_INTERFACE_H_
#define _USART_INTERFACE_H_

#define UE_ENABLED			0x00002000
#define UE_DISABLED			0x00000000

#define TE_ENABLED			0x00000008
#define TE_DISABLED			0x00000000

#define RE_ENABLED			0x00000004
#define RE_DISABLED			0x00000000

typedef void (*Handler)(void);
typedef struct
{
	u8 * data;
	u32	length;
	u32 position;
	u8 state;
	Handler handler;
	Handler M_handler;
}dataBuffer_t;

/******************************************************************************************
* function name: USART_voidInit
*
* parameters:  Input:
*                fclk
*                     type: u32
*                     Description: this variable carries the current APB2 bus clock freq in HZ
*              Output: NA
*              In/out: NA
*
* return: void
* Description: This function is used to initialize the USART peripheral according to user 
*			   configurations
********************************************************************************************/
void USART_voidInit(u32 fclk);

/******************************************************************************************
* function name: USART_voidChangeStatus
*
* parameters:  Input:
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
void USART_voidChangeStatus(u32 status);

/******************************************************************************************
* function name: USART_voidChangeTransStatus
*
* parameters:  Input:
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
void USART_voidChangeTransStatus(u32 status);

/******************************************************************************************
* function name: USART_voidChangeRecStatus
*
* parameters:  Input:
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
void USART_voidChangeRecStatus(u32 status);


/******************************************************************************************
* function name: USART_u8SendingDataASync
*
* parameters:  Input:
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
*                     Description: this variable carries the address of the callback function
*              Output: NA
*              In/out: NA
*
* return: OK , NOT_OK
* Description: This function sends data via USART with a specific length (Copy_length)
*			   from a specific address (Copy_Data) , and executes a function sent by user 
*			   when it is done sending (handler)
********************************************************************************************/

u8 USART_u8SendingDataASync(u8* Copy_Data , u32 Copy_Length , Handler user_handler , Handler manger_handler);

/******************************************************************************************
* function name: USART_u8RecDataAsync
*
* parameters:  Input:
*				Copy_length
*                     type: u32
*                     Description: this variable carries the number of bytes that will be received
*				handler
*                     type: Handler 
*                     Description: this variable carries the address of the callback function
*              Output: 
*                Copy_Data
*                     type: u8*
*                     Description: this variable carries the address of the data array in which
*								   data will be recieved
*              In/out: NA
*
* return: OK , NOT_OK
* Description: This function recieves data via USART with a specific length (Copy_length)
*			   into a specific address (Copy_Data) , and executes a function sent by user 
*			   when it is done recieving (handler)
********************************************************************************************/

u8 USART_u8RecDataAsync(u8* Copy_Data,u32 Copy_length,Handler handler , Handler manger_handler);
#endif
