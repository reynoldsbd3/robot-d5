/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:57 $
 * $Revision: 1.2 $
 */

/*
 *	Content:	Interface file to standard UNIX-style entry points ...
 *
 *	NB:			This file implements some UNIX low level support.  These functions
 *				are not guaranteed to be 100% conformant.
 */

#ifndef	_MSL_UTSNAME_H
#define	_MSL_UTSNAME_H

#include <ansi_parms.h>

_MSL_BEGIN_EXTERN_C

	__inline int _MSL_CDECL _uname(struct utsname* name ) _MSL_CANT_THROW { return uname(name); }

_MSL_END_EXTERN_C

#endif /*_MSL_UTSNAME_H */

/* Change record:
 * cc 010517 Made for _version of POSIX 
 * cc 010715 Removed circular include 
 * cc  011203 Added _MSL_CDECL for new name mangling 
 */