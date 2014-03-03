/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:43:58 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_STDARG_EPPC_H
#define _EWL_STDARG_EPPC_H

_EWL_BEGIN_EXTERN_C

#if defined(__GNUC__)

	#define va_start(v,l)	__builtin_va_start(v,l)
	#define va_end(v)		__builtin_va_end(v)
	#define va_arg(v,l)		__builtin_va_arg(v,l)
	#define va_copy(d,s)	__builtin_va_copy(d,s)
	
#elif defined(_GHSPORT_)

	typedef struct _va_struct {
		char _gpr,_fpr,_pad1,_pad2;
		#ifdef __PPC64__
			char _padding[4];
		#endif
		char    *_overflow_arg_area,*_reg_save_area;
		} __gh_va_list[1];

	#define __va_copy_as_struct

	typedef __gh_va_list va_list;
	void *__va_arg    (va_list, int);

	#if defined(__EDG__)
		#ifdef __FloatSingle
			#define __greg_va_type(mod) \
		       (__va_regtyp(mod)? \
			/* 1 or 2 for int or long long int */ \
			  ((sizeof(mod) > 4) ? 2 : 1) : \
			/* if not a register type, then pass a 0 */ \
			0)
		#else /* __FloatSingle */
			#define __greg_va_type(mod) \
		       (__va_regtyp(mod)? \
			/* if a register type, is it a double - then 3.  */ \
			/* appears 4 means float (ppc602 lunacy) */ \
			/* 1 or 2 for int or long long int */ \
			/* 5 will be taken to mean an AltiVec vector */ \
			(__va_float(mod) ? 3 : \
			 (__va_vector(mod) ? 5 :     \
			  ((sizeof(mod) > 4) ? 2 : 1))) : \
			/* if not a register type, then pass a 0 */ \
			0)
		#endif /* __FloatSingle */
	#else /* if !EDG */
		#define __greg_va_type(mod) \
	       (__va_regtyp(mod)? \
		/* if a register type, is it a double - then 3.  */ \
		/* appears 4 means float (ppc602 lunacy) */ \
		/* 1 or 2 for int or long long int */ \
		/* 5 will be taken to mean an AltiVec vector */ \
		(__va_float(mod) ? 3 : \
		 ((sizeof(mod) == 16) ? 5 :       \
		  ((sizeof(mod) > 4) ? 2 : 1))) : \
		/* if not a register type, then pass a 0 */ \
		0)
	#endif
	
	#if defined(__BigEndian)
		#define va_arg(list,mod) \
	        /* small ints (chars, shorts) need this shift - this is actually */ \
		/* not even required by ansi */ \
	       (((sizeof(mod) >= 3) || (__greg_va_type(mod) != 1)) ? \
		(*(mod*)__va_arg(list,__greg_va_type(mod))) : \
	        (*(mod*)((char*)__va_arg(list,__greg_va_type(mod)) \
							       - (sizeof(mod)-4u))))
	#else /* little endian */
		#define va_arg(list,mod) \
		(*(mod*)__va_arg(list,__greg_va_type(mod)))
	#endif /* #ifdef __BigEndian */

	#define va_start(p,va_alist) \
		((p)->_overflow_arg_area=(char *)(&__va_ansiarg), \
		 (p)->_reg_save_area=(char *)__va_intreg, \
		 (p)->_gpr= __va_iargnum(va_alist), \
		 (p)->_fpr= __va_dargnum(va_alist))

#else

	extern void* __va_arg(void*, int);

	#if _lint

		#define va_start(a,t)
		#define va_arg(a,t)   (*((t*)a))
		#define va_end(a)     ((void)0)

	#else

		#define __va_start(ap,fmt) ((void)fmt,__builtin_va_info(&ap))
		#define __va_arg(ap,t)	(*((t*)__va_arg(ap,_var_arg_typeof(t))))
		
		#define va_start __va_start
		#define va_arg   __va_arg
		#define va_end   __va_end

	#endif /* _lint */
	
	#define __va_end(ap) ((void)0)
	
	/* macro for copying one va_list to another */
	#define	__va_copy(a,b) ((a)[0] = (b)[0])

	#if _EWL_C99
		#define va_copy	__va_copy
	#endif

#endif /* __GNUC__ */

_EWL_END_EXTERN_C

#endif /* _EWL_STDARG_EPPC_H */
