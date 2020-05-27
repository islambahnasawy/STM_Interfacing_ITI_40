/*
 * LIN_interface.h
 *
 *  Created on: May 5, 2020
 *      Author: anwar
 */

#ifndef MCAL_LIN_LIN_INTERFACE_H_
#define MCAL_LIN_LIN_INTERFACE_H_


extern const  task_t Master_task;
extern const task_t Slave_task;

void LIN_init(USART_t* USART);
void LIN_masterInit(void);
u8 getSDU(u8 ID,u8*data);
u8 setSDU(u8 ID,u8*data);

#endif /* MCAL_LIN_LIN_INTERFACE_H_ */
