#define SWS_HSI						0x00
#define SWS_HSE						0x04
#define SWS_PLL						0x08
#define HSI_RDY						0x02
#define HSE_RDY						0x00020000
#define PLL_RDY						0x02000000
#define CLK_STATUS_ON				0x01
#define CLK_STATUS_OFF				0x00
#define	HSI_CLK						0x80000001
#define	HSE_CLK						0x80010000
#define	PLL_CLK						0x81000000
#define HSI							0x00
#define HSE							0x01
#define PLL							0x02
#define RC							0x00040000
#define CRYSTAL						0xfffbffff
#define PLL_input_clock_x2			0x40000000
#define PLL_input_clock_x3			0x40040000
#define PLL_input_clock_x4			0x40080000
#define PLL_input_clock_x5			0x400C0000
#define PLL_input_clock_x6			0x40100000
#define PLL_input_clock_x7			0x40140000
#define PLL_input_clock_x8			0x40180000
#define PLL_input_clock_x9			0x401C0000
#define PLL_input_clock_x10			0x40200000
#define PLL_input_clock_x11			0x40240000
#define PLL_input_clock_x12			0x40280000
#define PLL_input_clock_x13			0x402C0000
#define PLL_input_clock_x14			0x40300000
#define PLL_input_clock_x15			0x40340000
#define PLL_input_clock_x16			0x40380000
#define PLL_CLOCK_HSI_2				0x40000000
#define PLL_CLOCK_HSE				0x40010000
#define PLL_CLOCK_HSE_2				0x40030000
#define	MCO_NO_CLK					0x20000000
#define	MCO_SYS_CLK					0x24000000
#define	MCO_HSI_CLK					0x25000000
#define	MCO_HSE_CLK					0x26000000
#define	MCO_PLL_CLK					0x27000000
#define APB2_HCLK_NOT_DIVIDED		0x10000000
#define APB2_HCLK_DIVIDED_BY_2		0x10002000
#define APB2_HCLK_DIVIDED_BY_4		0x10002800
#define APB2_HCLK_DIVIDED_BY_8		0x10003000
#define APB2_HCLK_DIVIDED_BY_16		0x10003800
#define APB1_HCLK_NOT_DIVIDED		0x10000000
#define APB1_HCLK_DIVIDED_BY_2		0x10000400
#define APB1_HCLK_DIVIDED_BY_4		0x10000500
#define APB1_HCLK_DIVIDED_BY_8		0x10000600
#define APB1_HCLK_DIVIDED_BY_16		0x10000700
#define SYSCLK_NOT_DIVIDED			0x08000000
#define SYSCLK_DIVIDED_BY_2			0x08000080
#define SYSCLK_DIVIDED_BY_4			0x08000090
#define SYSCLK_DIVIDED_BY_8			0x080000A0
#define SYSCLK_DIVIDED_BY_16		0x080000B0
#define SYSCLK_DIVIDED_BY_64		0x080000C0
#define SYSCLK_DIVIDED_BY_128		0x080000D0
#define SYSCLK_DIVIDED_BY_256		0x080000E0
#define SYSCLK_DIVIDED_BY_512		0x080000F0
#define PERIPHERAL_ON				0xff
#define PERIPHERAL_OFF				0xfe
#define DMA1						0x20000001
#define DMA2						0x20000002
#define SRAM						0x20000004
#define FLITF						0x20000010
#define CRC							0x20000040
#define FSMC						0x20000100
#define SDIO						0x20000400
#define TIM2						0x80000001
#define TIM3						0x80000002
#define TIM4						0x80000004
#define TIM5						0x80000008
#define TIM6						0x80000010
#define TIM7						0x80000020
#define TIM12						0x80000040
#define TIM13						0x80000080
#define TIM14						0x80000100
#define WWDG						0x80000800
#define SPI2						0x80004000
#define SPI3						0x80008000
#define USART2						0x80020000
#define USART3						0x80040000
#define USART4						0x80080000
#define USART5						0x80100000
#define I2C1						0x80200000
#define I2C2						0x80400000
#define USB							0x80800000
#define CAN							0x82000000
#define BKP							0x88000000
#define PWR							0x90000000
#define DAC							0xA0000000
#define AFIO						0x40000001
#define IOPA						0x40000004
#define IOPB						0x40000008
#define IOPC						0x40000010
#define IOPD						0x40000020
#define IOPE						0x40000040
#define IOPF						0x40000080
#define IOPG						0x40000100
#define ADC1						0x40000200
#define ADC2						0x40000400
#define TIM1						0x40000800
#define SPI1						0x40001000
#define TIM8						0x40002000
#define USART1						0x40004000
#define ADC3						0x40008000
#define TIM9						0x40080000
#define TIM10						0x40100000
#define TIM11						0x40200000
/*
Check the passed argument against
SWS_HSI
SWS_HSE
SWS_PLL
to know the current system clock
*/
u8 RCC_u8ReadSystemClock(u8* clock);
/*
Check the passed argument against
HSI_RDY
to know if HSI is on
*/
u8 RCC_u8CheckHSI_ON(u8* value);
/*
Check the passed argument against
HSE_RDY
to know if HSE is on
*/
u8 RCC_u8CheckHSE_ON(u32* value);
/*
Check the passed argument against
PLL_RDY
to know if PLL is on
*/
u8 RCC_u8CheckPLL_ON(u32* value);
/*
Pass HSI_CLK , HSE_CLK , PLL_CLK in the Clock argument to change its status
pass CLK_STATUS_ON ,  CLK_STATUS_OFF in mode to turn on/off the clock
*/
u8 RCC_u8ChangeClockStatus(u32 Clock , u8 mode);
/*
 Pass the desired system clock to this function
 HSI,
 HSE,
 PLL
 as an argument (make sure it's on first)
*/
u8 RCC_u8SwitchSystemClock(u8 Clock);

