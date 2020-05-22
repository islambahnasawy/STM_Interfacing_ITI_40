#ifndef _NVIC_INTERFACE_H_
#define _NVIC_INTERFACE_H_

/*
 * These macros are passed as an argument to NVIC_SetPriorityGrouping function
 * to determine the number of groups and subgroups
 */
#define GROUP_16_SUB_NONE 	0x05fa0300
#define GROUP_8_SUB_2		0x05fa0400
#define GROUP_4_SUB_4		0x05fa0500
#define GROUP_2_SUB_8		0x05fa0600
#define GROUP_NONE_SUB_16	0x05fa0700

/*
 * These macros are the interrupt request numbers for STM32F10xxx devices
 */
#define NVIC_WWDG 				0
#define NVIC_PVD				1
#define NVIC_TAMPER				2
#define NVIC_RTC				3
#define	NVIC_FLASH				4
#define NVIC_RCC				5
#define NVIC_EXTI0				6
#define NVIC_EXTI1				7
#define NVIC_EXTI2				8
#define NVIC_EXTI3				9
#define NVIC_EXTI4				10
#define NVIC_DMA1_CHANNEL1		11
#define NVIC_DMA1_CHANNEL2		12
#define NVIC_DMA1_CHANNEL3		13
#define NVIC_DMA1_CHANNEL4		14
#define NVIC_DMA1_CHANNEL5		15
#define NVIC_DMA1_CHANNEL6		16
#define NVIC_DMA1_CHANNEL7		17
#define NVIC_ADC1_2 			18
#define NVIC_USB_HP_CAN_TX		19
#define NVIC_USB_LP_CAN_RX0		20
#define NVIC_CAN_RX1			21
#define NVIC_CAN_SCE			22
#define NVIC_EXTI9_5			23
#define NVIC_TIM1_BRK			24
#define NVIC_TIM1_UP			25
#define NVIC_TIM1_TRG_COM		26
#define NVIC_TIM1_CC			27
#define NVIC_TIM2				28
#define NVIC_TIM3				29
#define NVIC_TIM4				30
#define NVIC_I2C1_EV			31
#define NVIC_I2C1_ER			32
#define NVIC_I2c2_EV			33
#define NVIC_I2C2_ER			34
#define NVIC_SPI1				35
#define NVIC_SPI2				36
#define NVIC_USART1				37
#define NVIC_USART2				38
#define NVIC_USART3				39
#define NVIC_EXTI15_10			40
#define NVIC_RTCALARM			41
#define NVIC_USBWAKEUP			42
#define NVIC_TIM8_BRK			43
#define NVIC_TIM8_UP			44
#define NVIC_TIM8_TRG_COM		45
#define NVIC_TIM8_CC			46
#define NVIC_ADC3				47
#define NVIC_FSMC				48
#define NVIC_SDIO				49
#define NVIC_TIM5				50
#define NVIC_SPI3				51
#define NVIC_UART4				52
#define NVIC_UART5				53
#define NVIC_TIM6				54
#define NVIC_TIM7				55
#define NVIC_DMA2_CHANNEL1		56
#define NVIC_DMA2_CHANNEL2		57
#define NVIC_DMA2_CHANNEL3		58
#define NVIC_DMA2_CHANNEL4_5	59

/*
 * These macros indicates the status of the pending flag
 */
#define NVIC_PENDING_ON			1
#define NVIC_PNEDING_OFF		0
/*
 * These macros indicates the status of the active flag
 */
#define NVIC_ACTIVE_ON			1
#define NVIC_ACTIVE_OFF			0

/******************************************************************************************
 * function name: NVIC_SetPriorityGrouping
 *
 * parameters:  Input:
 *                priority_grouping
 *                     type: u32
 *                     Description: Number of Groups and subgroups
 *                     options :
 *                     GROUP_16_SUB_NONE,
 *	 	 	 	 	   GROUP_8_SUB_2,
 *	 	 	 	 	   GROUP_4_SUB_4,
 *	 	 	 	 	   GROUP_2_SUB_8,
 *	 	 	 	 	   GROUP_NONE_SUB_16
 *
 *              Output: NA
 *              In/out: NA
 *
 * return: void
 * Description: a function to configure the number of interrupt groups and subgroups
********************************************************************************************/
void NVIC_SetPriorityGrouping(u32 priority_grouping);

