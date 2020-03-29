/********************************************************/
/*Author : Mohamed Megahed                              */
/*Date   : 13 March 2020								*/
/*Version : V01											*/
/********************************************************/

#include "STD_TYPES.h"
#include "USART_interface.h"
#include "USART_reg.h"
#include "USART_config.h"
#include "USART_private.h"


dataBuffer_t txbuffer ;
dataBuffer_t rxbuffer;

void USART_voidInit(u32 fclk)
{
	u32 temp ;
#if USART_STATUS == ENABLED
	USART_voidChangeStatus(UE_ENABLED);
#else
	USART_voidChangeStatus(UE_DISABLED);
#endif

#if	USART_TRANS_STATUS == ENABLED
	USART_voidChangeTransStatus(TE_ENABLED);
#else
	USART_voidChangeTransStatus(TE_DISABLED);
#endif

#if USART_REC_STATUS == ENABLED
	USART_voidChangeRecStatus(RE_ENABLED);
#else
	USART_voidChangeRecStatus(RE_DISABLED);
#endif
	/*word length = 8 bits*/
	USART1->CR1 &= USART_CR1_M_CLEAR_MASK;
	temp = USART1->CR1;
	/*setting parrity bit*/
	temp &= USART_CR1_PARITY_CLEAR_MASK;
#if USART_PARITY_STATUS	== ENABLED
	temp |= USART_CR1_PCE_ENABLED;
#endif
#if USART_PARITY_MODE == ODD_PARITY
	temp |= ODD_PARITY;
#endif
	USART1->CR1 = temp;
	/*setting the number of stop bits*/
	temp = USART1->CR2;
	temp &= USART_CR2_STOP_CLEAR_MASK;
	temp |= USART_STOP_BIT_NUM;
	USART1->CR2 = temp;
	/*setting baudrate */
	USART1->BRR = fclk / USART_BAUDRATE;
	/*Initializing the txbuffer state as IDLE*/
	txbuffer.state = IDLE ;
	/*Initializing the rxbuffer state as IDLE*/
	rxbuffer.state = IDLE ;
	//USART1->GTPR = 1;
}

u8 USART_u8SendingDataASync(u8* Copy_Data , u32 Copy_Length , Handler user_handler , Handler manger_handler)
{
	u8 Local_Status = OK ;
	if(txbuffer.state == IDLE)
	{
		txbuffer.state = BUSY;
		txbuffer.length = Copy_Length;
		txbuffer.data = Copy_Data;
		txbuffer.handler = user_handler;
		txbuffer.M_handler = manger_handler;
		txbuffer.position = 0;
		/*Sending the first data byte that will start the chain of transmission of the rest of the bytes*/ 
		USART1->DR = txbuffer.data[txbuffer.position];
		txbuffer.position++;
		USART_voidTransIntStatus(TX_INT_ENABLED);
	}
	else
	{
		Local_Status = NOT_OK;
	}
	return Local_Status;
}

u8 USART_u8RecDataAsync(u8* Copy_Data,u32 Copy_length,Handler handler , Handler manger_handler)
{
	u8 Local_Status = OK;
	if(rxbuffer.state == IDLE)
	{
		rxbuffer.state = BUSY;
		rxbuffer.data = Copy_Data;
		rxbuffer.position = 0;
		rxbuffer.length = Copy_length;
		rxbuffer.handler = handler ;
		rxbuffer.M_handler = manger_handler;
		USART_voidRecIntStatus(RX_INT_ENABLED);
	}
	else
	{
		Local_Status = NOT_OK;
	}
	return Local_Status;
}

static u8 IsDataTransfered(void)
{
	return ((USART1->SR & USART_SR_TXE) == USART_SR_TXE);
}

static u8 IsDataRecieved(void)
{
	return ((USART1->SR & USART_SR_RXNE) == USART_SR_RXNE);
}

void USART_voidChangeStatus(u32 status)
{
	u32 temp = USART1->CR1 & UE_CLEAR_MASK ;
	temp |= status;
	USART1->CR1 = temp ;
}

void USART_voidChangeTransStatus(u32 status)
{
	u32 temp = USART1->CR1 & TE_CLEAR_MASK ;
	temp |= status;
	USART1->CR1 = temp ;
}

void USART_voidChangeRecStatus(u32 status)
{
	u32 temp = USART1->CR1 & RE_CLEAR_MASK ;
	temp |= status;
	USART1->CR1 = temp ;
}

static void USART_voidTransIntStatus(u32 status)
{
	u32 temp = USART1->CR1 & USART_CR1_TXEIE_CLEAR_MASK ;
	temp |= status;
	USART1->CR1 = temp ;
}

static void USART_voidRecIntStatus(u32 status)
{
	u32 temp = USART1->CR1 & USART_CR1_RXNEIE_CLEAR_MASK ;
	temp |= status;
	USART1->CR1 = temp ;
}


void USART1_IRQHandler(void)
{
	/*In case of recieve interrupt*/
	if(IsDataRecieved())
	{
		rxbuffer.data[rxbuffer.position] = USART1->DR;
		rxbuffer.position++;
		if(rxbuffer.position == rxbuffer.length)
		{
			rxbuffer.state = IDLE ;
			USART_voidRecIntStatus(RX_INT_DISABLED);
			if(rxbuffer.handler != NULL)
			{
				rxbuffer.handler();
			}
			if(rxbuffer.M_handler != NULL)
			{
				rxbuffer.M_handler();
			}
		}
	}
	/*In case of transmit interrupt*/
	else if(IsDataTransfered())
	{
		if (txbuffer.position < txbuffer.length)
		{
			USART1->DR = txbuffer.data[txbuffer.position];
			txbuffer.position++;
		}
		else
		{
			txbuffer.state = IDLE;
			USART_voidTransIntStatus(TX_INT_DISABLED);
			/*Perform the User's handler*/
			if(txbuffer.handler!= NULL)
			{
				txbuffer.handler();
			}
			/*Perform the Manager's handler*/
			if(txbuffer.M_handler != NULL)
			{
				txbuffer.M_handler();
			}
		}
	}

	else
	{
		/*return error in case any error interrupt is enabled*/
	}
}
