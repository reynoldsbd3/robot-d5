/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:41 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_DIRENT_H
#define _EWL_DIRENT_H

#include <ansi_parms.h>

#if _EWL_OS_DISK_FILE_SUPPORT

#ifndef NAME_MAX
	#define NAME_MAX 260		/* this should have been defined in <limits.h>! */
#endif

#pragma options align=native
#pragma warn_padding off

_EWL_BEGIN_EXTERN_C	

	struct dirent
	{
		char d_name[NAME_MAX];
	};
	
#if (__dest_os == __win32_os)

	typedef struct
	{
		struct dirent 	*_d__dirent;
	  	char 			*_d__wildcard;			/* "directory\\*" */
	  	unsigned long 	*_d__handle;			/* for FindNextFile() */
		void 			*_d__ffd;				/* really WIN32_FIND_DATA */ 
	} DIR;

#elif (__dest_os == __mac_os)

	typedef struct
	{
		struct dirent 	_d__dirent;
		
#if _EWL_USE_OLD_FILE_APIS
		short			_d__vrefnum;			/* old style directory vRefNum to iterate */
		long			_d__dirid;				/* old style directory ID to iterate */
		short			_d__index;				/* index in the directory */
#endif /* _EWL_USE_OLD_FILE_APIS */
#if _EWL_USE_NEW_FILE_APIS
		void *			_d__ref;				/* new style FSRef of directory to iterate */
		void *			_d__iterator;			/* new style directory iterator reference */
#endif /* _EWL_USE_NEW_FILE_APIS */
	} DIR;

#endif	/* __dest_os == __win32_os */
	
	
	_EWL_IMP_EXP_C DIR				*opendir(const char *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C struct dirent	*readdir(DIR *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int				readdir_r(DIR *, struct dirent *, struct dirent **) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C void				rewinddir(DIR *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C int				closedir(DIR *) _EWL_CANT_THROW;

_EWL_END_EXTERN_C

#pragma warn_padding reset
#pragma options align=reset

#endif /* _EWL_OS_DISK_FILE_SUPPORT */

#endif /* _EWL_DIRENT_H */