/******************************************************************************************
* function name: NVIC_EnableIRQ
*
* parameters:  Input:
*                IRQn
*                     type: u8
*                     Description: Interrupt request number
*                     options :         
*	 	 	 	 	   NVIC_WWDG 		 ,	
*	 	 	 	 	   NVIC_PVD			 ,
*	 	 	 	 	   NVIC_TAMPER		 ,
*					   NVIC_RTC			 ,
*					   NVIC_FLASH		 ,
*					   NVIC_RCC			 ,
*					   NVIC_EXTI0		 ,
*					   NVIC_EXTI1		 ,
*					   NVIC_EXTI2		 ,
*					   NVIC_EXTI3		 ,
*					   NVIC_EXTI4		 ,
*					   NVIC_DMA1_CHANNEL1,
*					   NVIC_DMA1_CHANNEL2,
*					   NVIC_DMA1_CHANNEL3,
*					   NVIC_DMA1_CHANNEL4,
*					   NVIC_DMA1_CHANNEL5,
*					   NVIC_DMA1_CHANNEL6,
*					   NVIC_DMA1_CHANNEL7,
*					   NVIC_ADC1_2 		 ,
*					   NVIC_USB_HP_CAN_TX,
*					   NVIC_USB_LP_CAN_RX,
*					   NVIC_CAN_RX1		 ,
*					   NVIC_CAN_SCE		 ,
*					   NVIC_EXTI9_5		 ,
*					   NVIC_TIM1_BRK	 ,
*					   NVIC_TIM1_UP		 ,
*					   NVIC_TIM1_TRG_COM ,
*					   NVIC_TIM1_CC		 ,
*					   NVIC_TIM2		 ,
*					   NVIC_TIM3		 ,
*					   NVIC_TIM4		 ,
*					   NVIC_I2C1_EV		 ,
*					   NVIC_I2C1_ER		 ,
*					   NVIC_I2c2_EV		 ,
*					   NVIC_I2C2_ER		 ,
*					   NVIC_SPI1		 ,
*					   NVIC_SPI2		 ,
*					   NVIC_USART1		 ,
*					   NVIC_USART2		 ,
*					   NVIC_USART3		 ,
*					   NVIC_EXTI15_10	 ,
*					   NVIC_RTCALARM	 ,
*					   NVIC_USBWAKEUP	 ,
*					   NVIC_TIM8_BRK	 ,
*					   NVIC_TIM8_UP		 ,
*					   NVIC_TIM8_TRG_COM ,
*					   NVIC_TIM8_CC		 ,
*					   NVIC_ADC3		 ,
*					   NVIC_FSMC		 ,
*					   NVIC_SDIO		 ,
*					   NVIC_TIM5		 ,
*					   NVIC_SPI3		 ,
*					   NVIC_UART4		 ,
*					   NVIC_UART5		 ,
*					   NVIC_TIM6		 ,
*					   NVIC_TIM7		 ,
*					   NVIC_DMA2_CHANNEL1,
*					   NVIC_DMA2_CHANNEL2,
*					   NVIC_DMA2_CHANNEL3,
*					   NVIC_DMA2_CHANNEL4_5
*
*              Output: NA
*              In/out: NA
*
* return: OK , NOT_OK
* Description: a function to enable an interrupt request
********************************************************************************************/
u8 NVIC_EnableIRQ(u8 IRQn);

