/********************************************************/
/*Author : Mohamed Megahed ,Islam El-Bahnasawy          */
/*Date   : 25 March 2020								*/
/*Version : V01											*/
/********************************************************/
#include "../../LIB/STD_TYPES.h"
#include "USART_MYTYPES.h"

#ifndef _USART_PRIVATE_H_
#define _USART_PRIVATE_H_

#define UE_CLEAR_MASK 							0xffffDfff
#define TE_CLEAR_MASK 							0xfffffff7
#define RE_CLEAR_MASK							0xfffffffB

#define USART_SR_TXE							0x00000080
#define USART_SR_RXNE							0x00000020
#define USART_CR1_PCE_ENABLED					0x00000400

#define USART_CR1_M_CLEAR_MASK					0xffffEfff
#define USART_CR1_PARITY_CLEAR_MASK				0xfffff9ff

#define USART_CR1_TXEIE_CLEAR_MASK				0xffffff7f
#define USART_CR1_RXNEIE_CLEAR_MASK				0xffffffDf

#define USART_CR2_STOP_CLEAR_MASK				0xffffcfff

#define ENABLED									34U
#define DISABLED								44U
#define ODD_PARITY								0x00000200
#define EVEN_PARITY								12U
#define ONE_STOP_BIT							0x00000000
#define HALF_STOP_BIT							0x00001000
#define TWO_STOP_BIT							0x00002000
#define ONEANDHALF_STOP_BIT						0x00003000

#define TX_INT_ENABLED							0x00000080
#define TX_INT_DISABLED							0x00000000

#define RX_INT_ENABLED							0x00000020
#define RX_INT_DISABLED							0x00000000

#define IDLE									92U
#define BUSY									13U

#define NULL									((void*)0)

/*default configurations */
#ifndef USART_BAUDRATE
#define USART_BAUDRATE 9600
#endif

#ifndef USART_STATUS
#define USART_STATUS 		ENABLED
#endif

#ifndef USART_TRANS_STATUS
#define USART_TRANS_STATUS	ENABLED
#endif

#ifndef USART_REC_STATUS
#define USART_REC_STATUS	ENABLED
#endif

#ifndef USART_PARITY_STATUS
#define USART_PARITY_STATUS	DISABLED
#endif 

#ifndef USART_PARITY_MODE
#define USART_PARITY_MODE	EVEN_PARITY
#endif

#ifndef USART_STOP_BIT_NUM
#define USART_STOP_BIT_NUM	ONE_STOP_BIT
#endif


typedef struct
{
	u8 * data;
	u32	length;
	u32 position;
	u8 state;
	Handler_t handler;
	M_handler_t	M_handler;
}dataBuffer_t;



/*Enter the USART peripheral Options :
 * 							 			USART1,
 * 							 			USART2,
 * 							 			USART3,
 * 							 			USART4,
 * 							 			USART5
 */
static u8 IsDataTransfered(USART_t* USART);
/*Enter the USART peripheral Options :
 * 							 			USART1,
 * 							 			USART2,
 * 							 			USART3,
 * 							 			USART4,
 * 							 			USART5
 */
static u8 IsDataRecieved(USART_t* USART);
/*Enter the USART peripheral Options :
 * 							 			USART1,
 * 							 			USART2,
 * 							 			USART3,
 * 							 			USART4,
 * 							 			USART5
 */
/*the status options : TX_INT_ENABLED	, TX_INT_DISABLED	*/
static void USART_voidTransIntStatus(USART_t* USART,u32 status);
/*Enter the USART peripheral Options :
 * 							 			USART1,
 * 							 			USART2,
 * 							 			USART3,
 * 							 			USART4,
 * 							 			USART5
 */
/*the status options : RX_INT_ENABLED	, RX_INT_DISABLED	*/
static void USART_voidRecIntStatus(USART_t* USART,u32 status);

#endif
