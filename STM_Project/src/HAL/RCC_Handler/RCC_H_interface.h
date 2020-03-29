/******************************************************/
/*Author   : Islam A. El-Bahnasawy, Mohammed Anwar    */
/*Date     : 28 Mar 2020                              */
/*Version  : V.01                                     */
/******************************************************/

#ifndef RCC_H_INTERFACE_H_
#define RCC_H_INTERFACE_H_


/******************************************************************************************
* function name: RCCH_u8SwitchSysClkToPLL
*
* parameters:  Input:
*                PLL_freq
*                     type: u8
*                     Description: this variable carries the desired PLL clock freq in MHZ
* 					  Options:
*								PLL_8MHZ,
*								PLL_12MHZ,
*                               PLL_16MHZ,
*                               PLL_20MHZ,
*                               PLL_24MHZ,
*                               PLL_28MHZ,
*                               PLL_32MHZ,
*                               PLL_36MHZ,
*                               PLL_40MHZ,
*                               PLL_44MHZ,
*                               PLL_48MHZ,
*                               PLL_52MHZ,
*                               PLL_56MHZ,
*                               PLL_60MHZ,
*                               PLL_64MHZ,
*                               PLL_72MHZ
*
*              Output: NA
*              In/out: NA
*
* return: OK , NOT_OK
* Description: This function is used to switch the system clock to PLL according to a specific 
*			   frequency given by user within frequency range: [8 MHZ to 72 MHZ] as shown above
*			   in "PLL_freq" oprtions.
********************************************************************************************/

u8 RCCH_u8SwitchSysClkToPLL(u8 PLL_freq);

#endif