/****************************************************************************/
/*


DESCRIPTION

	C MSL interface to the support functions provided by MetroTRK.
	This interface is designed to be built as a separate library
	that can be linked with a target program.  The target program
	can make calls into this interface to obtain support services
	from MetroTRK.  The interface is designed to be used directly
	by MSL, without requiring any #includes from the MetroTRK
	sources.

	NOTE: MetroTRK must be running on the target system in order for
	these routines to function.

	Public functions:

		__read_console() - Read a specified number of characters
			from the console into a buffer.

		__write_console() - Read a specified number of characters
			from a buffer to the console.


		Function parameters:

			handle (__file_handle): Specifies the console or
				disk file on which to operate.  Disk file
				handles are assigned dynamically.  Console
				file handles are:

					stdin:	0
					stdout: 1
					stderr:	2

			buffer (unsigned char *): For writes, points to
				the data which is to be written.  For reads,
				points to storage for the data which will
				be read.

			count (size_t *): The desired number of characters
				to read/write.  The value after calling
				a particular function indicates the actual
				number of characters read/written.

		Return value:  All of the public functions here return
			a value of type 'enum __io_results' as defined
			in stdio.h in the Metrowerks MSL.

	(c) 1998 Metrowerks Corporation
	(c) 2006 Freescale Semiconductors inc
	All rights reserved.

*/
/****************************************************************************/
#include <ansi_parms.h>

#include <console_io.h>

typedef int             bool;
typedef unsigned char	ui8;
typedef unsigned short	ui16;
typedef unsigned long	ui32;

static const bool false = 0;
static const bool true  = 1;

static bool inited = false;

static void init( void )
{
    if (inited) return;
    inited = true;

    /* ADD SCI INIT SEQUENCE HERE */

}

/****************************************************************************/
/*
 *	__read_console
 *
 *	Read the specified amount of data from the console file specified by
 *	handle into the given buffer.
 */
/****************************************************************************/
int __read_console( __file_handle handle, unsigned char * buffer,
					size_t * count)
{
	return 0;
}


/****************************************************************************/
/*
 *	__write_console
 *
 *	Write the specified amount of data to the console file specified by
 *	handle from the given buffer.
 */
/****************************************************************************/
int __write_console( __file_handle handle, unsigned char * buffer,
					 size_t * count )
{
	return 0;
}

/****************************************************************************/
/*
 *	__close_console
 *
 *	Close the console file specified by handle from the given buffer.
 */
/****************************************************************************/
int __close_console( __file_handle handle )
{
	return 0;
}

