/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_MEM_FUNCS_H
#define _EWL_MEM_FUNCS_H

#include <ansi_parms.h>

#ifndef _EWL_LONG_COPY_MIN
	#define _EWL_LONG_COPY_MIN	32		/* NEVER let this be < 16 */
#endif

#define __min_bytes_for_long_copy	_EWL_LONG_COPY_MIN

_EWL_BEGIN_EXTERN_C

	void _EWL_CDECL	__copy_mem                (void * dst, const void * src, unsigned long n) _EWL_CANT_THROW;
	void _EWL_CDECL	__move_mem                (void * dst, const void * src, unsigned long n) _EWL_CANT_THROW;
	void _EWL_CDECL	__copy_longs_aligned      (void * dst, const void * src, unsigned long n) _EWL_CANT_THROW;
	void _EWL_CDECL	__copy_longs_rev_aligned  (void * dst, const void * src, unsigned long n) _EWL_CANT_THROW;
	void _EWL_CDECL	__copy_longs_unaligned    (void * dst, const void * src, unsigned long n) _EWL_CANT_THROW;
	void _EWL_CDECL	__copy_longs_rev_unaligned(void * dst, const void * src, unsigned long n) _EWL_CANT_THROW;
	void _EWL_CDECL	__fill_mem				  (void * dst, int val, unsigned long n) _EWL_CANT_THROW;

_EWL_END_EXTERN_C

#endif /* _EWL_MEM_FUNCS_H */