/******************************************************************************************
* function name: NVIC_DisableIRQ
*
* parameters:  Input:
*                IRQn
*                     type: u8
*                     Description: Interrupt request number
*                     options :
*	 	 	 	 	   NVIC_WWDG 		 ,
*	 	 	 	 	   NVIC_PVD			 ,
*	 	 	 	 	   NVIC_TAMPER		 ,
*					   NVIC_RTC			 ,
*					   NVIC_FLASH		 ,
*					   NVIC_RCC			 ,
*					   NVIC_EXTI0		 ,
*					   NVIC_EXTI1		 ,
*					   NVIC_EXTI2		 ,
*					   NVIC_EXTI3		 ,
*					   NVIC_EXTI4		 ,
*					   NVIC_DMA1_CHANNEL1,
*					   NVIC_DMA1_CHANNEL2,
*					   NVIC_DMA1_CHANNEL3,
*					   NVIC_DMA1_CHANNEL4,
*					   NVIC_DMA1_CHANNEL5,
*					   NVIC_DMA1_CHANNEL6,
*					   NVIC_DMA1_CHANNEL7,
*					   NVIC_ADC1_2 		 ,
*					   NVIC_USB_HP_CAN_TX,
*					   NVIC_USB_LP_CAN_RX,
*					   NVIC_CAN_RX1		 ,
*					   NVIC_CAN_SCE		 ,
*					   NVIC_EXTI9_5		 ,
*					   NVIC_TIM1_BRK	 ,
*					   NVIC_TIM1_UP		 ,
*					   NVIC_TIM1_TRG_COM ,
*					   NVIC_TIM1_CC		 ,
*					   NVIC_TIM2		 ,
*					   NVIC_TIM3		 ,
*					   NVIC_TIM4		 ,
*					   NVIC_I2C1_EV		 ,
*					   NVIC_I2C1_ER		 ,
*					   NVIC_I2c2_EV		 ,
*					   NVIC_I2C2_ER		 ,
*					   NVIC_SPI1		 ,
*					   NVIC_SPI2		 ,
*					   NVIC_USART1		 ,
*					   NVIC_USART2		 ,
*					   NVIC_USART3		 ,
*					   NVIC_EXTI15_10	 ,
*					   NVIC_RTCALARM	 ,
*					   NVIC_USBWAKEUP	 ,
*					   NVIC_TIM8_BRK	 ,
*					   NVIC_TIM8_UP		 ,
*					   NVIC_TIM8_TRG_COM ,
*					   NVIC_TIM8_CC		 ,
*					   NVIC_ADC3		 ,
*					   NVIC_FSMC		 ,
*					   NVIC_SDIO		 ,
*					   NVIC_TIM5		 ,
*					   NVIC_SPI3		 ,
*					   NVIC_UART4		 ,
*					   NVIC_UART5		 ,
*					   NVIC_TIM6		 ,
*					   NVIC_TIM7		 ,
*					   NVIC_DMA2_CHANNEL1,
*					   NVIC_DMA2_CHANNEL2,
*					   NVIC_DMA2_CHANNEL3,
*					   NVIC_DMA2_CHANNEL4_5
*
*              Output: NA
*              In/out: NA
*
* return: OK , NOT_OK
* Description: a function to disable an interrupt request
********************************************************************************************/
u8 NVIC_DisableIRQ(u8 IRQn);

