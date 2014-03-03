/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:43 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_EXTRAS_STDLIB_H
#define _EWL_EXTRAS_STDLIB_H

#include <ansi_parms.h>

_EWL_BEGIN_EXTERN_C

	_EWL_IMP_EXP_C int _EWL_CDECL rand_r(unsigned int *) _EWL_CANT_THROW;
	
	_EWL_IMP_EXP_C char * _EWL_CDECL ultoa(unsigned long, char *, int) _EWL_CANT_THROW;
 	
	extern char * _EWL_CDECL __ewl_itoa(int val, char *str, int radix);
	__inline char * _EWL_CDECL itoa(int val, char *str, int radix) _EWL_CANT_THROW { return __ewl_itoa(val, str, radix); }
	__inline char * _EWL_CDECL ltoa(long x, char *y, int z) _EWL_CANT_THROW { return (itoa(x, y, z)); }
	
	#if _EWL_FLOATING_POINT
		_EWL_IMP_EXP_C char * _EWL_CDECL gcvt(double, int, char *) _EWL_CANT_THROW;
	#endif

_EWL_END_EXTERN_C

#endif /* _EWL_EXTRAS_STDLIB_H */
