/********************************************************/
/*Author : Mohamed Megahed , Islam El-Bahnasawy 		*/
/*Date   : 1 APRIL 2020									*/
/*Version : V01											*/
/********************************************************/
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "DMA_interface.h"
#include "DMA_config.h"
#include "DMA_reg.h"
#include "DMA_private.h"
#include "../RCC/RCC_interface.h"
#include "../NVIC/NVIC_interface.h"

#include "../../Other_Drivers/LED/LED_interface.h"


u8 Channel_States[7];
u8 Channel_AlwaysBusy[7];

static CB_t CB_Handlers;


void DMA_voidInit(void)
{
	/*Enabling the DMA1 clock */
	RCC_Peripheral_Config(RCC_DMA1,PERIPHERAL_ON);

	/*Enabling the DMA interrupts through NVIC*/
	NVIC_EnableIRQ(NVIC_DMA1_CHANNEL1);
	NVIC_EnableIRQ(NVIC_DMA1_CHANNEL2);
	NVIC_EnableIRQ(NVIC_DMA1_CHANNEL3);
	NVIC_EnableIRQ(NVIC_DMA1_CHANNEL4);
	NVIC_EnableIRQ(NVIC_DMA1_CHANNEL5);
	NVIC_EnableIRQ(NVIC_DMA1_CHANNEL6);
	NVIC_EnableIRQ(NVIC_DMA1_CHANNEL7);
}

u8 DMA_u8ChannelInit(u8 Channel_Num)
{
	u8 local_status = OK;
	u32 temp = DMA->Channel[Channel_Num].CCR;
	/*Clearing the Init bits*/
	temp &= INIT_BITS_CLEAR_MASK;
	/*Enabling the Transfer complete interrupt*/
	temp |= DMA_TRANS_COMP_INT_ENABLE;
	switch (Channel_Num)
	{
	case CH1:
		temp |= DMA_CHANNEL1_PRIOIRITY | DMA_CHANNEL1_ERROR_INT |DMA_CHANNEL1_HALF_TRANS_COMP_INT ;
		break;
	case CH2:
		temp |= DMA_CHANNEL2_PRIOIRITY | DMA_CHANNEL2_ERROR_INT |DMA_CHANNEL2_HALF_TRANS_COMP_INT ;
		break;
	case CH3:
		temp |= DMA_CHANNEL3_PRIOIRITY | DMA_CHANNEL3_ERROR_INT |DMA_CHANNEL3_HALF_TRANS_COMP_INT ;
		break;
	case CH4:
		temp |= DMA_CHANNEL4_PRIOIRITY | DMA_CHANNEL4_ERROR_INT |DMA_CHANNEL4_HALF_TRANS_COMP_INT ;
		break;
	case CH5:
		temp |= DMA_CHANNEL5_PRIOIRITY | DMA_CHANNEL5_ERROR_INT |DMA_CHANNEL5_HALF_TRANS_COMP_INT ;
		break;
	case CH6:
		temp |= DMA_CHANNEL6_PRIOIRITY | DMA_CHANNEL6_ERROR_INT |DMA_CHANNEL6_HALF_TRANS_COMP_INT ;
		break;
	case CH7:
		temp |= DMA_CHANNEL7_PRIOIRITY | DMA_CHANNEL7_ERROR_INT |DMA_CHANNEL7_HALF_TRANS_COMP_INT ;
		break;
	default :
		local_status = NOT_OK;
	}
	DMA->Channel[Channel_Num].CCR = temp ;
	return local_status;
}

void DMA_voidSetPriority(u8 Channel_Num,u32 Priority)
{
	u32 temp = DMA->Channel[Channel_Num].CCR & PRIORITY_CLEAR_MASK;
	temp |= Priority;
	DMA->Channel[Channel_Num].CCR = temp;
}

void DMA_voidSetInterruptsStatus(u8 Channel_Num,u8 Err_Int , u8 Half_Trans_comp_INT)
{
	u32 temp = DMA->Channel[Channel_Num].CCR & INTERRUPTS_CLEAR_MASK;
	temp |= Err_Int | Half_Trans_comp_INT ;
	DMA->Channel[Channel_Num].CCR = temp;
}