/******************************************************************************************
* function name: NVIC_GetPendingIRQ
*
* parameters:  Input:
*                IRQn
*                     type: u8
*                     Description: Interrupt request number
*                     options :
*	 	 	 	 	   NVIC_WWDG 		 ,
*	 	 	 	 	   NVIC_PVD			 ,
*	 	 	 	 	   NVIC_TAMPER		 ,
*					   NVIC_RTC			 ,
*					   NVIC_FLASH		 ,
*					   NVIC_RCC			 ,
*					   NVIC_EXTI0		 ,
*					   NVIC_EXTI1		 ,
*					   NVIC_EXTI2		 ,
*					   NVIC_EXTI3		 ,
*					   NVIC_EXTI4		 ,
*					   NVIC_DMA1_CHANNEL1,
*					   NVIC_DMA1_CHANNEL2,
*					   NVIC_DMA1_CHANNEL3,
*					   NVIC_DMA1_CHANNEL4,
*					   NVIC_DMA1_CHANNEL5,
*					   NVIC_DMA1_CHANNEL6,
*					   NVIC_DMA1_CHANNEL7,
*					   NVIC_ADC1_2 		 ,
*					   NVIC_USB_HP_CAN_TX,
*					   NVIC_USB_LP_CAN_RX,
*					   NVIC_CAN_RX1		 ,
*					   NVIC_CAN_SCE		 ,
*					   NVIC_EXTI9_5		 ,
*					   NVIC_TIM1_BRK	 ,
*					   NVIC_TIM1_UP		 ,
*					   NVIC_TIM1_TRG_COM ,
*					   NVIC_TIM1_CC		 ,
*					   NVIC_TIM2		 ,
*					   NVIC_TIM3		 ,
*					   NVIC_TIM4		 ,
*					   NVIC_I2C1_EV		 ,
*					   NVIC_I2C1_ER		 ,
*					   NVIC_I2c2_EV		 ,
*					   NVIC_I2C2_ER		 ,
*					   NVIC_SPI1		 ,
*					   NVIC_SPI2		 ,
*					   NVIC_USART1		 ,
*					   NVIC_USART2		 ,
*					   NVIC_USART3		 ,
*					   NVIC_EXTI15_10	 ,
*					   NVIC_RTCALARM	 ,
*					   NVIC_USBWAKEUP	 ,
*					   NVIC_TIM8_BRK	 ,
*					   NVIC_TIM8_UP		 ,
*					   NVIC_TIM8_TRG_COM ,
*					   NVIC_TIM8_CC		 ,
*					   NVIC_ADC3		 ,
*					   NVIC_FSMC		 ,
*					   NVIC_SDIO		 ,
*					   NVIC_TIM5		 ,
*					   NVIC_SPI3		 ,
*					   NVIC_UART4		 ,
*					   NVIC_UART5		 ,
*					   NVIC_TIM6		 ,
*					   NVIC_TIM7		 ,
*					   NVIC_DMA2_CHANNEL1,
*					   NVIC_DMA2_CHANNEL2,
*					   NVIC_DMA2_CHANNEL3,
*					   NVIC_DMA2_CHANNEL4_5
*				value
*						type : u32 *
*						Description: The value in this pointer to u32 indicates if there is a pending request , this variable shall hold one of two values
*						options :
*						NVIC_PENDING_OFF
*						NVIC_PENDING_ON
*              Output: NA
*              In/out: NA
*
* return: OK , NOT_OK
* Description: a function to check if the pending flag is risen for a given interrupt request number
********************************************************************************************/
u8 NVIC_GetPendingIRQ (u8 IRQn ,u32* value);

/******************************************************************************************
* function name: NVIC_SetPendingIRQ
*
* parameters:  Input:
*                IRQn
*                     type: u8
*                     Description: Interrupt request number
*                     options :
*	 	 	 	 	   NVIC_WWDG 		 ,
*	 	 	 	 	   NVIC_PVD			 ,
*	 	 	 	 	   NVIC_TAMPER		 ,
*					   NVIC_RTC			 ,
*					   NVIC_FLASH		 ,
*					   NVIC_RCC			 ,
*					   NVIC_EXTI0		 ,
*					   NVIC_EXTI1		 ,
*					   NVIC_EXTI2		 ,
*					   NVIC_EXTI3		 ,
*					   NVIC_EXTI4		 ,
*					   NVIC_DMA1_CHANNEL1,
*					   NVIC_DMA1_CHANNEL2,
*					   NVIC_DMA1_CHANNEL3,
*					   NVIC_DMA1_CHANNEL4,
*					   NVIC_DMA1_CHANNEL5,
*					   NVIC_DMA1_CHANNEL6,
*					   NVIC_DMA1_CHANNEL7,
*					   NVIC_ADC1_2 		 ,
*					   NVIC_USB_HP_CAN_TX,
*					   NVIC_USB_LP_CAN_RX,
*					   NVIC_CAN_RX1		 ,
*					   NVIC_CAN_SCE		 ,
*					   NVIC_EXTI9_5		 ,
*					   NVIC_TIM1_BRK	 ,
*					   NVIC_TIM1_UP		 ,
*					   NVIC_TIM1_TRG_COM ,
*					   NVIC_TIM1_CC		 ,
*					   NVIC_TIM2		 ,
*					   NVIC_TIM3		 ,
*					   NVIC_TIM4		 ,
*					   NVIC_I2C1_EV		 ,
*					   NVIC_I2C1_ER		 ,
*					   NVIC_I2c2_EV		 ,
*					   NVIC_I2C2_ER		 ,
*					   NVIC_SPI1		 ,
*					   NVIC_SPI2		 ,
*					   NVIC_USART1		 ,
*					   NVIC_USART2		 ,
*					   NVIC_USART3		 ,
*					   NVIC_EXTI15_10	 ,
*					   NVIC_RTCALARM	 ,
*					   NVIC_USBWAKEUP	 ,
*					   NVIC_TIM8_BRK	 ,
*					   NVIC_TIM8_UP		 ,
*					   NVIC_TIM8_TRG_COM ,
*					   NVIC_TIM8_CC		 ,
*					   NVIC_ADC3		 ,
*					   NVIC_FSMC		 ,
*					   NVIC_SDIO		 ,
*					   NVIC_TIM5		 ,
*					   NVIC_SPI3		 ,
*					   NVIC_UART4		 ,
*					   NVIC_UART5		 ,
*					   NVIC_TIM6		 ,
*					   NVIC_TIM7		 ,
*					   NVIC_DMA2_CHANNEL1,
*					   NVIC_DMA2_CHANNEL2,
*					   NVIC_DMA2_CHANNEL3,
*					   NVIC_DMA2_CHANNEL4_5
*
*              Output: NA
*              In/out: NA
*
* return: OK , NOT_OK
* Description: a function to set the pending flag of an interrupt request number
********************************************************************************************/
u8 NVIC_SetPendingIRQ (u8 IRQn);

