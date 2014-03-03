/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:44:21 $
 * $Revision: 1.1 $
 */
 
/*
 *	uart_console_config.h
 */

#ifndef __uart_console_config__
#define __uart_console_config__

#include "sys/UART.h"

#if defined(__COLDFIRE__)
	#include <coldfire/uart_console_config.h>		 /* ColdFire */
#elif defined(__HC12__)
	#include <hc/uart_console_config.h>				   /* HC12 */
#elif defined(__PPC_EABI__)
	#include <pa/uart_console_config.h>	/* Embedded Power Architecture */
#elif defined(__m56800E__) || defined(__m56800__)
    #include <dsp/uart_console_config.h>			/* Hawk DSP */
#elif defined (__arm) || defined(__arm__)
	#include <arm/uart_console_config.h>			/* ARM */
#elif defined (__STARCORE__)
	#include <sc/uart_console_config.h>		/* StarCore */
#else
	/* Unknown platform! */
	#error ewlGlobals.h could not include prefix file
#endif

#ifdef __GCN_1_3_COMPATIBILITY__
#define __GCN_1_3_COMPATIBILITY__ 0
#endif

#endif
