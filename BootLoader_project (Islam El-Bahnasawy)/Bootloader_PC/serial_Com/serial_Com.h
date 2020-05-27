#ifndef _SERIAL_COM_H_
#define _SERIAL_COM_H_


void serialCom_init(void);
void serialCom_sendData(u8* data, u16 length);
void serialCom_receiveData(u8* data, u16 length);

#endif

				   