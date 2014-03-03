/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:44:21 $
 * $Revision: 1.1 $
 */

#ifndef UART_H
#define UART_H

#if !_EWL_CONSOLE_SUPPORT
#error _EWL_CONSOLE_SUPPORT must not be defined to 1.
#endif

typedef int UARTError;

enum {
	kUARTNoError = 0,
	kUARTUnknownBaudRate,
	kUARTConfigurationError,
	kUARTBufferOverflow,				/* specified buffer was too small */
	kUARTNoData							/* no data available from polling */
};

typedef enum {
	kBaudHWSet = -1,					/* use HW settings such as DIP switches */
	kBaud300 = 300,						/* valid baud rates */
	kBaud600 = 600,
	kBaud1200 = 1200,
	kBaud1800 = 1800,
	kBaud2000 = 2000,
	kBaud2400 = 2400,
	kBaud3600 = 3600,
	kBaud4800 = 4800,
	kBaud7200 = 7200,
	kBaud9600 = 9600,
	kBaud19200 = 19200,
	kBaud38400 = 38400,
	kBaud57600 = 57600,
	kBaud115200 = 115200,
	kBaud230400 = 230400
} UARTBaudRate;

UARTError InitializeUART(UARTBaudRate baudRate);
UARTError TerminateUART(void);

UARTError ReadUARTPoll(char* c);

UARTError ReadUART1(char* c);
UARTError ReadUARTN(void* bytes, unsigned long length);
UARTError ReadUARTString(char* string, unsigned long limit, char terminateChar);

UARTError WriteUART1(char c);
UARTError WriteUARTN(const void* bytes, unsigned long length);
UARTError WriteUARTString(const char* string);

#endif /* UART_H */