u8 DMA_voidStart(u8 Channel_Num ,DMA_data* data)
{
	u8 local_status = OK;
	if(Channel_States[Channel_Num] == DMA_IDLE && Channel_AlwaysBusy[Channel_Num] == FALSE)
	{
		/*Indicating the current channel is busy*/
		Channel_States[Channel_Num] = DMA_BUSY;
		/*Clearing the MEM2MEM , MSIZE , PSIZE , MINC , PINC , CIRC , DIR , EN bits */
		u32 temp = DMA->Channel[Channel_Num].CCR & DMA_START_CLEAR_MASK;
		/*Assigning the values passed by user */
		temp |= data->Trans_Dir | data->Peri_Inc | data->Mem_Inc | data->Peri_Size | data->Mem_Size ;
		/*In case the we are not working in MEM2MEM mode*/
		if((temp & MEM_TO_MEM) != MEM_TO_MEM)
		{
			temp |= data->Circ_mode;
			if(data->Circ_mode == CIRCULAR_MODE_ENABLED)
			{
				/*This channel now is  always busy*/
				Channel_AlwaysBusy[Channel_Num] = TRUE;
			}
		}
		/*In case of MEM2MEM mode with Circular mode*/
		else if(data->Circ_mode == CIRCULAR_MODE_ENABLED)
		{
			local_status = NOT_OK;
		}
		/*Assiging the config register with the user's choices*/
		DMA->Channel[Channel_Num].CCR = temp;
		/*Assigning the number of transfers*/
		DMA->Channel[Channel_Num].CNDTR = data->Num_Of_Trans;
		/*Assigning the Peripheral address*/
		DMA->Channel[Channel_Num].CPAR = data->Peri_Add;
		/*Assigning the memory address*/
		DMA->Channel[Channel_Num].CMAR = data->Mem_Add;

		/* DMA Channel X Enable */
		DMA->Channel[Channel_Num].CCR |= 1;
	}
	else
	{
		local_status = NOT_OK;
	}
	return local_status;
}

static u8 get_TCFlagStatus(u8 Channel)
{
	return ((DMA->ISR >>(1+(4*Channel)))&1);
}

static u8 get_HTCFlagStatus(u8 Channel)
{
	return ((DMA->ISR >>(2+(4*Channel)))&1);
}

static u8 get_ERRFlagStatus(u8 Channel)
{
	return ((DMA->ISR >>(3+(4*Channel)))&1);
}

static void clear_TCFlag(u8 Channel)
{
	DMA->IFCR |= 1<<(1+(4*Channel));
}
static void clear_HTCFlag(u8 Channel)
{
	DMA->IFCR |= 1<<(2+(4*Channel));
}
static void clear_ERRFlag(u8 Channel)
{
	DMA->IFCR |= 1<<(3+(4*Channel));
}

u8 set_ChannelX_TCHandler(u8 Channel_Num, CHN_Handler handler)
{
	u8 local_status = NOT_OK;
	if(handler)
	{
		CB_Handlers.Channel_TCHandler[Channel_Num] = handler;
		local_status = OK;
	}
	return local_status;
}

u8 set_ChannelX_HTCHandler(u8 Channel_Num, CHN_Handler handler)
{
	u8 local_status = NOT_OK;
	if(handler)
	{
		CB_Handlers.Channel_HTCHandler[Channel_Num] = handler;
		local_status = OK;
	}
	return local_status;
}

u8 set_ChannelX_ERRHandler(u8 Channel_Num, CHN_Handler handler)
{
	u8 local_status = NOT_OK;
	if(handler)
	{
		CB_Handlers.Channel_ERRHandler[Channel_Num] = handler;
		local_status = OK;
	}
	return local_status;
}

void DMA_voidStop(u8 Channel)
{
	/*Turning the DMA channel to idle state*/
	Channel_States[Channel]=DMA_IDLE;
	Channel_AlwaysBusy[Channel]=FALSE;
	/* DMA Channel X Disable */
	DMA->Channel[Channel].CCR &= ~1;
}

void DMA1_Channel1_IRQHandler(void)
{
	/*In case of Transmission complete*/
	if(get_TCFlagStatus(CH1))
	{
		Channel_States[CH1] = DMA_IDLE;
		clear_TCFlag(CH1);
		if(CB_Handlers.Channel_TCHandler[CH1])
		{
			CB_Handlers.Channel_TCHandler[CH1]();
		}
	}
	/*In case of half Transmission complete*/
	else if(get_HTCFlagStatus(CH1))
	{
		clear_HTCFlag(CH1);
		if(CB_Handlers.Channel_HTCHandler[CH1])
		{
			CB_Handlers.Channel_HTCHandler[CH1]();
		}
	}
	/*In case of Transmission error*/
	else if(get_ERRFlagStatus(CH1))
	{
		clear_ERRFlag(CH1);
		if(CB_Handlers.Channel_ERRHandler[CH1])
		{
			CB_Handlers.Channel_ERRHandler[CH1]();
		}
	}
}

void DMA1_Channel2_IRQHandler(void)
{
	/*In case of Transmission complete*/
	if(get_TCFlagStatus(CH2))
	{
		Channel_States[CH2] = DMA_IDLE;
		clear_TCFlag(CH2);
		if(CB_Handlers.Channel_TCHandler[CH2])
		{
			CB_Handlers.Channel_TCHandler[CH2]();
		}
	}
	/*In case of half Transmission complete*/
	else if(get_HTCFlagStatus(CH2))
	{
		clear_HTCFlag(CH2);
		if(CB_Handlers.Channel_HTCHandler[CH2])
		{
			CB_Handlers.Channel_HTCHandler[CH2]();
		}
	}
	/*In case of Transmission error*/
	else if(get_ERRFlagStatus(CH2))
	{
		clear_ERRFlag(CH2);
		if(CB_Handlers.Channel_ERRHandler[CH2])
		{
			CB_Handlers.Channel_ERRHandler[CH2]();
		}
	}
}

