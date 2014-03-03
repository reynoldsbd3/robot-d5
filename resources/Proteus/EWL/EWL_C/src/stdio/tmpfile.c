/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/07/12 22:31:14 $
 * $Revision: 1.2 $
 */

/*
 *	Routines
 *	--------
 *		tmpfile
 */

#include <ansi_parms.h>

#include <misc_io.h>
#include <file_io.h>
#include <ansi_files.h>
#include <critical_regions.h>

#include <ewl_misra_types.h>

#if _EWL_OS_DISK_FILE_SUPPORT

static __file_modes	__temp_file_mode =
#if _EWL_WIDE_CHAR
	{(uchar_t)__create_or_truncate, (uchar_t)__read_write_mode, (uchar_t)_IOFBF, (uchar_t)__disk_file, (uchar_t)__unoriented, 1u};
#else
	{(uchar_t)__create_or_truncate, (uchar_t)__read_write_mode, (uchar_t)_IOFBF, (uchar_t)__disk_file, 1u};
#endif /* _EWL_WIDE_CHAR */

FILE * _EWL_CDECL tmpfile(void)
{
	FILE *	file;

	__stdio_atexit();
	__begin_critical_region(files_access);

	file = __find_unopened_file();
	if (file != 0) {
		__end_critical_region(files_access);
		__init_file(file, __temp_file_mode, 0, (size_t)BUFSIZ);
		__begin_critical_region(files_access);

		if (__open_temp_file(&file->handle)) {
			file->mode.file_kind = (uchar_t)__closed_file;
			file = NULL;
		}
	}

	__end_critical_region(files_access);

	return(file);
}

#endif /* _EWL_OS_DISK_FILE_SUPPORT */
