/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:50 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_EXTRAS_MALLOC_WIN32_H
#define _EWL_EXTRAS_MALLOC_WIN32_H

#include <ansi_parms.h>

_EWL_BEGIN_EXTERN_C

	_EWL_IMP_EXP_C  int _EWL_CDECL _heapmin(void) _EWL_CANT_THROW;

_EWL_END_EXTERN_C

#endif /* _EWL_EXTRAS_MALLOC_WIN32_H */

/* Change record:
 * cc  030622 Extras split to reduce namespace pollution
 */