/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/07/12 22:31:14 $
 * $Revision: 1.2 $
 */

/*
 *	Routines
 *	--------
 *		fclose
 */

#include <ansi_parms.h>

#include <file_io.h>
#if _EWL_OS_DISK_FILE_SUPPORT
#include <stdlib.h>
#endif /* _EWL_OS_DISK_FILE_SUPPORT */

#include <ewl_misra_types.h>

MISRA_EXCEPTION_RULE_16_2()
int_t _EWL_CDECL fclose(FILE * _file)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	int_t	flush_result, close_result;

	if (file == NULL) {
		MISRA_EXCEPTION_RULE_14_7()
		return(EOF);
	}
	if (file->mode.file_kind == (uint_t)__closed_file) {
		MISRA_EXCEPTION_RULE_14_7()
		return(0);
	}

	flush_result = fflush(_file);

	close_result = (*file->close_proc)(file->handle);

	file->mode.file_kind = (uchar_t)__closed_file;
	file->handle         = 0U;

	if (file->state.free_buffer) {
#if _EWL_OS_DISK_FILE_SUPPORT
		MISRA_EXCEPTION_RULE_20_4()
		free(file->buffer);
#else
		MISRA_EXCEPTION_RULE_14_7()
		return(-1);
#endif
	}

	if (flush_result || close_result) {
		MISRA_EXCEPTION_RULE_14_7()
		return EOF;
	} else {
		MISRA_EXCEPTION_RULE_14_7()
		return 0L;
	}
}

