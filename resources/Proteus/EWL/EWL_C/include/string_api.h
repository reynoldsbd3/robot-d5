/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_STRING_API_H
#define _EWL_STRING_API_H

#include <ansi_parms.h>

#define __max_errstr 40

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C

	char * _EWL_CDECL __strerror(int, char *) _EWL_CANT_THROW;
	void * _EWL_CDECL __memrchr(const void *, int, size_t) _EWL_CANT_THROW;

_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

#endif /* _EWL_STRING_API_H */
