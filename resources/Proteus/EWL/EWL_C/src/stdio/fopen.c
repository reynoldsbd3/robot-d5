/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/07/12 22:31:14 $
 * $Revision: 1.2 $
 */

/*
 *	Routines
 *	--------
 *		fopen
 *		freopen
 *
 *		__get_file_modes
 */

#include <ansi_parms.h>

#include <file_io.h>
#include <ansi_files.h>
#include <critical_regions.h>

#if _EWL_OS_DISK_FILE_SUPPORT
#include <stdlib.h>
#endif /* _EWL_OS_DISK_FILE_SUPPORT */

#include <ewl_misra_types.h>

#if _EWL_OS_DISK_FILE_SUPPORT

int_t _EWL_CDECL __get_file_modes(const char_t * mode, __file_modes * modes)
{
	const char_t *	mode_ptr = mode;
	uint32_t		mode_str;
	uchar_t			open_mode, io_mode;

	modes->file_kind = (uchar_t)__disk_file;
#if _EWL_WIDE_CHAR
	modes->file_orientation = (uchar_t)__unoriented;
#endif /* _EWL_WIDE_CHAR */
	modes->binary_io = 0u;

	mode_str = (uint32_t)*mode_ptr;
	++mode_ptr;
	switch (mode_str) {
		case 'r':
			open_mode = (uchar_t)__must_exist;
			break;

		case 'w':
			open_mode = (uchar_t)__create_or_truncate;
			break;

		case 'a':
			open_mode = (uchar_t)__create_if_necessary;
			break;

		default:
			return(0);
	}

	modes->open_mode = open_mode;
	switch (*mode_ptr++) {
		case 'b':
			modes->binary_io = 1u;
			if (*mode_ptr == '+') {
				mode_str = (mode_str << 8) | (uint32_t)'+';
			}
			break;

		case '+':
			mode_str = (mode_str << 8) | (uint32_t)'+';
			if (*mode_ptr == 'b') {
				modes->binary_io = 1u;
			}
			break;

		default: break;
	}

	switch (mode_str) {
		case 'r':
			io_mode = (uchar_t)__read_mode;
			break;

		case 'w':
			io_mode = (uchar_t)__write_mode;
			break;

		case 'a':
			io_mode = (uchar_t)__write_mode | (uchar_t)__append_mode;
			break;

		case 'r+':
			io_mode = (uchar_t)__read_write_mode;
			break;

		case 'w+':
			io_mode = (uchar_t)__read_write_mode;
			break;

		case 'a+':
			io_mode = (uchar_t)__read_write_mode | (uchar_t)__append_mode;
			break;

		default:
			return(0);
	}

	modes->io_mode = io_mode;

	return(1);
}

FILE * _EWL_CDECL freopen(const char_t * _EWL_RESTRICT name, const char_t * _EWL_RESTRICT mode, FILE * _EWL_RESTRICT file)
{
	__file_modes modes;

	__stdio_atexit();
	if (!file) {
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

	(void)fclose(file);
	clearerr(file);

	if (!__get_file_modes(mode, &modes)) {
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}

	__init_file(file, modes, 0, (size_t)BUFSIZ);

	if (__open_file(name, modes, &file->handle)) {
		file->mode.file_kind = (uchar_t)__closed_file;
	    if (file->state.free_buffer) {
			MISRA_EXCEPTION_RULE_20_4()
	    	free(file->buffer);
	    }
		MISRA_EXCEPTION_RULE_14_7()
		return(NULL);
	}
	if (modes.io_mode & (uchar_t)__append_mode) {
		(void)fseek(file, 0, SEEK_END);
	}

	return(file);
}


FILE * _EWL_CDECL fopen(const char_t * _EWL_RESTRICT name, const char_t * _EWL_RESTRICT mode)
{
	FILE * file;

	__begin_critical_region(files_access);
	file = __find_unopened_file();
	__end_critical_region(files_access);

	return(freopen(name, mode, file));
}

#endif /* _EWL_OS_DISK_FILE_SUPPORT */


