/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:33 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		ungetwc
 */

#pragma ANSI_strict off

#include <ansi_parms.h>

#if _EWL_WIDE_CHAR

#pragma ANSI_strict reset

#include <ewl_misra_types.h>
MISRA_EXCEPTION_RULE_20_9()
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <ansi_files.h>
#include <wchar.h>
#include <critical_regions.h>

wchar_t _EWL_CDECL __ungetwc(wchar_t c, FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	uchar_t state = file->state.io_state;

	if (fwide(_file, 1) <= 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return(WEOF);
	}

	if ((state == (uchar_t)__writing) || (state == ((uchar_t)__rereading + ((uchar_t)__ungetc_buffer_size - 1U))) || (c == WEOF)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(WEOF);
	}

	if (state < (uchar_t)__rereading) {
		file->saved_buffer_len = file->buffer_len;
		file->buffer_len       = 0U;
		file->state.io_state = (state = (uchar_t)__rereading);
	} else {
		state = ++file->state.io_state;
	}

	file->ungetwc_buffer[state - (uchar_t)__rereading] = c;
	file->state.eof = 0U;

	return c;
}

wchar_t _EWL_CDECL ungetwc(wchar_t c, FILE * file)
{
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (file == stdin) {
		crtrgn = stdin_access;
	} else {
		crtrgn = files_access;
	}

  	__begin_critical_region(crtrgn);
	c = __ungetwc(c, file);
  	__end_critical_region(crtrgn);

	return(c);
}


#endif /* _EWL_WIDE_CHAR */
