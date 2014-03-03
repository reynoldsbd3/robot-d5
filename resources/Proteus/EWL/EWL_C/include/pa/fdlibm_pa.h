/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#ifndef _FDLIBM_PA_H_
#define	_FDLIBM_PA_H_ 1

#if _EWL_FLOATING_POINT

typedef union
{
	f32_t    d;
	int32_t  sw;
	uint32_t uw;
} *_ewl_float2word_ptr;

_MISRA_EXCEPTION_MATHAPISP_MACROS()

#ifndef GET_FLOAT_WORD
	#define GET_FLOAT_WORD(lx,x) (lx) = ((_ewl_float2word_ptr)&x)->sw
#endif
#ifndef GET_FLOAT_UWORD
	#define GET_FLOAT_UWORD(lx,x) (lx) = ((_ewl_float2word_ptr)&x)->uw
#endif
#ifndef SET_FLOAT_WORD
	#define SET_FLOAT_WORD(x,lx) ((_ewl_float2word_ptr)&x)->sw = (lx)
#endif
#ifndef SET_FLOAT_UWORD
	#define SET_FLOAT_UWORD(x,lx) ((_ewl_float2word_ptr)&x)->uw = (lx)
#endif

/* single precision ieee style elementary functions */
extern f32_t _EWL_MATH_CDECL __ieee754_acosf __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_acoshf __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_asinf __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_atan2f __P((f32_t, f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_atanhf __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_coshf __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_expf __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_fmodf __P((f32_t, f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_gammaf __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_gammaf_r __P((f32_t,int32_t *));
extern f32_t _EWL_MATH_CDECL __ieee754_hypotf __P((f32_t, f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_j0f __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_y0f __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_j1f __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_y1f __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_jnf __P((int32_t, f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_ynf __P((int32_t, f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_ldexpf __P((f32_t,int32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_lgammaf __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_lgammaf_r __P((f32_t,int32_t *));
extern f32_t _EWL_MATH_CDECL __ieee754_log10f __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_logf __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_powf __P((f32_t, f32_t));
extern int32_t _EWL_MATH_CDECL __ieee754_rem_pio2f __P((f32_t,f32_t*));
extern f32_t __ieee754_remainderf __P((f32_t, f32_t));
#ifdef _SCALB_INT
extern f32_t _EWL_MATH_CDECL __ieee754_scalbf __P((f32_t,int32_t));
#else
extern f32_t _EWL_MATH_CDECL __ieee754_scalbf __P((f32_t,f32_t));
#endif /* _SCALB_INT */
extern f32_t _EWL_MATH_CDECL __ieee754_sinhf __P((f32_t));
extern f32_t _EWL_MATH_CDECL __ieee754_sqrtf __P((f32_t));

/* fdlibm kernel function */
extern f32_t _EWL_MATH_CDECL __kernel_cosdf __P((f64_t));
extern f32_t _EWL_MATH_CDECL __kernel_sindf __P((f64_t));
extern f32_t _EWL_MATH_CDECL __kernel_tandf __P((f64_t,int32_t));
extern f32_t _EWL_MATH_CDECL __kernel_sinf __P((f32_t,f32_t,int32_t));
extern f32_t _EWL_MATH_CDECL __kernel_cosf __P((f32_t,f32_t));
extern f32_t _EWL_MATH_CDECL __kernel_tanf __P((f32_t,f32_t,int32_t));
extern int32_t _EWL_MATH_CDECL __kernel_rem_pio2f __P((const f32_t*,f32_t*,int32_t,int32_t,int32_t,const int32_t*));
extern int isnanf __P((f32_t));

extern f32_t dremf __P((f32_t,f32_t));
extern int32_t finitef __P((f32_t));
extern int64_t llrintf __P((f32_t));
extern f32_t rintf __P((f32_t));
extern f32_t roundf __P((f32_t));
extern f32_t significandf __P((f32_t));
extern f32_t gammaf __P((f32_t));
extern f32_t gammaf_r __P((f32_t,int_t*));

#endif /* _EWL_FLOATING_POINT */
#endif /* _FDLIBM_PA_H_ */


