/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:35 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		fflush
 */

#include <ansi_parms.h>

#include <ansi_files.h>
#include <buffer_io.h>
#include <file_io.h>

#include <ewl_misra_types.h>

int_t _EWL_CDECL fflush(FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	int32_t	position;

	if (!file) {
		MISRA_EXCEPTION_RULE_14_7()
		return(__flush_all());
	}
	if (file->state.error || (file->mode.file_kind == (uint_t)__closed_file)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	if (file->mode.io_mode == (uint_t)__read_mode) {
		MISRA_EXCEPTION_RULE_14_7()
		return 0;
	}

	if (file->state.io_state >= (uchar_t)__rereading) {
		file->state.io_state = (uchar_t)__reading;
	}

	if (file->state.io_state == (uchar_t)__reading) {
		file->buffer_len = 0U;
	}

	if (file->state.io_state != (uchar_t)__writing) {
		file->state.io_state = (uchar_t)__neutral;
		MISRA_EXCEPTION_RULE_14_7()
		return(0);
	}

#if _EWL_OS_DISK_FILE_SUPPORT
	if (file->mode.file_kind != (uchar_t)__disk_file) {
		position = 0;
	} else {
		position = ftell(file);
		if (position < 0) {
			position = 0;
		}
	}
#else
	position = 0;
#endif

	if (__flush_buffer(_file, NULL)) {
		set_error(file);
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	file->state.io_state = (uchar_t)__neutral;
	file->position       = (uint_t)position;
	file->buffer_len     = 0U;

	return(0);
}
