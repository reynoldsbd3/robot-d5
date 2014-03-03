/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:49 $
 * $Revision: 1.2 $
 */

/*
 *		This file was built to correspond with the Visual C++ direct.h
 */

#ifndef _EWL_DIRECT_H
#define _EWL_DIRECT_H

#include <ansi_parms.h>

#if (__dest_os	== __win32_os )

_EWL_BEGIN_EXTERN_C

	#ifndef _SIZE_T_DEFINED
		typedef unsigned int size_t;
		#define _SIZE_T_DEFINED
	#endif
	
	#ifndef _DISKFREE_T_DEFINED
	struct _diskfree_t {
		unsigned long total_clusters;
		unsigned long avail_clusters;
		unsigned long sectors_per_cluster;
		unsigned long bytes_per_sector;
		};
	
	#define _DISKFREE_T_DEFINED
	#endif

	_EWL_IMP_EXP_C char * _EWL_CDECL		_getdcwd(int, char *, int) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C unsigned long _EWL_CDECL	_getdrives(void) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C unsigned _EWL_CDECL		_getdiskfree(unsigned, struct _diskfree_t *) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C 	int  _EWL_CDECL _chdrive(int) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C 	int  _EWL_CDECL _getdrive() _EWL_CANT_THROW;	
	
	#define diskfree_t  _diskfree_t

_EWL_END_EXTERN_C

#endif /* __dest_os == __win32_os */
#endif /* _EWL_DIRECT_H */

/* Change record:
 * hh  971206 Added #include <ansi_parms.h>
 * cc  000316 added _EWL_IMP_EXP_C
 * cc  000410 removed duplicate prototypes - look at extras.win32.h
 * cc  000518 removed unistd.h
 * cc  010410 updated to new namespace macros 	
 * cc  011203 Added _EWL_CDECL for new name mangling 
 */