/*
 bypath choices :
 CRYSTAL,
 RC
 */
u8 RCC_HSE_HSEBYP(u32 bypath);
/*
 This function takes mul parameter to determine the factor it will multiply the PLL in with options :
 	PLL_input_clock_x2,
	PLL_input_clock_x3,
	PLL_input_clock_x4,
	PLL_input_clock_x5,
	PLL_input_clock_x6,
	PLL_input_clock_x7,
	PLL_input_clock_x8,
	PLL_input_clock_x9,
	PLL_input_clock_x10,
	PLL_input_clock_x11,
	PLL_input_clock_x12,
	PLL_input_clock_x13,
	PLL_input_clock_x14,
	PLL_input_clock_x15,
	PLL_input_clock_x16
 You can chose which clock source you want for the PLL the options for clock_source:
	PLL_CLOCK_HSI_2
	PLL_CLOCK_HSE
	PLL_CLOCK_HSE_2
 */

u8 RCC_PLL_Config(u32 mul,u32 clock_source);
/*
This function outputs a clock based on choice in the input argument (Clock) which are :
	MCO_NO_CLK,
	MCO_SYS_CLK,
	MCO_HSI_CLK,
	MCO_HSE_CLK,
	MCO_PLL_CLK
*/
u8 RCC_MCO_Config(u32 Clock);

/*
This function Configures the APB2 prescaler with options
	HCLK_NOT_DIVIDED
	APB2_HCLK_DIVIDED_BY_2
	APB2_HCLK_DIVIDED_BY_4
	APB2_HCLK_DIVIDED_BY_8
	APB2_HCLK_DIVIDED_BY_16
*/

u8 RCC_APB2Clock_Config(u32 Prescaler);

/*
This function Configures the APB1 prescaler with options
	HCLK_NOT_DIVIDED
	APB1_HCLK_DIVIDED_BY_2
	APB1_HCLK_DIVIDED_BY_4
	APB1_HCLK_DIVIDED_BY_8
	APB1_HCLK_DIVIDED_BY_16
*/

u8 RCC_APB1Clock_Config(u32 Prescaler);
/*
This function Configures the AHB prescaler with options
	SYSCLK_NOT_DIVIDED
	SYSCLK_DIVIDED_BY_2
	SYSCLK_DIVIDED_BY_4
	SYSCLK_DIVIDED_BY_8
	SYSCLK_DIVIDED_BY_16
	SYSCLK_DIVIDED_BY_64
	SYSCLK_DIVIDED_BY_128
	SYSCLK_DIVIDED_BY_256
	SYSCLK_DIVIDED_BY_512
*/

u8 RCC_AHBClock_Config(u32 Prescaler);

/*
This function Enables/Disables the clock to a peripheral with peripheral choices passed as an argument :
  DMA1,
  DMA2,
  SRAM ,
  FLITF,
  CRC  ,
  FSMC ,
  SDIO ,
  TIM2 ,
  TIM3 ,
  TIM4 ,
  TIM5 ,
  TIM6 ,
  TIM7 ,
  TIM12,
  TIM13,
  TIM14,
  WWDG ,
  SPI2 ,
  SPI2 ,
  USART200,
  USART300,
  USART400,
  USART500,
  I2C1,
  I2C2,
  USB ,
  CAN ,
  BKP ,
  PWR ,
  DAC ,
  AFIO,
  IOPA,
  IOPB,
  IOPC,
  IOPD,
  IOPE,
  IOPF,
  IOPG,
  ADC1,
  ADC2,
  TIM1,
  SPI1,
  TIM8,
  USART1,
  ADC3 ,
  TIM9 ,
  TIM10,
  TIM11,
And it has two modes (on/off) to enable or disable the clock input to the peripheral :
 PERIPHERAL_ON,
 PERIPHERAL_OFF

*/

u8 RCC_Peripheral_Config(u32 Peripheral ,u8 mode);

/*
 * This function returns the system frequency in HZ
 */
u32 RCC_u32ReadSystemFreq(void);
/*
 * This function returns the AHB clock in HZ
 */
u32 RCC_u32ReadAHBFreq(void);
/*
 * This function returns the APB2 clock in HZ
 */
u32 RCC_u32ReadAPB2Freq(void);