void DMA1_Channel3_IRQHandler(void)
{
	/*In case of Transmission complete*/
	if(get_TCFlagStatus(CH3))
	{
		Channel_States[CH3] = DMA_IDLE;
		clear_TCFlag(CH3);
		if(CB_Handlers.Channel_TCHandler[CH3])
		{
			CB_Handlers.Channel_TCHandler[CH3]();
		}
	}
	/*In case of half Transmission complete*/
	else if(get_HTCFlagStatus(CH3))
	{
		clear_HTCFlag(CH3);
		if(CB_Handlers.Channel_HTCHandler[CH3])
		{
			CB_Handlers.Channel_HTCHandler[CH3]();
		}
	}
	/*In case of Transmission error*/
	else if(get_ERRFlagStatus(CH3))
	{
		clear_ERRFlag(CH3);
		if(CB_Handlers.Channel_ERRHandler[CH3])
		{
			CB_Handlers.Channel_ERRHandler[CH3]();
		}
	}
}

void DMA1_Channel4_IRQHandler(void)
{
	/*In case of Transmission complete*/
	/*USART transmit*/
	if(get_TCFlagStatus(CH4))
	{
		Channel_States[CH4] = DMA_IDLE;
		clear_TCFlag(CH4);
		if(CB_Handlers.Channel_TCHandler[CH4])
		{
			CB_Handlers.Channel_TCHandler[CH4]();
		}
	}
	/*In case of half Transmission complete*/
	else if(get_HTCFlagStatus(CH4))
	{
		clear_HTCFlag(CH4);
		if(CB_Handlers.Channel_HTCHandler[CH4])
		{
			CB_Handlers.Channel_HTCHandler[CH4]();
		}
	}
	/*In case of Transmission error*/
	else if(get_ERRFlagStatus(CH4))
	{
		clear_ERRFlag(CH4);
		if(CB_Handlers.Channel_ERRHandler[CH4])
		{
			CB_Handlers.Channel_ERRHandler[CH4]();
		}
	}
}

void DMA1_Channel5_IRQHandler(void)
{
	/*In case of Transmission complete*/
	/*USART Receive*/
  	if(get_TCFlagStatus(CH5))
	{
		Channel_States[CH5] = DMA_IDLE;
		clear_TCFlag(CH5);
		if(CB_Handlers.Channel_TCHandler[CH5])
		{
			CB_Handlers.Channel_TCHandler[CH5]();
		}
	}
	/*In case of half Transmission complete*/
	else if(get_HTCFlagStatus(CH5))
	{
		clear_HTCFlag(CH5);
		if(CB_Handlers.Channel_HTCHandler[CH5])
		{
			CB_Handlers.Channel_HTCHandler[CH5]();
		}
	}
	/*In case of Transmission error*/
	else if(get_ERRFlagStatus(CH5))
	{
		clear_ERRFlag(CH5);
		if(CB_Handlers.Channel_ERRHandler[CH5])
		{
			CB_Handlers.Channel_ERRHandler[CH5]();
		}
	}
}

void DMA1_Channel6_IRQHandler(void)
{
	/*In case of Transmission complete*/
	if(get_TCFlagStatus(CH6))
	{
		Channel_States[CH6] = DMA_IDLE;
		clear_TCFlag(CH6);
		if(CB_Handlers.Channel_TCHandler[CH6])
		{
			CB_Handlers.Channel_TCHandler[CH6]();
		}
	}
	/*In case of half Transmission complete*/
	else if(get_HTCFlagStatus(CH6))
	{
		clear_HTCFlag(CH6);
		if(CB_Handlers.Channel_HTCHandler[CH6])
		{
			CB_Handlers.Channel_HTCHandler[CH6]();
		}
	}
	/*In case of Transmission error*/
	else if(get_ERRFlagStatus(CH6))
	{
		clear_ERRFlag(CH6);
		if(CB_Handlers.Channel_ERRHandler[CH6])
		{
			CB_Handlers.Channel_ERRHandler[CH6]();
		}
	}
}

void DMA1_Channel7_IRQHandler(void)
{
	/*In case of Transmission complete*/
	if(get_TCFlagStatus(CH7))
	{
		Channel_States[CH7] = DMA_IDLE;
		clear_TCFlag(CH7);
		if(CB_Handlers.Channel_TCHandler[CH7])
		{
			CB_Handlers.Channel_TCHandler[CH7]();
		}
	}
	/*In case of half Transmission complete*/
	else if(get_HTCFlagStatus(CH7))
	{
		clear_HTCFlag(CH7);
		if(CB_Handlers.Channel_HTCHandler[CH7])
		{
			CB_Handlers.Channel_HTCHandler[CH7]();
		}
	}
	/*In case of Transmission error*/
	else if(get_ERRFlagStatus(CH7))
	{
		clear_ERRFlag(CH7);
		if(CB_Handlers.Channel_ERRHandler[CH7])
		{
			CB_Handlers.Channel_ERRHandler[CH7]();
		}
	}
}
