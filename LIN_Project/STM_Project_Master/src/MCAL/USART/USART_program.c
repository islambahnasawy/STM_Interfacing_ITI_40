/********************************************************/
/*Author : Mohamed Megahed ,Islam El-Bahnasawy          */
/*Date   : 25 March 2020								*/
/*Version : V01											*/
/********************************************************/
#include "../../LIB/STD_TYPES.h"
#include "USART_MYTYPES.h"

#include "USART_interface.h"
#include "USART_reg.h"
#include "USART_config.h"
#include "USART_private.h"
#include "../DMA/DMA_interface.h"

#include "../../RTOS/RTOS_interface.h"
#include "../../HAL/LCD/C_LCD_interface.h"

#define DMA 	5
#define UART 	6

u8 USART_DMAState=IDLE;

static dataBuffer_t txbuffer ;
static dataBuffer_t rxbuffer;
static Handler_t DMA_CB;
static Handler_t LIN_break_CB;

static Sending_Mode;

void USART_voidInit(USART_t* USART,u32 fclk)
{
	u32 temp ;
#if USART_STATUS == ENABLED
	USART_voidChangeStatus(USART,UE_ENABLED);
#else
	USART_voidChangeStatus(USART,UE_DISABLED);
#endif

#if	USART_TRANS_STATUS == ENABLED
	USART_voidChangeTransStatus(USART,TE_ENABLED);
#else
	USART_voidChangeTransStatus(USART,TE_DISABLED);
#endif

#if USART_REC_STATUS == ENABLED
	USART_voidChangeRecStatus(USART,RE_ENABLED);
#else
	USART_voidChangeRecStatus(USART,RE_DISABLED);
#endif
	/*word length = 8 bits*/
	USART->CR1 &= USART_CR1_M_CLEAR_MASK;
	temp = USART->CR1;
	/*setting parrity bit*/
	temp &= USART_CR1_PARITY_CLEAR_MASK;
#if USART_PARITY_STATUS	== ENABLED
	temp |= USART_CR1_PCE_ENABLED;
#endif
#if USART_PARITY_MODE == ODD_PARITY
	temp |= ODD_PARITY;
#endif
	USART->CR1 = temp;
	/*setting the number of stop bits*/
	temp = USART->CR2;
	temp &= USART_CR2_STOP_CLEAR_MASK;
	temp |= USART_STOP_BIT_NUM;
	USART->CR2 = temp;
	/*setting baudrate */
	USART->BRR = fclk / USART_BAUDRATE;
	/*Initializing the txbuffer state as IDLE*/
	txbuffer.state = IDLE ;
	/*Initializing the rxbuffer state as IDLE*/
	rxbuffer.state = IDLE ;
}

void USART_LINInit(USART_t* USART,u32 fclk)
{
	u32 temp ;
	/*Enabling UART*/
	USART_voidChangeStatus(USART,UE_ENABLED);
	/*Enabling UART transmission*/
	USART_voidChangeTransStatus(USART,TE_ENABLED);
	/*Enabling UART Receiption*/
	USART_voidChangeRecStatus(USART,RE_ENABLED);
	/*word length = 8 bits*/
	USART->CR1 &= USART_CR1_M_CLEAR_MASK;
	/*disabling parity bit*/
	USART->CR1 &= USART_CR1_PARITY_CLEAR_MASK;
	temp = USART->CR2;
	/*Clearing the stop bits*/
	temp &= USART_CR2_STOP_CLEAR_MASK;
	/*Clearing CLKEN bit*/
	temp &= USART_CR2_CLKEN_CLEAR_MASK;
	/*Clearing the LBDIE , LBDL bits*/
	temp &= USART_CR2_LBDIE_LBDL_CLEAR_MASK;
	/*Setting the LBDIE , LBDL according to user configuration*/
	temp |= LIN_BREAK_INTERRUPT;
	/*Setting the LIN enable bit*/
	//temp |= USART_CR2_LIN_ENABLE_MASK;
	USART->CR2 = temp;
	/*Clearing IREN , HDSEL , SCEN bits*/
	USART->CR3 &= USART_CR3_LIN_INIT_CLEAR_MASK;
	/*setting baudrate */
	USART->BRR = fclk / LIN_BAUDRATE;
	/*Initializing the txbuffer state as IDLE*/
	txbuffer.state = IDLE ;
	/*Initializing the rxbuffer state as IDLE*/
	rxbuffer.state = IDLE ;

}

void USART_LINsendBreak(USART_t* USART)
{
	USART->CR1 |= USART_CR1_LIN_SEND_BREAK_MASK;
}


void USART_DMAEnable(USART_t* USART)
{
	/*Enabling Transmission through DMA*/
	USART->CR3 |= DMAR_MASK;
	/*Enabling Reception through DMA*/
	USART->CR3 |= DMAT_MASK;
	/*Clearing the transmission compelete flag*/
	USART->SR &= (~TC_MASK);
	/*Enabling the Transmit complete interrupt*/
	USART_voidTCIntStatus(USART,TC_INT_ENABLE);
}

