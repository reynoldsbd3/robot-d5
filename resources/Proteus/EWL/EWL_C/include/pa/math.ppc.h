
#ifndef __MATH_PPC_H__
#define __MATH_PPC_H__ 1

	/*	Zen/e500/e500v2 has optimized sqrt/sqrtf routines */
	#ifndef __HAS_SQRTF
		#if defined(__PPCZen__) || defined(__PPCe500__) || defined(__PPCe500v2)
			#define __HAS_SQRTF 1
		#elif _EWL_USES_SUN_SP_MATH_LIB
			#define __HAS_SQRTF 1
		#else
			#define __HAS_SQRTF 0
		#endif /* defined(__PPCZen__) || defined(__PPCe500__) || defined(__PPCe500v2) */
	#endif /* __HAS_SQRTF */
	#ifndef __HAS_SQRT
		#if _EWL_USES_SUN_SP_MATH_LIB
			#define __HAS_SQRT 0
		#else
			/* sqrt() will probably come from some libm */
			#define __HAS_SQRT 1
		#endif /* _EWL_USES_SUN_SP_MATH_LIB */
	#endif /* __HAS_SQRT */

#endif /* __MATH_PPC_H__ */

