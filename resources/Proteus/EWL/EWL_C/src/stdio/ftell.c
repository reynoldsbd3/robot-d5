/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:35 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		ftell
 */

#include <ansi_parms.h>

#include <errno.h>
#include <buffer_io.h>
#include <critical_regions.h>

#include <ewl_misra_types.h>

MISRA_EXCEPTION_RULE_16_2()

fpos_t _EWL_CDECL _ftell(FILE * _EWL_RESTRICT _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	uchar_t charsInUndoBuffer = 0U;
	fpos_t	position;

	if ( !(((__file_kinds)file->mode.file_kind == __disk_file) || ((__file_kinds)file->mode.file_kind == __console_file)) || file->state.error) {
		MISRA_EXCEPTION_RULE_20_5()
		errno = EFPOS;
		position = (fpos_t)-1;
		MISRA_EXCEPTION_RULE_14_7()
		return position;
	}

	if (file->state.io_state == (uint_t)__neutral) {
		MISRA_EXCEPTION_RULE_14_7()
		return(file->position);
	}

	MISRA_EXCEPTION_RULE_17_3()
	position = file->buffer_pos + (file->buffer_ptr - file->buffer);

	if (file->state.io_state >= (uint_t)__rereading) {
		charsInUndoBuffer = (uchar_t)((file->state.io_state - (uchar_t)__rereading) + 1U);
		position -= charsInUndoBuffer;
	}

	return(position);

	MISRA_EXCEPTION_RULE_16_7()
}

int32_t _EWL_CDECL ftell(FILE * file)
{
	int32_t retval;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (file == stdin) {
		crtrgn = stdin_access;
	} else if (file == stdout) {
		crtrgn = stdout_access;
	} else if (file == stderr) {
		crtrgn = stderr_access;
	} else {
		crtrgn = files_access;
	}

  	__begin_critical_region(crtrgn);
	retval = (int32_t)_ftell(file);
  	__end_critical_region(crtrgn);

	return(retval);
}

int_t _EWL_CDECL fgetpos(FILE * _EWL_RESTRICT file, fpos_t * _EWL_RESTRICT pos)
{
	int_t result;
	MISRA_QUIET_UNUSED_SYMBOL()
	critical_regions crtrgn;

	if (file == stdin) {
		crtrgn = stdin_access;
	} else if (file == stdout) {
		crtrgn = stdout_access;
	} else if (file == stderr) {
		crtrgn = stderr_access;
	} else {
		crtrgn = files_access;
	}
  	__begin_critical_region(crtrgn);

	*pos = _ftell(file);

  	__end_critical_region(crtrgn);

	result = (*pos == (fpos_t)-1);
	return result;
}


