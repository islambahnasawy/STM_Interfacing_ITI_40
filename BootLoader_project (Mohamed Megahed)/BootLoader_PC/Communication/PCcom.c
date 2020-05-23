/********************************************************/
/*Author : Mohamed Megahed 								*/
/*Date   : 16 May 2020									*/
/*Version : V01											*/
/********************************************************/

#include<windows.h>
#include<stdio.h>

#include "../LIB/STD_TYPES.h"

#include "PCcom.h"
#include "PCcom_cfg.h"


#define OK		1
#define NOT_OK	0

HANDLE hComm;
u32 Status;

u8 serial_Init(void)
{
	u8 local_status = OK;
	hComm = CreateFile("COM4",                //port name
						GENERIC_READ | GENERIC_WRITE, //Read/Write
						0,                            // No Sharing
						NULL,                         // No Security
						OPEN_EXISTING,// Open existing port only
						0,            // Non Overlapped I/O
						NULL);
	if (hComm != INVALID_HANDLE_VALUE)
	{
	DCB dcbSerialParams = { 0 }; // Initializing DCB structure
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	Status = GetCommState(hComm, &dcbSerialParams);


	dcbSerialParams.BaudRate = BAUD_RATE;  // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = BYTESIZE;         // Setting ByteSize = 8
	dcbSerialParams.StopBits = STOPBITS;// Setting StopBits = 1
	dcbSerialParams.Parity   = PARITY;  // Setting Parity = None
	
	/*setting timeout*/
	
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout         = 50; // in milliseconds
	timeouts.ReadTotalTimeoutConstant    = 50; // in milliseconds
	timeouts.ReadTotalTimeoutMultiplier  = 10; // in milliseconds
	timeouts.WriteTotalTimeoutConstant   = 50; // in milliseconds
	timeouts.WriteTotalTimeoutMultiplier = 10; // in milliseconds
	
	SetCommState(hComm, &dcbSerialParams);
	printf("Init done\n");
	}
	else
	{
		local_status=NOT_OK;
	}
	return local_status;
}
u8 serial_Send(u8* buffer , u16 length)
{  
	//serial_Init();

	Status = SetCommMask(hComm, 0);
  if (hComm == INVALID_HANDLE_VALUE)
  {
     // printf("Error in opening serial port\n");
  }
  else
  {
    // printf("opening serial port successful\n");
  
	DWORD dNoOfBytesWritten = 0;     // No of bytes written to the port

	
	Status = WriteFile(hComm,        // Handle to the Serial port
                   buffer,     // Data to be written to the port
                   length,  //No of bytes to write
                   &dNoOfBytesWritten, //Bytes written
                   NULL);
	
	//CloseHandle(hComm);//Closing the Serial Port
  }
}

u8 serial_Receive(u8*buffer ,u16 length)
{

	DWORD dwEventMask; 
	DWORD NoBytesRead;
	int i = 0;
	u8 a;
	Status = SetCommMask(hComm, EV_RXCHAR);
	Status = WaitCommEvent(hComm, &dwEventMask, NULL); 
	if(Status==EV_RXCHAR)
		{
			Status=0;
			do
			{
				DWORD NoBytesRead;
				ReadFile( hComm,           //Handle of the Serial port
					&a,       //Temporary character
					1,//Size of TempChar
					&NoBytesRead,    //Number of bytes read
					NULL);

				buffer[i]=a;
								i++;
			}
	while (length>i);
		}	
	//CloseHandle(hComm);
}
