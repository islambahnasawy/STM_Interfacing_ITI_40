#include "../LIB/STD_TYPES.h"
#include "../MCAL/USART/USART_MYTYPES.h"

#include "../MCAL/RCC/RCC_interface.h"
#include "../HAL/LCD/C_LCD_interface.h"
#include "../RTOS/RTOS_interface.h"
#include "../HAL/Switch/Switch_interface.h"
#include "../HAL/USART_Handler/USRTH_interface.h"




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
	RTOS_init();
	RTOS_start();
	while(1);

}

