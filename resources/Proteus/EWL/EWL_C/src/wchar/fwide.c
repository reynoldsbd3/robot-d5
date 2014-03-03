/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:32 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		fwide
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

int_t _EWL_CDECL fwide(FILE* _file, int_t mode)
{
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE * file = (struct _FILE *)_file;
	int_t orientation;
	int_t result = 0;

	if ((file == NULL) || (file->mode.file_kind == (uint_t)__closed_file)) {
		MISRA_EXCEPTION_RULE_14_7()
		return 0;   /* there is no associated stream therefore no orientation */
	}

	orientation = (int_t)file->mode.file_orientation;
	switch (orientation) {
		case __unoriented:
			if (mode > 0) {
				file->mode.file_orientation = (uchar_t)__wide_oriented;
			} else {
				if (mode < 0) {
					file->mode.file_orientation = (uchar_t)__char_oriented;
				}
			}
			result = mode;
			break;

		case __wide_oriented:
			result = 1;
			break;

		case __char_oriented:
			result = -1;
			break;

		default:
			break;
	}
	return result;
}

int_t _EWL_CDECL __fwide(FILE* stream, int_t mode)
{
	return fwide(stream, mode);
}

#endif /* _EWL_WIDE_CHAR */
