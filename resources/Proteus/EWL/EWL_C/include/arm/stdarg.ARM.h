/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:40:17 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_STDARG_ARM_H
#define _EWL_STDARG_ARM_H

_EWL_BEGIN_EXTERN_C

#if defined(__GNUC__)

	#define va_start(v,l)	__builtin_va_start(v,l)
	#define va_end(v)		__builtin_va_end(v)
	#define va_arg(v,l)		__builtin_va_arg(v,l)
	#define va_copy(d,s)	__builtin_va_copy(d,s)
	
#else
	/*	NB: It is safe to ignore endian in these macros. The standard requires that scalar	*/
	/*	types smaller than 'int' (including enum types) be extracted via va_arg(ap,int).	*/
	
	_MISRA_EXCEPTION_STDARG_MACROS()
	#define	__fourbytealign(n)		((((unsigned long) (n)) + 3U) & ~3U)	
	#define __va_start(parm)		((__std(va_list)) ((char*) ((unsigned long)(&parm) & ~3U) + __fourbytealign(sizeof(parm))))
   	
	#define va_start(ap, parm)		((ap) = __va_start(parm))
	#define va_arg(ap, type)		(*(type *) ((ap += __fourbytealign(sizeof(type))) - __fourbytealign(sizeof(type))))
	#define va_end(ap)				((void) 0)

#if _EWL_C99
	#define va_copy(dest, src)		dest = src
#endif
#endif /* defined(__GNUC__) */

_EWL_END_EXTERN_C

#endif /* _EWL_STDARG_ARM_H */

/* Change record:
 * JMM 020423 New file to make stdarg information platform independent
 */
