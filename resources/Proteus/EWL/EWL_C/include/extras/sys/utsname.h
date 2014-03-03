/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:43:25 $
 * $Revision: 1.1 $
 */

/*
 *	Content:	Interface file to standard UNIX-style entry points ...
 *
 *	NB:			This file implements some UNIX low level support.  These functions
 *				are not guaranteed to be 100% conformant.
 */

#ifndef	_EWL_UTSNAME_H
#define	_EWL_UTSNAME_H

#include <ansi_parms.h>

#pragma options align=native

_EWL_BEGIN_EXTERN_C

	#define _UTSNAME_FIELD_LENGTH 32
	
	/* struct for uname */
	struct utsname {
		char    sysname[_UTSNAME_FIELD_LENGTH];
		char    nodename[_UTSNAME_FIELD_LENGTH];
		char    release[_UTSNAME_FIELD_LENGTH];
		char    version[_UTSNAME_FIELD_LENGTH];
		char    machine[_UTSNAME_FIELD_LENGTH];
	};
	
	_EWL_IMP_EXP_C int _EWL_CDECL uname (struct utsname* ) _EWL_CANT_THROW;
	

_EWL_END_EXTERN_C

#pragma options align=reset

#endif /*_EWL_UTSNAME_H */

