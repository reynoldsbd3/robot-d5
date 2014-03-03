/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:37 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_ARITH_H
#define _EWL_ARITH_H

#include <ansi_parms.h>

#include <div_t.h>

_EWL_BEGIN_EXTERN_C

	int _EWL_CDECL __ewl_add (int  * x, int  y) _EWL_CANT_THROW;
	int _EWL_CDECL __ewl_ladd(long * x, long y) _EWL_CANT_THROW;

	int _EWL_CDECL __ewl_mul (int  * x, int  y) _EWL_CANT_THROW;
	int _EWL_CDECL __ewl_lmul(long * x, long y) _EWL_CANT_THROW;

	__std(ldiv_t) _EWL_CDECL __ewl_ldiv(long x, long y) _EWL_CANT_THROW;
	__std(div_t)  _EWL_CDECL __ewl_div(int  x, int  y) _EWL_CANT_THROW;

	#if _EWL_LONGLONG

		int _EWL_CDECL __lladd(long long * x, long long y) _EWL_CANT_THROW;
		int _EWL_CDECL __llmul(long long * x, long long y) _EWL_CANT_THROW;
	#if _EWL_C99
		__std(lldiv_t) _EWL_CDECL __lldiv(long long x, long long y) _EWL_CANT_THROW;
	#endif /* _EWL_C99 */

	#endif /* _EWL_LONGLONG */

	#ifndef __MOTO__

		int	_EWL_CDECL  __ewl_mod(int  x, int  y) _EWL_CANT_THROW;
		long _EWL_CDECL __ewl_lmod(long x, long y) _EWL_CANT_THROW;

		#if _EWL_LONGLONG
			long long _EWL_CDECL __llmod(long long x, long long y) _EWL_CANT_THROW;
		#endif /* _EWL_LONGLONG */

	#endif /* __MOTO__ */

_EWL_END_EXTERN_C

#endif /* _EWL_ARITH_H */