void USART_DMADisable(USART_t* USART)
{
	/*Disabling Transmission through DMA*/
	USART->CR3 &= ~DMAR_MASK;
	/*Disabling Reception through DMA*/
	USART->CR3 &= ~DMAT_MASK;
	/*Disabling the Transmit complete interrupt*/
	USART_voidTCIntStatus(USART,TC_INT_DISABLE);
}

u8 USART_u8SendingDataASync(USART_t* USART , u8* Copy_Data , u32 Copy_Length , Handler_t user_handler , M_handler_t manger_handler)
{
	u8 Local_Status = OK ;
	Sending_Mode=UART;
	/*Check if there is no transmission in progress by either the Asynchronous USART function or via DMA*/
	if(txbuffer.state == IDLE && USART_DMAState==IDLE)
	{
		txbuffer.state = BUSY;
		txbuffer.length = Copy_Length;
		txbuffer.data = Copy_Data;
		txbuffer.handler = user_handler;
		txbuffer.M_handler = manger_handler;
		txbuffer.position = 0;
		/*Sending the first data byte that will start the chain of transmission of the rest of the bytes*/
		//trace_printf("data transimtted\n");
		USART->DR = txbuffer.data[txbuffer.position];
		txbuffer.position++;
		USART_voidTransIntStatus(USART,TX_INT_ENABLED);
	}
	else
	{
		Local_Status = NOT_OK;
	}
	return Local_Status;
}

u8 USART_u8RecDataAsync(USART_t* USART ,u8* Copy_Data,u32 Copy_length,Handler_t handler , M_handler_t manger_handler)
{
	u8 Local_Status = OK;
	/*Check if there is no reception in progress by either the Asynchronous USART function or via DMA*/
	if(rxbuffer.state == IDLE && Channel_States[CH5]==IDLE)
	{
		rxbuffer.state = BUSY;
		rxbuffer.data = Copy_Data;
		rxbuffer.position = 0;
		rxbuffer.length = Copy_length;
		rxbuffer.handler = handler ;
		rxbuffer.M_handler = manger_handler;
		USART_voidRecIntStatus(USART,RX_INT_ENABLED);
	}
	else
	{
		Local_Status = NOT_OK;
	}
	return Local_Status;
}

static u8 IsTransmissionComplete(USART_t* USART)
{
	return ((USART->SR & TC_MASK) == TC_MASK);
}


static u8 IsDataTransfered(USART_t* USART)
{
	return ((USART->SR & USART_SR_TXE) == USART_SR_TXE);
}

static u8 IsDataRecieved(USART_t* USART)
{
	return ((USART->SR & USART_SR_RXNE) == USART_SR_RXNE);
}

static u8 IsBreakRecieved(USART_t* USART)
{
	return ((USART->SR & USART_SR_LBD) == USART_SR_LBD);
}

void USART_voidChangeStatus(USART_t* USART , u32 status)
{
	u32 temp = USART->CR1 & UE_CLEAR_MASK ;
	temp |= status;
	USART->CR1 = temp ;
}

void USART_voidChangeTransStatus(USART_t* USART , u32 status)
{
	u32 temp = USART->CR1 & TE_CLEAR_MASK ;
	temp |= status;
	USART->CR1 = temp ;
}

void USART_voidChangeRecStatus(USART_t* USART ,u32 status)
{
	u32 temp = USART->CR1 & RE_CLEAR_MASK ;
	temp |= status;
	USART->CR1 = temp ;
}

static void USART_voidTransIntStatus(USART_t* USART ,u32 status)
{
	u32 temp = USART->CR1 & USART_CR1_TXEIE_CLEAR_MASK ;
	temp |= status;
	USART->CR1 = temp ;
}

static void USART_voidRecIntStatus(USART_t* USART ,u32 status)
{
	u32 temp = USART->CR1 & USART_CR1_RXNEIE_CLEAR_MASK ;
	temp |= status;
	USART->CR1 = temp ;
}

static void USART_voidTCIntStatus(USART_t* USART ,u32 status)
{
	u32 temp = USART->CR1 & ~(TC_MASK) ;
	temp |= status;
	USART->CR1 = temp ;
}

