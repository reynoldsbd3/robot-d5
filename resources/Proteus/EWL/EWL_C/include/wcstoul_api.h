/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_WCSTOUL_H
#define _EWL_WCSTOUL_H

#include <ansi_parms.h>

#if _EWL_WIDE_CHAR

#include <wchar_t.h>

_EWL_BEGIN_EXTERN_C

    extern unsigned long _EWL_CDECL		__wcstoul(	int	base,
										int		max_width,
										__std(wint_t) (*ReadProc)(void *, __std(wint_t), int),
										void * ReadProcArg,
										int	* chars_scanned,
										int	* negative,
										int	* overflow) _EWL_CANT_THROW;

#if _EWL_LONGLONG
	extern unsigned long long _EWL_CDECL __wcstoull(	int	base,
										 int		max_width,
										 __std(wint_t) (*ReadProc)(void *, __std(wint_t), int),
										 void * ReadProcArg,
										 int	* chars_scanned,
										 int	* negative,
										 int	* overflow) _EWL_CANT_THROW;
#endif /*_EWL_LONGLONG*/

_EWL_END_EXTERN_C
#endif /* _EWL_WIDE_CHAR  */
#endif /* _EWL_WCSTOUL_H */
