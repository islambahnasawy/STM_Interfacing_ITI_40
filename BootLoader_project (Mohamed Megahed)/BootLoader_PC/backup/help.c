#include<windows.h>
#include<stdio.h>
#include "PCcom_cfg.h"



int main()
{
  HANDLE hComm;
	int Status;
  hComm = CreateFile("COM4",                //port name
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
	
	char lpBuffer[] = "trashc";
	DWORD dNoOFBytestoWrite;         // No of bytes to write into the port
	DWORD dNoOfBytesWritten = 0;     // No of bytes written to the port
	dNoOFBytestoWrite = sizeof(lpBuffer);
	
	/*Status = WriteFile(hComm,        // Handle to the Serial port
                   lpBuffer,     // Data to be written to the port
                   dNoOFBytestoWrite,  //No of bytes to write
                   &dNoOfBytesWritten, //Bytes written
                   NULL);
				   */
				   
				   
	
	Status = SetCommMask(hComm, EV_RXCHAR);
	
	DWORD dwEventMask; 
	//Status = WaitCommEvent(hComm, &dwEventMask, NULL); 

	char TempChar; //Temporary character used for reading
	char SerialBuffer[256];//Buffer for storing Rxed Data
	//int myNum=5;
	DWORD NoBytesRead;
	int i = 0;
	
	do
	{
	ReadFile( hComm,           //Handle of the Serial port
				&TempChar,       //Temporary character
				sizeof(TempChar),//Size of TempChar
				&NoBytesRead,    //Number of bytes read
				NULL);
	
	SerialBuffer[i] = TempChar;// Store Tempchar into buffer
	//printf("data received: \n");
	
	printf("%c", TempChar);
	//myNum--;
	i++;
	//printf("NUmber of bytes: %d\n", NoBytesRead);
	}
	
	while (NoBytesRead>0);


	CloseHandle(hComm);//Closing the Serial Port

	printf("\nHamada wsal");
  return 0;
}