/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:43:58 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_SETJMP_EPPC_H
#define _EWL_SETJMP_EPPC_H

#pragma pack(push,4)

_EWL_BEGIN_NAMESPACE_STD
_EWL_BEGIN_EXTERN_C

	#if __VEC__
	
		typedef __vector unsigned long jmp_buf[29];
		
		#ifndef __SETJMP_NOT_INTERNAL__
			#pragma internal on
		#endif
		
			_EWL_IMP_EXP_C int __vec_setjmp(jmp_buf);
		
		#ifndef __SETJMP_NOT_INTERNAL__
			#pragma internal reset
		#endif
		
		int __vec_longjmp(jmp_buf, int);
		
		#define setjmp(jmp_buf)	__std(__vec_setjmp(jmp_buf))
		_EWL_INLINE void longjmp(jmp_buf j, int val) { __vec_longjmp(j, val); }
	
	#else
	
		#if __PPCGEKKO__
			typedef long jmp_buf[100];
		#elif __PPCe500__ || __PPCZen__ || __PPCe500v2
			typedef long jmp_buf[44];
		#else
			typedef long jmp_buf[70];	/* really 68... */
		#endif
		
		#ifndef __SETJMP_NOT_INTERNAL__
			#pragma internal on
		#endif
		
			_EWL_IMP_EXP_C int __setjmp(jmp_buf);
		
		#ifndef __SETJMP_NOT_INTERNAL__
			#pragma internal reset
		#endif
		
		#define setjmp(jmp_buf)	__std(__setjmp(jmp_buf))
		void longjmp(jmp_buf,int);
	
	#endif

_EWL_END_EXTERN_C
_EWL_END_NAMESPACE_STD

#pragma pack(pop)

#endif /* _EWL_SETJMP_EPPC_H */
