/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_WCSTOLD_H
#define _EWL_WCSTOLD_H

#include <ansi_parms.h>
#include <cwchar>

_EWL_BEGIN_EXTERN_C

#if _EWL_WIDE_CHAR
	extern long double _EWL_CDECL __wcstold(int	max_width,
									__std(wint_t) (*ReadProc)(void *, __std(wint_t), int),
									void * ReadProcArg,
									int	* chars_scanned,
									int	* overflow) _EWL_CANT_THROW;

#endif

_EWL_END_EXTERN_C


#endif /* _EWL_WCSTOLD_H */
