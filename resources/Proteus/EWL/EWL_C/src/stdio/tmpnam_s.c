/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/07/12 22:31:14 $
 * $Revision: 1.2 $
 */

/*
 *	Routines
 *	--------
 *		tmpnam
 */

#include <ansi_parms.h>

#include <file_io.h>
#include <errno.h>
#include <critical_regions.h>
#if _EWL_OS_DISK_FILE_SUPPORT
#include <string.h>
#include <stdlib.h>
#endif /* _EWL_OS_DISK_FILE_SUPPORT */

#include <ewl_misra_types.h>

#if _EWL_OS_DISK_FILE_SUPPORT

#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__

errno_t _EWL_CDECL tmpnam_s(char_t *name, rsize_t maxsize)
{
	char_t temp_name[L_tmpnam];

	if ((name == NULL) || (maxsize < 1u) || (maxsize > RSIZE_MAX)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		return ERANGE;
	}

	__temp_file_name(temp_name, NULL);

	if ((strnlen_s(temp_name, (size_t)L_tmpnam) + 1u) > maxsize) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, ERANGE);
		name[0] = 0;
		return ERANGE;
	}

	strcpy(name, temp_name);
	return ENOERR;
}
#endif /* defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ */

#endif /* _EWL_OS_DISK_FILE_SUPPORT */