/******************************************************************************************
* function name: NVIC_ClearPendingIRQ
*
* parameters:  Input:
*                IRQn
*                     type: u8
*                     Description: Interrupt request number
*                     options :
*	 	 	 	 	   NVIC_WWDG 		 ,
*	 	 	 	 	   NVIC_PVD			 ,
*	 	 	 	 	   NVIC_TAMPER		 ,
*					   NVIC_RTC			 ,
*					   NVIC_FLASH		 ,
*					   NVIC_RCC			 ,
*					   NVIC_EXTI0		 ,
*					   NVIC_EXTI1		 ,
*					   NVIC_EXTI2		 ,
*					   NVIC_EXTI3		 ,
*					   NVIC_EXTI4		 ,
*					   NVIC_DMA1_CHANNEL1,
*					   NVIC_DMA1_CHANNEL2,
*					   NVIC_DMA1_CHANNEL3,
*					   NVIC_DMA1_CHANNEL4,
*					   NVIC_DMA1_CHANNEL5,
*					   NVIC_DMA1_CHANNEL6,
*					   NVIC_DMA1_CHANNEL7,
*					   NVIC_ADC1_2 		 ,
*					   NVIC_USB_HP_CAN_TX,
*					   NVIC_USB_LP_CAN_RX,
*					   NVIC_CAN_RX1		 ,
*					   NVIC_CAN_SCE		 ,
*					   NVIC_EXTI9_5		 ,
*					   NVIC_TIM1_BRK	 ,
*					   NVIC_TIM1_UP		 ,
*					   NVIC_TIM1_TRG_COM ,
*					   NVIC_TIM1_CC		 ,
*					   NVIC_TIM2		 ,
*					   NVIC_TIM3		 ,
*					   NVIC_TIM4		 ,
*					   NVIC_I2C1_EV		 ,
*					   NVIC_I2C1_ER		 ,
*					   NVIC_I2c2_EV		 ,
*					   NVIC_I2C2_ER		 ,
*					   NVIC_SPI1		 ,
*					   NVIC_SPI2		 ,
*					   NVIC_USART1		 ,
*					   NVIC_USART2		 ,
*					   NVIC_USART3		 ,
*					   NVIC_EXTI15_10	 ,
*					   NVIC_RTCALARM	 ,
*					   NVIC_USBWAKEUP	 ,
*					   NVIC_TIM8_BRK	 ,
*					   NVIC_TIM8_UP		 ,
*					   NVIC_TIM8_TRG_COM ,
*					   NVIC_TIM8_CC		 ,
*					   NVIC_ADC3		 ,
*					   NVIC_FSMC		 ,
*					   NVIC_SDIO		 ,
*					   NVIC_TIM5		 ,
*					   NVIC_SPI3		 ,
*					   NVIC_UART4		 ,
*					   NVIC_UART5		 ,
*					   NVIC_TIM6		 ,
*					   NVIC_TIM7		 ,
*					   NVIC_DMA2_CHANNEL1,
*					   NVIC_DMA2_CHANNEL2,
*					   NVIC_DMA2_CHANNEL3,
*					   NVIC_DMA2_CHANNEL4_5
*
*              Output: NA
*              In/out: NA
*
* return: OK , NOT_OK
* Description: a function to clear pending flag  for an  interrupt request number
********************************************************************************************/
u8 NVIC_ClearPendingIRQ (u8 IRQn);

