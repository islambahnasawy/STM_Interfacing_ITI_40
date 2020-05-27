#include "../LIB/STD_TYPES.h"
#include "../MCAL/USART/USART_MYTYPES.h"

#include "../RTOS/RTOS_interface.h"
#include "../MCAL/RCC/RCC_interface.h"
#include "../HAL/LCD/C_LCD_interface.h"
#include "../HAL/Switch/Switch_interface.h"
#include "../HAL/USART_Handler/USRTH_interface.h"
#include "../Other_Drivers/LED/LED_interface.h"
#include "../MCAL/DMA/DMA_interface.h"
#include "../MCAL/Flash/Flash_interface.h"
#include "../HAL/LIN/LIN_interface.h"
#include "Bootloader_App/BL_APP_interface.h"


typedef void (*jump)(void);

extern volatile u32 marker;
extern volatile u32 entrypoint;

extern u32* ptr_marker;
extern u32* ptr_entrypoint;

int
main(int argc, char* argv[])
{


	//LIN_init(USART1);
	//LIN_masterInit();


	trace_printf("marker value: %x", *ptr_marker);
	trace_printf("entry point value: %x", *ptr_entrypoint);

	/*flash initial value for marker*/
	if(marker == 0xAABBCCDD)
	{

		RCC_Peripheral_Config(RCC_IOPC,PERIPHERAL_ON);
		RCC_Peripheral_Config(RCC_IOPA,PERIPHERAL_ON);
		RCC_Peripheral_Config(RCC_IOPB,PERIPHERAL_ON);
		RCC_u8SwitchSystemClock(HSE);
		USRTH_voidInit(USART1);
		//switch_init();
		//CLCD_init();
		DMA_voidInit();
		USRTH_DMAEnable(USART1);
		Flash_unlock();

		/*No application to run, wait for application*/
		receiveStartFrame();
	}
	else if (marker == 0x44224422)
	{
		/*there is application, jump to entry point */
		trace_printf("Jumping to entry point...\n");
		if(*ptr_entrypoint != 0xDDCCBBAA)
		{
			((jump)(entrypoint))();
		}
	}


	/* check if marker has value that presents no application is flashed,
	 * then wait for application using receiveStartFrame();
	 *
	 * else if marker has value that presents there is a flashed application,
	 * then jump to entry point address
	 * */


	//receiveDataFrame();

	trace_printf("hamada");
	trace_printf("hamada");
	//RTOS_init();
	//RTOS_start();

	while(1);

	return 0;

}




