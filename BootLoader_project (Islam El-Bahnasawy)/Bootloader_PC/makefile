fileParser 	= file_parser.c
BL_Protocol = BL_Protocol/BL_Protocol.c
serial_Com 	= serial_Com/serial_Com.c
Delay 		= LIB/DELAY_program.c

allSrc = $(fileParser) $(BL_Protocol) $(serial_Com) $(Delay)
FinalTargetName = FLASH_Application.exe
CC = gcc



$(FinalTargetName): CFLAG = 
$(allSrc): CFLAG =


$(FinalTargetName): $(allSrc)
	$(CC) $(CFLAG) $(allSrc) -o $(FinalTargetName)
#here $(CFLAG) has no effect