/******************************************************************************************
* function name: NVIC_GetActive
*
* parameters:  Input:
*                IRQn
*                     type: u8
*                     Description: Interrupt request number
*                     options :
*	 	 	 	 	   NVIC_WWDG 		 ,
*	 	 	 	 	   NVIC_PVD			 ,
*	 	 	 	 	   NVIC_TAMPER		 ,
*					   NVIC_RTC			 ,
*					   NVIC_FLASH		 ,
*					   NVIC_RCC			 ,
*					   NVIC_EXTI0		 ,
*					   NVIC_EXTI1		 ,
*					   NVIC_EXTI2		 ,
*					   NVIC_EXTI3		 ,
*					   NVIC_EXTI4		 ,
*					   NVIC_DMA1_CHANNEL1,
*					   NVIC_DMA1_CHANNEL2,
*					   NVIC_DMA1_CHANNEL3,
*					   NVIC_DMA1_CHANNEL4,
*					   NVIC_DMA1_CHANNEL5,
*					   NVIC_DMA1_CHANNEL6,
*					   NVIC_DMA1_CHANNEL7,
*					   NVIC_ADC1_2 		 ,
*					   NVIC_USB_HP_CAN_TX,
*					   NVIC_USB_LP_CAN_RX,
*					   NVIC_CAN_RX1		 ,
*					   NVIC_CAN_SCE		 ,
*					   NVIC_EXTI9_5		 ,
*					   NVIC_TIM1_BRK	 ,
*					   NVIC_TIM1_UP		 ,
*					   NVIC_TIM1_TRG_COM ,
*					   NVIC_TIM1_CC		 ,
*					   NVIC_TIM2		 ,
*					   NVIC_TIM3		 ,
*					   NVIC_TIM4		 ,
*					   NVIC_I2C1_EV		 ,
*					   NVIC_I2C1_ER		 ,
*					   NVIC_I2c2_EV		 ,
*					   NVIC_I2C2_ER		 ,
*					   NVIC_SPI1		 ,
*					   NVIC_SPI2		 ,
*					   NVIC_USART1		 ,
*					   NVIC_USART2		 ,
*					   NVIC_USART3		 ,
*					   NVIC_EXTI15_10	 ,
*					   NVIC_RTCALARM	 ,
*					   NVIC_USBWAKEUP	 ,
*					   NVIC_TIM8_BRK	 ,
*					   NVIC_TIM8_UP		 ,
*					   NVIC_TIM8_TRG_COM ,
*					   NVIC_TIM8_CC		 ,
*					   NVIC_ADC3		 ,
*					   NVIC_FSMC		 ,
*					   NVIC_SDIO		 ,
*					   NVIC_TIM5		 ,
*					   NVIC_SPI3		 ,
*					   NVIC_UART4		 ,
*					   NVIC_UART5		 ,
*					   NVIC_TIM6		 ,
*					   NVIC_TIM7		 ,
*					   NVIC_DMA2_CHANNEL1,
*					   NVIC_DMA2_CHANNEL2,
*					   NVIC_DMA2_CHANNEL3,
*					   NVIC_DMA2_CHANNEL4_5
*				value
*						type : u32 *
*						Description: The value in this pointer to u32 indicates if there is a pending request , this variable shall hold one of two values
*						options :
*						NVIC_ACTIVE_OFF
*						NVIC_ACTIVE_ON
*              Output: NA
*              In/out: NA
*
* return: OK , NOT_OK
* Description: a function to check if the active flag is risen for a given interrupt request number
********************************************************************************************/
u8 NVIC_GetActive (u8 IRQn,u32* value);

