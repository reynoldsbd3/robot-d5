/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:55 $
 * $Revision: 1.2 $
 */

#ifndef	_MSL_TIMEB_WIN32_H
#define	_MSL_TIMEB_WIN32_H

#include <ansi_parms.h>

_MSL_BEGIN_EXTERN_C

	_MSL_IMP_EXP_C void _MSL_CDECL ftime(struct timeb *) _MSL_CANT_THROW;
	_MSL_IMP_EXP_C void _MSL_CDECL _ftime(struct timeb *) _MSL_CANT_THROW;

_MSL_END_EXTERN_C

#endif /* _MSL_TIMEB_WIN32_H */

/* Change record:
 * cc  020711 Added ftime
 */