/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/07/12 22:31:14 $
 * $Revision: 1.2 $
 */

/*
 *	Routines
 *	--------
 *		fputc
 *		putc
 *		putchar
 *
 */

#include <ansi_parms.h>

#include <ansi_files.h>
#include <buffer_io.h>
#include <char_io.h>
#if _EWL_OS_DISK_FILE_SUPPORT
#include <misc_io.h>
#endif /* _EWL_OS_DISK_FILE_SUPPORT */

#include <ewl_misra_types.h>

int_t _EWL_CDECL __put_char(int_t c, FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	__file_kinds   kind = (__file_kinds)file->mode.file_kind;

	file->buffer_len = 0U;

	if (file->state.error || (kind == __closed_file)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

#if _EWL_OS_DISK_FILE_SUPPORT
	if (kind == __console_file) {
		__stdio_atexit();
	}
#endif

	if (file->state.io_state == (uchar_t)__neutral) {
		if (file->mode.io_mode & (uchar_t)__write_mode) {
#if _EWL_OS_DISK_FILE_SUPPORT
			if (file->mode.io_mode & (uchar_t)__append_mode) {
				if (_fseek(file, 0u, SEEK_END)) {
					MISRA_EXCEPTION_RULE_14_7()
					return(0);
				}
			}
#endif

			file->state.io_state = (uchar_t)__writing;
			__prep_buffer(_file);
		}
	}

	if (file->state.io_state != (uchar_t)__writing) {
		set_error(file);
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}

	MISRA_EXCEPTION_RULE_17_3()
	if ((file->mode.buffer_mode == (uint_t)_IOFBF) || ((file->buffer_ptr - file->buffer) == (int_t)file->buffer_size)) {
		if (__flush_buffer(_file, NULL)) {
			set_error(file);
			MISRA_EXCEPTION_RULE_14_7()
			return(EOF);
		}
	}

	file->buffer_len--;
	*file->buffer_ptr++ = (uchar_t)c;

	if (file->mode.buffer_mode != (uint_t)_IOFBF) {
		if ((file->mode.buffer_mode == (uint_t)_IONBF) || (c == '\n')) {
			if (__flush_buffer(_file, NULL)) {
				set_error(file);
				MISRA_EXCEPTION_RULE_14_7()
				return(EOF);
			}
		}

		file->buffer_len = 0U;
	}

	return c;
}
