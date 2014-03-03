/* EWL
 * Copyright © 2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:36 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		tmpnam
 */

#include <ansi_parms.h>

#include <file_io.h>
#include <critical_regions.h>

#include <ewl_misra_types.h>

#if _EWL_OS_DISK_FILE_SUPPORT

char_t * _EWL_CDECL tmpnam(char_t * name)
{

#if !_EWL_THREADSAFE || !_EWL_LOCALDATA_AVAILABLE
	static char_t temp_name[L_tmpnam];
#endif

	if (name) {
		__temp_file_name(name, NULL);
		return(name);
	} else {
		__temp_file_name(_EWL_LOCALDATA(temp_name), NULL);
		return(_EWL_LOCALDATA(temp_name));
	}
}

#endif /* _EWL_OS_DISK_FILE_SUPPORT */

