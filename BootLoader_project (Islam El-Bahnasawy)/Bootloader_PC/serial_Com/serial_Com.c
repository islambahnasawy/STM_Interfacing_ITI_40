#include <windows.h>
#include <stdio.h>
#include "../LIB/STD_TYPES.h"

#include "serial_Com.h"

HANDLE hComm;

void serialCom_init(void)
{
	int Status;
  hComm = CreateFile("COM8",                //port name
                      GENERIC_READ | GENERIC_WRITE, //Read/Write
                      0,                            // No Sharing
                      NULL,                         // No Security
                      OPEN_EXISTING,// Open existing port only
                      0,            // Non Overlapped I/O
                      NULL);        // Null for Comm Devices
					   

  if (hComm == INVALID_HANDLE_VALUE)
      printf("Error in opening serial port\n");
  else
      printf("opening serial port successful\n");
  
  
	DCB dcbSerialParams = { 0 }; // Initializing DCB structure
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	Status = GetCommState(hComm, &dcbSerialParams);


	dcbSerialParams.BaudRate = CBR_9600;  // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;         // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;// Setting StopBits = 1
	dcbSerialParams.Parity   = NOPARITY;  // Setting Parity = None
	
	/*setting timeout*/
	
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout         = 50; // in milliseconds
	timeouts.ReadTotalTimeoutConstant    = 50; // in milliseconds
	timeouts.ReadTotalTimeoutMultiplier  = 10; // in milliseconds
	timeouts.WriteTotalTimeoutConstant   = 50; // in milliseconds
	timeouts.WriteTotalTimeoutMultiplier = 10; // in milliseconds
	
	SetCommState(hComm, &dcbSerialParams);	
}

void serialCom_sendData(u8* data, u16 length)
{
	u8 status = OK;
	status = SetCommMask(hComm, 0);
	DWORD dNoOFBytestoWrite = length;         // No of bytes to write into the port
	DWORD dNoOfBytesWritten = 0;     // No of bytes written to the port
	
	status = WriteFile(hComm,        // Handle to the Serial port
                   data,     // Data to be written to the port
                   dNoOFBytestoWrite,  //No of bytes to write
                   &dNoOfBytesWritten, //Bytes written
                   NULL);
   
   status = SetCommMask(hComm, EV_RXCHAR);

}

void serialCom_receiveData(u8* data, u16 length)
{
	int Status;
	
	//printf("receiving data : \n");
	Status = SetCommMask(hComm, EV_RXCHAR);
	DWORD dwEventMask; 
	Status = WaitCommEvent(hComm, &dwEventMask, NULL); 

	u8 TempByte; //Temporary character used for reading
	//char SerialBuffer[256];//Buffer for storing Rxed Data
	//int myNum=5;
	DWORD NoBytesRead;
	u16 frame_length = length;
	u16 i =0;
	
	do
	{
	ReadFile( hComm,           //Handle of the Serial port
				&TempByte,       //Temporary character
				sizeof(TempByte),//Size of TempChar
				&NoBytesRead,    //Number of bytes read
				NULL);
	
	data[i] = TempByte;// Store Tempchar into buffer
	//printf("data received: \n");
	
	printf("%x ", TempByte);
	//myNum--;
	frame_length--;
	i++;
	//printf("NUmber of bytes: %d\n", NoBytesRead);
	}
	
	while (frame_length>0);


	//CloseHandle(hComm);//Closing the Serial Port

	printf("\nHamada wsal");
}



				   