void USART1_IRQHandler(void)
{

	if (IsBreakRecieved(USART1_ADDRESS))
	{
		LCD_OS_write("Break");
		/*Clearing the LBD flag */
		USART1_ADDRESS->SR &= ~USART_SR_LBD;
		/*Clearing the RXNE flag */
		USART1_ADDRESS->SR &= ~USART_SR_RXNE;
		/*Clearing the TC flag */
		USART1_ADDRESS->SR &= ~USART_SR_TC;
		if(LIN_break_CB != NULL)
		{
			trace_printf("Break\n");
			LIN_break_CB();
		}
	}

	/*In case of transmit complete interrupt*/
	else if(IsTransmissionComplete(USART1_ADDRESS)){
		/*Clearing the TC flag */
		asm("NOP");
		USART1_ADDRESS->SR &= ~USART_SR_TC;
		if(Sending_Mode==DMA)
		{
			USART_DMAState = IDLE;
			/*Clear the Transfer complete flag*/
			USART1_ADDRESS->SR &= (~TC_MASK);
			if(DMA_CB != NULL)
			{
				DMA_CB();
			}
		}
		else if(Sending_Mode==UART)
		{
			LCD_OS_write("Sent");
			trace_printf("data transmitted\n");
			if (txbuffer.position < txbuffer.length)
			{
				USART1_ADDRESS->DR = txbuffer.data[txbuffer.position];
				txbuffer.position++;
			}
			else
			{
				txbuffer.state = IDLE;
				USART_voidTransIntStatus(USART1_ADDRESS,TX_INT_DISABLED);
				/*Perform the User's handler*/
				if(txbuffer.handler!= NULL)
				{
					txbuffer.handler();
				}
				/*Perform the Manager's handler*/
				if(txbuffer.M_handler != NULL)
				{
					txbuffer.M_handler(USART1_ADDRESS);
				}
			}
		}
	}

	/*In case of recieve interrupt*/
	else if(IsDataRecieved(USART1_ADDRESS))
	{
		LCD_OS_write("rec");
		rxbuffer.data[rxbuffer.position] = USART1_ADDRESS->DR;
		rxbuffer.position++;
		trace_printf("data recieved\n");
		if(rxbuffer.position == rxbuffer.length)
		{
			rxbuffer.state = IDLE ;
			USART_voidRecIntStatus(USART1_ADDRESS,RX_INT_DISABLED);
			if(rxbuffer.handler != NULL)
			{
				rxbuffer.handler();
			}
			if(rxbuffer.M_handler != NULL)
			{
				rxbuffer.M_handler(USART1_ADDRESS);
			}
		}
	}


	/*In case of register empty interrupt*/
	else if(IsDataTransfered(USART1_ADDRESS))
	{

	}



	else
	{
		/*return error in case any error interrupt is enabled*/
	}
}

u8 set_TransmissionCompleteCB(Handler_t handler)
{
	u8 local_status=OK;
	DMA_CB = handler ;

	return local_status;
}


u8 set_LIN_BreakCB(Handler_t handler)
{
	u8 local_status=OK;
	if(handler)
	{
		LIN_break_CB = handler ;
	}
	else
	{
		local_status=NOT_OK;
	}
	return local_status;
}




u8 USART_DMASend(USART_t* USART,u8* Copy_Data , u32 Copy_Length , Handler_t handler)
{
	u8 local_status = OK;
	if(USART_DMAState == IDLE)
	{
		Sending_Mode=DMA;
		USART_DMAState = BUSY;
		DMA_data data ;
		data.Trans_Dir = MEM_TO_PREI;
		data.Peri_Size = PERIPHERAL_SIZE_8_BITS ;
		data.Mem_Size = MEMORY_SIZE_8_BITS;
		data.Circ_mode = CIRCULAR_MODE_DISABLED;
		data.Peri_Inc=PERIPHERAL_INC_MODE_DISABLED;
		data.Mem_Inc=MEMORY_INC_MODE_ENABLED;
		data.Num_Of_Trans=Copy_Length;
		data.Peri_Add=DMA_CHN4_PERI_USART1_TX;
		data.Mem_Add=(u32)Copy_Data;

		set_TransmissionCompleteCB(handler);
		DMA_voidStart(CH4,&data);
	}
	else
	{
		local_status = NOT_OK;
	}
	return local_status;
}

u8 USART_DMARecieve(USART_t* USART,u8* Copy_Data , u32 Copy_Length , Handler_t handler)
{
	u8 local_status = OK;

	DMA_data data ;
	data.Trans_Dir = PERI_TO_MEM;
	data.Circ_mode = CIRCULAR_MODE_DISABLED;
	data.Peri_Inc=PERIPHERAL_INC_MODE_DISABLED;
	data.Mem_Inc=MEMORY_INC_MODE_ENABLED;
	data.Peri_Size = PERIPHERAL_SIZE_8_BITS ;
	data.Mem_Size = MEMORY_SIZE_8_BITS;
	data.Num_Of_Trans=Copy_Length;
	data.Peri_Add=DMA_CHN5_PERI_USART1_RX;
	data.Mem_Add=(u32)Copy_Data;

	set_ChannelX_TCHandler(CH5,handler);
	local_status = DMA_voidStart(CH5,&data);

	return local_status;
}
