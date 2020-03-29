/******************************************************/
/*Author   : Islam A. El-Bahnasawy, Mohammed Anwar    */
/*Date     : 28 Mar 2020                              */
/*Version  : V.01                                     */
/******************************************************/


#include "../../LIB/STD_TYPES.h"

#include "RCC_H_interface.h"
#include "../../MCAL/RCC/RCC_interface.h"

#define PLL_8MHZ	8U
#define PLL_12MHZ	12U
#define PLL_16MHZ	16U
#define PLL_20MHZ	20U
#define PLL_24MHZ	24U
#define PLL_28MHZ   28U
#define PLL_32MHZ   32U
#define PLL_36MHZ   36U
#define PLL_40MHZ   40U
#define PLL_44MHZ   44U
#define PLL_48MHZ   48U
#define PLL_52MHZ   52U
#define PLL_56MHZ   56U
#define PLL_60MHZ   60U
#define PLL_64MHZ   64U
#define PLL_72MHZ   72U

u8 RCCH_u8SwitchSysClkToPLL(u8 PLL_freq)
{
	u8 local_status = OK;
	u8 currentSysCLK;
	
	/*	Get the current System Clock. 
	if it's PLL, turn HSI on, then switch system clock to HSI.*/
	
	if(RCC_u8ReadSystemClock(currentSysCLK) == OK)
	{
		if(currentSysCLK == SWS_PLL)
		{
			if(RCC_u8ChangeClockStatus(HSI_CLK, CLK_STATUS_ON) == OK)
			{
				RCC_u8SwitchSystemClock(HSI);
			}
			else
			{
				local_status = NOT_OK;
			}
		}
		/* turn PLL off, to be able to configure it with the required frequency */
		
		if(RCC_u8ChangeClockStatus(PLL_CLK, CLK_STATUS_OFF) == OK)
		{
			switch(PLL_freq)
			{
				case PLL_8MHZ:
					RCC_PLL_Config(PLL_input_clock_x2, PLL_CLOCK_HSE_2);
					/*turn PLL clock ON*/
					local_status = RCC_u8ChangeClockStatus(PLL_CLK, CLK_STATUS_ON);
					break;
				case PLL_12MHZ:
					RCC_PLL_Config(PLL_input_clock_x3, PLL_CLOCK_HSE_2);
					local_status = RCC_u8ChangeClockStatus(PLL_CLK, CLK_STATUS_ON);
					break;
				case PLL_16MHZ:
					RCC_PLL_Config(PLL_input_clock_x4, PLL_CLOCK_HSE_2);
					local_status = RCC_u8ChangeClockStatus(PLL_CLK, CLK_STATUS_ON);
					break;
				case PLL_20MHZ:
					RCC_PLL_Config(PLL_input_clock_x5, PLL_CLOCK_HSE_2);
					local_status = RCC_u8ChangeClockStatus(PLL_CLK, CLK_STATUS_ON);
					break;
				case PLL_24MHZ:
					RCC_PLL_Config(PLL_input_clock_x6, PLL_CLOCK_HSE_2);
					local_status = RCC_u8ChangeClockStatus(PLL_CLK, CLK_STATUS_ON);
					break;
				case PLL_28MHZ:
					RCC_PLL_Config(PLL_input_clock_x7, PLL_CLOCK_HSE_2);
					local_status = RCC_u8ChangeClockStatus(PLL_CLK, CLK_STATUS_ON);
					break;
				case PLL_32MHZ:
					RCC_PLL_Config(PLL_input_clock_x8, PLL_CLOCK_HSE_2);
					local_status = RCC_u8ChangeClockStatus(PLL_CLK, CLK_STATUS_ON);
					break;
				case PLL_36MHZ:
					RCC_PLL_Config(PLL_input_clock_x9, PLL_CLOCK_HSE_2);
					local_status = RCC_u8ChangeClockStatus(PLL_CLK, CLK_STATUS_ON);
					break;
				case PLL_40MHZ:
					RCC_PLL_Config(PLL_input_clock_x10, PLL_CLOCK_HSE_2);
					local_status = RCC_u8ChangeClockStatus(PLL_CLK, CLK_STATUS_ON);
					break;
				case PLL_44MHZ:
					RCC_PLL_Config(PLL_input_clock_x11, PLL_CLOCK_HSE_2);
					local_status = RCC_u8ChangeClockStatus(PLL_CLK, CLK_STATUS_ON);
					break;
				case PLL_48MHZ:
					RCC_PLL_Config(PLL_input_clock_x12, PLL_CLOCK_HSE_2);
					local_status = RCC_u8ChangeClockStatus(PLL_CLK, CLK_STATUS_ON);
					break;
				case PLL_52MHZ:
					RCC_PLL_Config(PLL_input_clock_x13, PLL_CLOCK_HSE_2);
					local_status = RCC_u8ChangeClockStatus(PLL_CLK, CLK_STATUS_ON);
					break;
				case PLL_56MHZ:
					RCC_PLL_Config(PLL_input_clock_x14, PLL_CLOCK_HSE_2);
					local_status = RCC_u8ChangeClockStatus(PLL_CLK, CLK_STATUS_ON);
					break;				
				case PLL_60MHZ:
					RCC_PLL_Config(PLL_input_clock_x15, PLL_CLOCK_HSE_2);
					local_status = RCC_u8ChangeClockStatus(PLL_CLK, CLK_STATUS_ON);
					break;				
				case PLL_64MHZ:
					RCC_PLL_Config(PLL_input_clock_x16, PLL_CLOCK_HSE_2);
					local_status = RCC_u8ChangeClockStatus(PLL_CLK, CLK_STATUS_ON);
					break;				
				case PLL_72MHZ:
					RCC_PLL_Config(PLL_input_clock_x9, PLL_CLOCK_HSE);
					local_status = RCC_u8ChangeClockStatus(PLL_CLK, CLK_STATUS_ON);
					break;
				default:
					local_status = NOT_OK;
					break;
	
			}
		}
		else
		{
			local_status = NOT_OK;
		}
	}
	else
	{
		local_status = NOT_OK;
	}

	return local_status;
	
}