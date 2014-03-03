/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/07/12 22:31:14 $
 * $Revision: 1.2 $
 */

/*
 *	Routines
 *	--------
 *		fopen_s
 *		freopen_s
 */

#include <ansi_parms.h>

#include <stdlib.h>
#include <ansi_files.h>
#include <file_io.h>
#include <errno.h>
#include <critical_regions.h>

#include <ewl_misra_types.h>

#if _EWL_OS_DISK_FILE_SUPPORT
#include <misc_io.h>

#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__

errno_t _EWL_CDECL freopen_s(FILE * _EWL_RESTRICT * _EWL_RESTRICT newfile, const char_t * _EWL_RESTRICT name, const char_t * _EWL_RESTRICT mode, FILE * _EWL_RESTRICT file)
{
	__file_modes modes;

	if (newfile != NULL) {
		*newfile = NULL;
	}

	if ((newfile == NULL) || (mode == NULL) || (file == NULL)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	__stdio_atexit();

	if (name == NULL) {
		if (!__get_file_modes(mode, &modes)) {
			MISRA_EXCEPTION_RULE_14_7()
			return ERANGE;
		}

		file->mode.binary_io = modes.binary_io;

		*newfile = file;
		MISRA_EXCEPTION_RULE_14_7()
		return ENOERR;
	}

	(void)fclose(file);
	clearerr(file);

	if (!__get_file_modes(mode, &modes)) {
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	__init_file(file, modes, 0, (size_t)BUFSIZ);

	if (__open_file(name, modes, &file->handle)) {
		file->mode.file_kind = (uchar_t)__closed_file;
		if (file->state.free_buffer) {
			MISRA_EXCEPTION_RULE_20_4()
			free(file->buffer);
		}
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}
	if (modes.io_mode & (uchar_t)__append_mode) {
		(void)fseek(file, 0, SEEK_END);
	}

	*newfile = file;
	return ENOERR;
}

errno_t _EWL_CDECL fopen_s(FILE * _EWL_RESTRICT * _EWL_RESTRICT file, const char_t * _EWL_RESTRICT name, const char_t * _EWL_RESTRICT mode)
{
	FILE *newfile;

	if (file != NULL) {
		*file = NULL;
	}

	if ((file == NULL) || (name == NULL) || (mode == NULL)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		MISRA_EXCEPTION_RULE_14_7()
		return ERANGE;
	}

	__begin_critical_region(files_access);
	newfile = __find_unopened_file();
	__end_critical_region(files_access);

	return(freopen_s(file, name, mode, newfile));
}

#endif /* defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ */

#endif /* _EWL_OS_DISK_FILE_SUPPORT */


