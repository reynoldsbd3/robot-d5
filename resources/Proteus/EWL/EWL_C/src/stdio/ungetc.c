/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:36 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		ungetc
 *
 */

#include <ansi_parms.h>

#include <buffer_io.h>
#include <critical_regions.h>
#if _EWL_WIDE_CHAR  && _EWL_C99
#include <wchar.h>
#endif

#include <ewl_misra_types.h>

int_t _EWL_CDECL __ungetc(int_t c, FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE *file = (struct _FILE *)_file;
	uchar_t state = file->state.io_state;

#if _EWL_WIDE_CHAR  && _EWL_C99
	if (fwide(_file, -1) >= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
#endif /* _EWL_WIDE_CHAR && _EWL_C99*/

	if ((state == (uchar_t)__writing) || (state == (((uchar_t)__rereading + (uchar_t)__ungetc_buffer_size) - 1U)) || (c == (int_t)EOF)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	if (state < (uchar_t)__rereading) {
		file->saved_buffer_len = file->buffer_len;
		file->buffer_len       = 0U;
		file->state.io_state = (state = (uchar_t)__rereading);
	} else {
		state = ++file->state.io_state;
	}

	file->ungetc_buffer[state - (uchar_t)__rereading] = (uchar_t)c;
	file->state.eof = 0U;

	return c;
}

int_t _EWL_CDECL ungetc(int_t c, FILE * file)
{
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (file == stdin) {
		crtrgn = stdin_access;
	} else {
		crtrgn = files_access;
	}

  	__begin_critical_region(crtrgn);
	c = __ungetc(c, file);
  	__end_critical_region(crtrgn);

	return(c);
}

