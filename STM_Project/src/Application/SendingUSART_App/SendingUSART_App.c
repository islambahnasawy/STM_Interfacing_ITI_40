#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/USART/USART_MYTYPES.h"

#include "../../RTOS/RTOS_interface.h"
#include "../../HAL/USART_Handler/USRTH_interface.h"
#include "../Switch_incCounter_APP/Switch_incCounter_APP.h"

static void SendingTask(void);

task_t TransTask = {.handler=SendingTask,.preodicityMS=80};

static void SendingTask(void)
{
	static u8 TxData[4];
	/*Forming the frame that will be transmitted*/
	TxData[0] = TransValue ;
	TxData[1] = TransValue>>8 ;
	TxData[2] = TransValue>>16 ;
	TxData[3] = TransValue>>24 ;

	/*transmitting the frame*/
	USRTH_u8SendingReq(USART1,TxData,4,0);
}
