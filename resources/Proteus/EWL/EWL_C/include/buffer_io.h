/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:37 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_BUFFER_IO_H
#define _EWL_BUFFER_IO_H

#include <ansi_parms.h>

#include <cstdio>

_EWL_BEGIN_EXTERN_C

	enum {
		__align_buffer,
		__dont_align_buffer
	};

	int	 _EWL_CDECL __load_buffer (__std(FILE) *  , __std(size_t) * bytes_loaded, int alignment) _EWL_CANT_THROW;

	void _EWL_CDECL __prep_buffer (__std(FILE) *  ) _EWL_CANT_THROW;
	int	 _EWL_CDECL __flush_buffer(__std(FILE) *  , __std(size_t) * bytes_flushed) _EWL_CANT_THROW;

_EWL_END_EXTERN_C

#endif /* _EWL_BUFFER_IO_H */