/******************************************************************************************
* function name: NVIC_SetPriority
*
* parameters:  Input:
*                IRQn
*                     type: u8
*                     Description: Interrupt request number
*                     options :
*	 	 	 	 	   NVIC_WWDG 		 ,
*	 	 	 	 	   NVIC_PVD			 ,
*	 	 	 	 	   NVIC_TAMPER		 ,
*					   NVIC_RTC			 ,
*					   NVIC_FLASH		 ,
*					   NVIC_RCC			 ,
*					   NVIC_EXTI0		 ,
*					   NVIC_EXTI1		 ,
*					   NVIC_EXTI2		 ,
*					   NVIC_EXTI3		 ,
*					   NVIC_EXTI4		 ,
*					   NVIC_DMA1_CHANNEL1,
*					   NVIC_DMA1_CHANNEL2,
*					   NVIC_DMA1_CHANNEL3,
*					   NVIC_DMA1_CHANNEL4,
*					   NVIC_DMA1_CHANNEL5,
*					   NVIC_DMA1_CHANNEL6,
*					   NVIC_DMA1_CHANNEL7,
*					   NVIC_ADC1_2 		 ,
*					   NVIC_USB_HP_CAN_TX,
*					   NVIC_USB_LP_CAN_RX,
*					   NVIC_CAN_RX1		 ,
*					   NVIC_CAN_SCE		 ,
*					   NVIC_EXTI9_5		 ,
*					   NVIC_TIM1_BRK	 ,
*					   NVIC_TIM1_UP		 ,
*					   NVIC_TIM1_TRG_COM ,
*					   NVIC_TIM1_CC		 ,
*					   NVIC_TIM2		 ,
*					   NVIC_TIM3		 ,
*					   NVIC_TIM4		 ,
*					   NVIC_I2C1_EV		 ,
*					   NVIC_I2C1_ER		 ,
*					   NVIC_I2c2_EV		 ,
*					   NVIC_I2C2_ER		 ,
*					   NVIC_SPI1		 ,
*					   NVIC_SPI2		 ,
*					   NVIC_USART1		 ,
*					   NVIC_USART2		 ,
*					   NVIC_USART3		 ,
*					   NVIC_EXTI15_10	 ,
*					   NVIC_RTCALARM	 ,
*					   NVIC_USBWAKEUP	 ,
*					   NVIC_TIM8_BRK	 ,
*					   NVIC_TIM8_UP		 ,
*					   NVIC_TIM8_TRG_COM ,
*					   NVIC_TIM8_CC		 ,
*					   NVIC_ADC3		 ,
*					   NVIC_FSMC		 ,
*					   NVIC_SDIO		 ,
*					   NVIC_TIM5		 ,
*					   NVIC_SPI3		 ,
*					   NVIC_UART4		 ,
*					   NVIC_UART5		 ,
*					   NVIC_TIM6		 ,
*					   NVIC_TIM7		 ,
*					   NVIC_DMA2_CHANNEL1,
*					   NVIC_DMA2_CHANNEL2,
*					   NVIC_DMA2_CHANNEL3,
*					   NVIC_DMA2_CHANNEL4_5
*				priority
*						type : u8
*						Description: this parameter is used to configure the group number and subgroup number , check the number of bits allocated
*									 for the group and subgroup priority in the NVIC_SetPriorityGrouping function to be able to properly set the
*									 group and subgroup priority
*						options :
*						Depending on the NVIC_SetPriorityGrouping set the right number
*              Output: NA
*              In/out: NA
*
* return: OK , NOT_OK
* Description: a function to set the group and subgroup priority
********************************************************************************************/
u8 NVIC_SetPriority (u8 IRQn, u8 priority);

