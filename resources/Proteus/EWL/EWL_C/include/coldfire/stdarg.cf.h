/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:38:39 $
 * $Revision: 1.2 $
 */

#ifndef _EWL_STDARG_CF_H
#define _EWL_STDARG_CF_H

_EWL_BEGIN_EXTERN_C

#if defined(__GNUC__)

	#define va_start(v,l)	__builtin_va_start(v,l)
	#define va_end(v)		__builtin_va_end(v)
	#define va_arg(v,l)		__builtin_va_arg(v,l)
	#define va_copy(d,s)	__builtin_va_copy(d,s)
	
#else

	#define va_start(ap, parm)	ap = __va_start(parm)
	#define va_end(ap)       ((void)0)

#if _EWL_C99
	#define va_copy(dest, src)  dest = src
#endif

_MISRA_EXCEPTION_STDARG_MACROS()
#if __STDABI__ || __REGABI__ || __option(native_coldfire_alignment)
	#define __va_start(parm) (__std(va_list)) (&parm + 1)
	#define va_arg(ap, type) (*(type *)((ap += sizeof(type) + 3U & ~3U) - (sizeof(type))))
#else
	#define __va_start(parm) (__std(va_list)) (((char_t *)&parm) + ((sizeof(parm) + (1U)) & ~(1U)))
	#define va_arg(ap, type) (*(((type *) (ap += sizeof(type))) - 1))
#endif

#endif /* defined(__GNUC__) */

_EWL_END_EXTERN_C

#endif /* _EWL_STDARG_CF_H */
