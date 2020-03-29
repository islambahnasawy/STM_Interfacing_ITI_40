/********************************************************/
/*Author : Mohamed Megahed ,Islam El-Bahnasawy          */
/*Date   : 25 March 2020								*/
/*Version : V01											*/
/********************************************************/

#ifndef _USART_CONFIG_H_
#define _USART_CONFIG_H_

/*User configuration */
/*Put the needed baudrate*/
#define USART_BAUDRATE		9600


/*Options : ENABLED , DISABLED*/
#define USART_STATUS		ENABLED

/*Options : ENABLED , DISABLED*/
#define USART_TRANS_STATUS	ENABLED

/*Options : ENABLED , DISABLED*/
#define USART_REC_STATUS	ENABLED

/*Options : ENABLED , DISABLED*/
#define USART_PARITY_STATUS	DISABLED

/*Options : EVEN_PARITY , ODD_PARITY*/
#define USART_PARITY_MODE	EVEN_PARITY

/*Options : STOP_BIT_1 , STOP_BIT_0.5 , STOP_BIT_2 , STOP_BIT_1.5*/
#define USART_STOP_BIT_NUM	ONE_STOP_BIT

#endif