/******************************************************************************************
* function name: NVIC_GetPriority
*
* parameters:  Input:
*                IRQn
*                     type: u8
*                     Description: Interrupt request number
*                     options :
*	 	 	 	 	   NVIC_WWDG 		 ,
*	 	 	 	 	   NVIC_PVD			 ,
*	 	 	 	 	   NVIC_TAMPER		 ,
*					   NVIC_RTC			 ,
*					   NVIC_FLASH		 ,
*					   NVIC_RCC			 ,
*					   NVIC_EXTI0		 ,
*					   NVIC_EXTI1		 ,
*					   NVIC_EXTI2		 ,
*					   NVIC_EXTI3		 ,
*					   NVIC_EXTI4		 ,
*					   NVIC_DMA1_CHANNEL1,
*					   NVIC_DMA1_CHANNEL2,
*					   NVIC_DMA1_CHANNEL3,
*					   NVIC_DMA1_CHANNEL4,
*					   NVIC_DMA1_CHANNEL5,
*					   NVIC_DMA1_CHANNEL6,
*					   NVIC_DMA1_CHANNEL7,
*					   NVIC_ADC1_2 		 ,
*					   NVIC_USB_HP_CAN_TX,
*					   NVIC_USB_LP_CAN_RX,
*					   NVIC_CAN_RX1		 ,
*					   NVIC_CAN_SCE		 ,
*					   NVIC_EXTI9_5		 ,
*					   NVIC_TIM1_BRK	 ,
*					   NVIC_TIM1_UP		 ,
*					   NVIC_TIM1_TRG_COM ,
*					   NVIC_TIM1_CC		 ,
*					   NVIC_TIM2		 ,
*					   NVIC_TIM3		 ,
*					   NVIC_TIM4		 ,
*					   NVIC_I2C1_EV		 ,
*					   NVIC_I2C1_ER		 ,
*					   NVIC_I2c2_EV		 ,
*					   NVIC_I2C2_ER		 ,
*					   NVIC_SPI1		 ,
*					   NVIC_SPI2		 ,
*					   NVIC_USART1		 ,
*					   NVIC_USART2		 ,
*					   NVIC_USART3		 ,
*					   NVIC_EXTI15_10	 ,
*					   NVIC_RTCALARM	 ,
*					   NVIC_USBWAKEUP	 ,
*					   NVIC_TIM8_BRK	 ,
*					   NVIC_TIM8_UP		 ,
*					   NVIC_TIM8_TRG_COM ,
*					   NVIC_TIM8_CC		 ,
*					   NVIC_ADC3		 ,
*					   NVIC_FSMC		 ,
*					   NVIC_SDIO		 ,
*					   NVIC_TIM5		 ,
*					   NVIC_SPI3		 ,
*					   NVIC_UART4		 ,
*					   NVIC_UART5		 ,
*					   NVIC_TIM6		 ,
*					   NVIC_TIM7		 ,
*					   NVIC_DMA2_CHANNEL1,
*					   NVIC_DMA2_CHANNEL2,
*					   NVIC_DMA2_CHANNEL3,
*					   NVIC_DMA2_CHANNEL4_5
*				value
*						type : u8*
*						Description: holds the priority number for a given interrupt request number
*              Output: NA
*              In/out: NA
*
* return: OK , NOT_OK
* Description: a function to get the group and subgroup priority
********************************************************************************************/
u8 NVIC_GetPriority (u8 IRQn , u8*value);

/******************************************************************************************
* function name: NVIC_Enable_GlobalInterrupt
*
* parameters:  Input : NA
*              Output: NA
*              In/out: NA
*
* return: void
* Description: a function to enable the global interrupt
********************************************************************************************/
void NVIC_Enable_GlobalInterrupt(void);

/******************************************************************************************
* function name: NVIC_Disable_GlobalInterrupt
*
* parameters:  Input : NA
*              Output: NA
*              In/out: NA
*
* return: void
* Description: a function to disable the global interrupt
********************************************************************************************/
void NVIC_Disable_GlobalInterrupt(void);

/******************************************************************************************
* function name: NVIC_Enable_FaultMask
*
* parameters:  Input : NA
*              Output: NA
*              In/out: NA
*
* return: void
* Description: a function to enable the global interrupt and faults
********************************************************************************************/
void NVIC_Enable_FaultMask(void);

/******************************************************************************************
* function name: NVIC_Disable_FaultMask
*
* parameters:  Input : NA
*              Output: NA
*              In/out: NA
*
* return: void
* Description: a function to disable the global interrupt and faults
********************************************************************************************/
void NVIC_Disable_FaultMask(void);

/******************************************************************************************
* function name: NVIC_Disable_LowerPriority
*
* parameters:  Input:
*                priority
*                     type: u8
*                     Description: interrupts with GROUP Priority lower (higher in value) than this variable
*                     			   shall be disabled
*                     Options :
*                     			Put the group priority number and all interrupts that has an equal or higher
*                     			(in value) priority number will be disabled
*                     			0 to remove the effect of this function
*
*              Output: NA
*              In/out: NA
*
* return: void
* Description: a function to disable the interrupts which has a Group priority lower (higher in value)
* 			   than the input parameter and the rest are enabled
********************************************************************************************/
void NVIC_Disable_LowerPriority(u8 priority);


#endif
