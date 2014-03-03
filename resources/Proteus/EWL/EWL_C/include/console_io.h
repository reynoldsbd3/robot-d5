/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:37 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_CONSOLE_IO_H
#define _EWL_CONSOLE_IO_H

#include <ansi_parms.h>
#include <stdio.h>
#include <file_struc.h>

_EWL_BEGIN_EXTERN_C

	#if _EWL_CONSOLE_SUPPORT

		/** these function return one off:
			enum __io_results {
			     __no_io_error,
			     __io_error,
			     __io_EOF
				};
		 */
		int	_EWL_CDECL __read_console	(__std(__file_handle) handle, unsigned char * buffer, __std(size_t) * count);
		int	_EWL_CDECL __write_console	(__std(__file_handle) handle, unsigned char * buffer, __std(size_t) * count);
		int	_EWL_CDECL __close_console	(__std(__file_handle) handle) _EWL_CANT_THROW;

	#endif /* _EWL_CONSOLE_SUPPORT */

_EWL_END_EXTERN_C


#endif  /* _EWL_CONSOLE_IO_H */
