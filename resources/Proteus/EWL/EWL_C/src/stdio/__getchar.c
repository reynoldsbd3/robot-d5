/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:34 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		__get_char
 */

#include <ansi_parms.h>

#include <ansi_files.h>
#include <buffer_io.h>

#include <ewl_misra_types.h>

int_t _EWL_CDECL __get_char(FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	__io_states	state;
	__io_results ioresult;

	file->buffer_len = 0U;

	if (file->state.error || ((__file_kinds)file->mode.file_kind == __closed_file)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	state = (__io_states)file->state.io_state;

	if ((state == __writing) || !(file->mode.io_mode & (uchar_t)__read_mode)) {
		set_error(file);
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	if (state >= __rereading) {
		file->state.io_state--;

		if (state == __rereading) {
			file->buffer_len = file->saved_buffer_len;
		}

		MISRA_EXCEPTION_RULE_14_7()
		return (int_t)(file->ungetc_buffer[(int_t)state - (int_t)__rereading]);

	} else {
		file->state.io_state = (uchar_t)__reading;
	}

	ioresult = (__io_results)__load_buffer(_file, NULL, __align_buffer);

	if (ioresult || !file->buffer_len) {
		if (ioresult == __io_error) {
			set_error(file);
		} else {
			set_eof(file);
		}

		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	file->buffer_len--;

	return (int_t)(*file->buffer_ptr++);
}

