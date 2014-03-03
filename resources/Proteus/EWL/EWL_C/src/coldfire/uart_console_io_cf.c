/*
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 */

/*
 *    Routines
 *    --------
 *        __read_console
 *        __write_console
 *        __close_console
 *
 *    Implementation
 *    --------------
 *
 *        These routines implement low-level console IO routines using
 *        any UART library which conforms to the MetroTRK UART library
 *        interface.  They are intended to provide basic console IO
 *        services in an embedded environment which lacks OS console
 *        support.
 */

#include <ansi_parms.h>

#if _EWL_CONSOLE_SUPPORT

#include <ewl_misra_types.h>

#include <console_io.h>
#include <sys/uart_console_config.h>

/************************************************************************/

static UARTError __init_uart_console(void)
{
    UARTError err = kUARTNoError;
    static int_t initialized = 0;

    if (initialized == 0)
    {
        err = InitializeUART( UART_CONSOLE_BAUD_RATE );

        if (err == kUARTNoError)
        {
            initialized = 1;
    	}
    }

    return( err );
}

/************************************************************************/

int_t __read_console(__file_handle handle,
                     uchar_t * buffer,
                     size_t  * count)
{
    #pragma unused(handle)
    MISRA_QUIET_UNUSED_ARGS()

    size_t limit;
    UARTError err;

    if (__init_uart_console() != kUARTNoError) {
        MISRA_EXCEPTION_RULE_14_7()
        return((int_t)__io_error );
    }

    /*
    ** A console read should return when it encounters a line-ending
    ** or after it has read the requested number of characters.
    */

    limit = *count;
    *count = 0u;
    err = kUARTNoError;

    while((*count < limit) && (err == kUARTNoError)) {
        /* Changed this to ReadUARTN and set the count to 1
         * since it is the same functionality as ReadUART1,
         * but ReadUART1 broke end user's implementations
         */
        MISRA_EXCEPTION_RULE_11_4() 
        err = ReadUARTN((char_t *)buffer, 1UL );
        ++(*count);
        if (*buffer == (uchar_t)'\r') {
            break;
        }
        ++buffer;
    }

    return(int_t)( (err == kUARTNoError) ? __no_io_error : __io_error );

	MISRA_EXCEPTION_RULE_16_7()
}

/************************************************************************/

int_t __write_console(__file_handle handle,
                    uchar_t * buffer,
                    size_t * count)
{
    #pragma unused(handle)
    MISRA_QUIET_UNUSED_ARGS()

    if (__init_uart_console() != kUARTNoError)
    {
        MISRA_EXCEPTION_RULE_14_7()
        return((int_t)__io_error );
    }

    if (WriteUARTN( buffer, (uint32_t)*count ) != kUARTNoError)
    {
        *count = 0u;
        MISRA_EXCEPTION_RULE_14_7()        
        return((int_t)__io_error );
    }

    return((int_t)__no_io_error );

	MISRA_EXCEPTION_RULE_16_7()
}

/************************************************************************/

int_t __close_console(__file_handle handle)
{
    #pragma unused(handle)
    MISRA_QUIET_UNUSED_ARGS()

    return((int_t)__no_io_error );
}

#endif /* _EWL_CONSOLE_SUPPORT */
