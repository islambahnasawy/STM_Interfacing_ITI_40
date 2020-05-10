#include "../LIB/STD_TYPES.h"
#include "../MCAL/USART/USART_MYTYPES.h"

#include "../RTOS/RTOS_interface.h"
#include "../MCAL/RCC/RCC_interface.h"
#include "../HAL/LCD/C_LCD_interface.h"
#include "../HAL/Switch/Switch_interface.h"
#include "../HAL/USART_Handler/USRTH_interface.h"
#include "../Other_Drivers/LED/LED_interface.h"
#include "../MCAL/DMA/DMA_interface.h"
#include "../HAL/LIN/LIN_interface.h"
#include "../MCAL/Flash/Flash_interface.h"

#include "../MCAL/Flash/Flash_reg.h"

int
main(int argc, char* argv[])
{
	/*RCC_Peripheral_Config(RCC_IOPC,PERIPHERAL_ON);
	RCC_Peripheral_Config(RCC_IOPA,PERIPHERAL_ON);
	RCC_Peripheral_Config(RCC_IOPB,PERIPHERAL_ON);
	RCC_u8SwitchSystemClock(HSE);
	//USRTH_voidInit(USART1);
	switch_init();
	CLCD_init();
	DMA_voidInit();
	LIN_init(USART1);
	USRTH_DMAEnable(USART1);
	LED_Init(LED1);
	LED_Init(LED2);
	LED_setLedState(LED1,LED_STATE_OFF);
	//LED_setLedState(LED2,LED_STATE_OFF);
	LIN_masterInit();

	RTOS_init();
	RTOS_start();*/
	u16 arr[1536]={0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA,0xAAAA};
	u16 arr1[1536] = {0xBBBB,0xBBBB,0xBBBB,0xBBBB,0xBBBB,0xBBBB,0xBBBB,0xBBBB,0xBBBB,0xBBBB,0xBBBB,0xBBBB,0xBBBB,0xBBBB,0xBBBB};
	RCC_u8ChangeClockStatus(HSI_CLK,CLK_STATUS_ON);
	Flash_unlock();
	u8 x = OK;
	x = Flash_programWrite((u16*)0x8001F70,arr , 40);

	x= Flash_programWrite((u16*)0x8001F72,arr1 , 30);

	for(u16 i=0;i<1536;i++)
	{
		arr[i]=0xAAAA;
	}
	for(u16 i=0;i<1534;i++)
	{
		arr1[i]=0xBBBB;
	}


	x = Flash_programWrite((u16*)0x8001C00,arr , 3072);
	if(x==1)
	{
		//trace_printf("OK");
		x++;
	}
	else
	{
		//trace_printf("Not OK");
		x--;
	}
	//Flash_erase(0x08001F00);
	x= Flash_programWrite((u16*)0x8001C06,arr1 , 3068);
	if(x==1)
	{
		//trace_printf("OK");
		x++;
	}
	else
	{
		//trace_printf("Not OK");
		x--;
	}
	//Flash_pageErase(0x08001F00);
	//Flash_massErase();

	Flash_lock();
	LED_Init(LED1);
	//trace_printf("%d",x);
	/*Flash_program(0x08001F00,0x1111);
	//Flash_program(0x08001F02,0x1234);
	Flash_program(0x08001F04,0x5678);
	//Flash_program(0x08001F06,0xABCD);
	Flash_program(0x08001F00,0xef12);
	/*Flash_program(0x08001F72,0xAAAA);
	Flash_program(0x08001F74,0xBBBB);
	Flash_program(0x08001F70,0x1234);*/

	//trace_printf("hello");
	while(1);

}

