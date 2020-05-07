/********************************************************/
/*Author : Mohamed Megahed , Islam El-Bahnasawy 		*/
/*Date   : 1 APRIL 2020									*/
/*Version : V01											*/
/********************************************************/

#ifndef _DMA_INTERFACE_H_
#define _DMA_INTERFACE_H_

/*Options for DMA priority*/
#define DMA_LOW_PL						0x00000000
#define DMA_MEDIUM_PL					0x00001000
#define DMA_HIGH_PL						0x00002000
#define DMA_VERYHIGH_PL					0x00003000


/*Options for Error interrupt*/
#define DMA_ERROR_INT_ENABLE			0x00000008
#define DMA_ERROR_INT_DISABLE			0x00000000

/*Options for Transmit compelete interrupt*/
#define DMA_TRANS_COMP_INT_ENABLE		0x00000002
#define DMA_TRANS_COMP_INT_DISABLE		0x00000000

/*Options for half Transimit complete interrupt*/
#define DMA_HALF_TRANS_COMP_INT_ENABLE	0x00000004
#define DMA_HALF_TRANS_COMP_INT_DISABLE	0x00000000

/*Channel Options*/
#define CH1 0
#define CH2 1
#define CH3 2
#define CH4 3
#define CH5 4
#define CH6 5
#define CH7 6

/* Transmission direction options (Trans_Dir)*/
#define PERI_TO_MEM						0x00000000
#define MEM_TO_PREI						0x00000010
#define MEM_TO_MEM						0x00004000

/*Circular Mode options (Circ_mode)*/
#define CIRCULAR_MODE_ENABLED			0x00000020
#define CIRCULAR_MODE_DISABLED			0x00000000

/*Peripheral increment mode options (Peri_Inc)*/
#define PERIPHERAL_INC_MODE_DISABLED	0x00000000
#define PERIPHERAL_INC_MODE_ENABLED		0x00000040

/*Memory increment mode options (Mem_Inc)*/
#define MEMORY_INC_MODE_DISABLED		0x00000000
#define MEMORY_INC_MODE_ENABLED			0x00000080

/*Peripheral size options (Peri_Size)*/
#define PERIPHERAL_SIZE_8_BITS			0x00000000
#define PERIPHERAL_SIZE_16_BITS			0x00000100
#define PERIPHERAL_SIZE_32_BITS			0x00000200

/*Memory size options (Mem_Size)*/
#define MEMORY_SIZE_8_BITS				0x00000000
#define MEMORY_SIZE_16_BITS				0x00000400
#define MEMORY_SIZE_32_BITS				0x00000800


/*Peripheral address options (Peri_Add)*/

/*************************Channel 1 Peripherals***********************/
#define DMA_CHN1_PERI_ADC1
#define DMA_CHN1_PERI_TIM2_CH3
#define DMA_CHN1_PERI_TIM4_CH1

/*************************Channel 2 Peripherals***********************/
#define DMA_CHN2_PERI_USART3_TX
#define DMA_CHN2_PERI_TIM1_CH1
#define DMA_CHN2_PERI_TIM2_UP
#define DMA_CHN2_PERI_TIM3_CH3
#define DMA_CHN2_PERI_SPI1_RX

/*************************Channel 3 Peripherals***********************/
#define DMA_CHN3_PERI_USART3_RX
#define DMA_CHN3_PERI_TIM1_CH2
#define DMA_CHN3_PERI_TIM3_CH4
#define DMA_CHN3_PERI_TIM3_UP
#define DMA_CHN3_PERI_SPI1_TX

/*************************Channel 4 Peripherals***********************/
#define DMA_CHN4_PERI_USART1_TX			0x40013804
#define DMA_CHN4_PERI_TIM1_CH4
#define DMA_CHN4_PERI_TIM1_TRIG
#define DMA_CHN4_PERI_TIM1_COM
#define DMA_CHN4_PERI_TIM4_CH2
#define DMA_CHN4_PERI_SPI/I2S2_RX
#define DMA_CHN4_PERI_I2C2_TX

/*************************Channel 5 Peripherals***********************/
#define DMA_CHN5_PERI_USART1_RX			0x40013804
#define DMA_CHN5_PERI_TIM1_UP
#define DMA_CHN5_PERI_SPI/I2S2_TX
#define DMA_CHN5_PERI_TIM2_CH1
#define DMA_CHN5_PERI_TIM4_CH3
#define DMA_CHN1_PERI_I2C2_RX

/*************************Channel 6 Peripherals***********************/
#define DMA_CHN6_PERI_USART2_RX
#define DMA_CHN6_PERI_TIM1_CH3
#define DMA_CHN6_PERI_TIM3_CH1
#define DMA_CHN6_PERI_TIM3_TRIG
#define DMA_CHN6_PERI_I2C1_TX

/*************************Channel 7 Peripherals***********************/
#define DMA_CHN1_PERI_USART2_TX
#define DMA_CHN1_PERI_TIM2_CH2
#define DMA_CHN1_PERI_TIM2_CH4
#define DMA_CHN1_PERI_TIM4_UP
#define DMA_CHN5_PERI_I2C1_RX

typedef void (*CHN_Handler) (void);

typedef struct
{ 
	u32 Trans_Dir;
	u32 Circ_mode;
	u32 Peri_Inc;
	u32 Mem_Inc;
	u32 Peri_Size;
	u32 Mem_Size;
	u32 Num_Of_Trans;
	u32 Peri_Add;
	u32 Mem_Add;
}DMA_data;


extern u8 Channel_States[];
extern u8 Channel_AlwaysBusy[];

void DMA_voidInit(void);

u8 DMA_u8ChannelInit(u8 Channel);

u8 DMA_voidStart(u8 Channel_Num ,DMA_data* data);

u8 set_ChannelX_TCHandler(u8 Channel_Num, CHN_Handler handler);

u8 set_ChannelX_HTCHandler(u8 Channel_Num, CHN_Handler handler);

u8 set_ChannelX_ERRHandler(u8 Channel_Num, CHN_Handler handler);

void DMA_voidStop(u8 Channel);

#endif
