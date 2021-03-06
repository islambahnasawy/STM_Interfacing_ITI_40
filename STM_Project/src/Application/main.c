#include "../LIB/STD_TYPES.h"
#include "../MCAL/USART/USART_MYTYPES.h"

#include "../RTOS/RTOS_interface.h"
#include "../MCAL/RCC/RCC_interface.h"
#include "../HAL/LCD/C_LCD_interface.h"
#include "../HAL/Switch/Switch_interface.h"
#include "../HAL/USART_Handler/USRTH_interface.h"
#include "../Other_Drivers/LED/LED_interface.h"
#include "../MCAL/DMA/DMA_interface.h"


int
main(int argc, char* argv[])
{
	RCC_Peripheral_Config(RCC_IOPC,PERIPHERAL_ON);
	RCC_Peripheral_Config(RCC_IOPA,PERIPHERAL_ON);
	RCC_Peripheral_Config(RCC_IOPB,PERIPHERAL_ON);
	RCC_u8SwitchSystemClock(HSE);
	USRTH_voidInit(USART1);
	switch_init();
	CLCD_init();
	DMA_voidInit();
	USRTH_DMAEnable(USART1);
	RTOS_init();
	RTOS_start();
	while(1);

}

