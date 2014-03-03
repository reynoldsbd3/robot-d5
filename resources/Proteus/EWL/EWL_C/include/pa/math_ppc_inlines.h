/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:43:58 $
 * $Revision: 1.1 $
 */
 
#ifndef __INSIDE_MATH_PPC_H__
#error "This file should only be included inside math_ppc.h"
#endif

#pragma mark -- number checking

/*	isnan, etc.? */

#pragma mark -- _inv_sqrtf --

extern _EWL_INLINE float _inv_sqrtf(float x);

#if defined(__USING_INTRINSIC_SQRT__) && __USING_INTRINSIC_SQRT__

#define __HAS_INV_SQRTF	1

extern _EWL_INLINE float _inv_sqrtf(float x)
{
	if(x > 0.0f)
	{
		double xd = (double)x;
		double guess = (double)__frsqrte(xd);		/* returns an approximation to	*/
		guess = .5f*guess*(3.0f - guess*guess*xd);		/* now have 8  sig bits			*/
		guess = .5f*guess*(3.0f - guess*guess*xd);		/* now have 16 sig bits			*/
		guess = .5f*guess*(3.0f - guess*guess*xd);		/* now have >24 sig bits		*/
		return (float)guess ;
	}
	else if(x)
		return NAN ;
	
	return INFINITY ; 
 }

#else	/* non-intrinsic */

extern const _UINT32 __inv_sqrt_guess[129];
extern const _UINT32 __inv_sqrt_guess2[129];
extern const _UINT32 __two_inv_x[129];
extern const _UINT32 __inv_x_sqr[129];

#define __HAS_INV_SQRTF	1

extern _EWL_INLINE float _inv_sqrtf(float x)
{

  const _UINT32 numbits = (sizeof(__inv_sqrt_guess))/(4*64) + 5;

  /* calculated at compile time(hopefully)--assumes minimal # of
     elements in inv_sqrt_guess is 32 or an integral (power of two)*32 
  */
     
  const _UINT32 bit_shift=23-numbits;
  const _UINT32 bit_mask=0x007fffff&(~(sizeof(__inv_sqrt_guess)>>2)<<bit_shift);
  const _UINT32 first_several_sig_bits_of_x=(*(_UINT32*)&x) & bit_mask;
  const _UINT32 biased_exp=(*(_UINT32*)&x) & 0x7f800000U;
  float guess;
  float scaled_x;
 
 
   if( biased_exp & 0x00800000)  /* if biased_exp is odd then the sqrt of the exponent is 2^^intsqrt(2) */
   {
    (*(_UINT32*)&scaled_x)=0x3E800000 + ((*(_UINT32*)&x)&0x007fffff);  /* scaled_x in [.25,.5) */
    (*(_UINT32*)&guess)=__inv_sqrt_guess[(first_several_sig_bits_of_x >>bit_shift)]; 
    
   }
   else
   {
    (*(_UINT32*)&scaled_x)=0x3f000000 + ((*(_INT32*)&x)&0x007fffffU);  /* scaled_x in [. 5,1.0) */
    (*(_UINT32*)&guess)=__inv_sqrt_guess2[(first_several_sig_bits_of_x>>bit_shift)]; 
   } 
 
/*   guess = .5f*guess*(3.0f - guess*guess*scaled_x);  // now have 12 sig bits
     guess = .5f*guess*(3.0f - guess*guess*scaled_x);  // now have 24 sig bits
   */
      guess *= (3.0f - guess*guess*scaled_x);  /* now have 12 sig bits */
      guess *= (3.0f - guess*guess*scaled_x);  /* now have 24 sig bits */

              
   /* we now reduce x to 2^^n*y  where y is in [.5,1) we then calculate sqrt(x)=sqrt(2^^n)*sqrt(y) 
      where if n is even we simply shift the exponent of guess appropriately or if n is odd we shift
      and multiply by sqrt(2) if n > 0 and 1/sqrt(2) if n > 0
   */
 
  if(biased_exp > 0x3f000000)
    (*(_INT32*)&guess)-=(((biased_exp - 0x3e800000)>>1)&0xffbfffff) ;  /* this subtracts off bias(127=0x3f80...)  
    	                                                            	from biased_exp and one more which divides by two */
   else
    (*(_INT32*)&guess)+=((0x3f000000 - biased_exp)>>1)&0xffbfffff;  

  /* __HI(guess)-=0x01000000;*/  /* eliminates two multiplies */   
  return guess;
} 

#endif /* __USING_INTRINSIC_SQRT__ */

#pragma mark -- sqrt, sqrtf --

#ifndef _No_Floating_Point_Regs

/*	Fast sqrt() and sqrtf() implementations using "frsqrte" 
	intrinsic instruction (non-IEEE conformant due to precision and errno handling) */
	
#if __USING_INTRINSIC_SQRT__
#if __LESS_ACCURATE_FP__

#define __HAS_SQRTF 1

extern _EWL_INLINE float sqrtf(float x)
{
	static const double _half=.5f;
	static const double _three=3.0f;

	if(x > 0.0f)
	{
		double xd = (double)x;
		double guess = __frsqrte(xd);		  		 	/* returns an approximation to	*/
		guess = _half*guess*(_three - guess*guess*xd);	/* now have 12 sig bits 		*/
		guess = _half*guess*(_three - guess*guess*xd);	/* now have 24 sig bits			*/
		return (float)(xd * guess);
	}
	else if (x < 0.0)
		return NAN;
	else
		return x;
}   

#else /* __LESS_ACCURATE_FP__ */

#define __HAS_SQRTF 1

extern _EWL_INLINE float sqrtf(float x)
{
	static const double _half=.5;
	static const double _three=3.0;

	if(x > 0.0f)
	{
		double xd = (double)x;
		double guess = __frsqrte(xd);   				/* returns an approximation to	*/
		guess = _half*guess*(_three - guess*guess*xd);	/* now have 12 sig bits			*/
		guess = _half*guess*(_three - guess*guess*xd);	/* now have 24 sig bits			*/
		guess = _half*guess*(_three - guess*guess*xd);	/* now have 32 sig bits			*/
		return (float)(xd * guess);
	}
	else if (x < 0.0)
		return NAN;
	else if (isnan(x))
		return NAN;
	else
		return x;
}   
#endif /* __LESS_ACCURATE_FP__ */

#define __HAS_SQRT 1

extern _EWL_INLINE double sqrt(double x)
{
	if(x > 0.0)
	{
		double guess = __frsqrte(x);   				/* returns an approximation to	*/
		guess = .5*guess*(3.0 - guess*guess*x);  	/* now have 8 sig bits			*/
		guess = .5*guess*(3.0 - guess*guess*x);  	/* now have 16 sig bits			*/
		guess = .5*guess*(3.0 - guess*guess*x);  	/* now have 32 sig bits			*/
		guess = .5*guess*(3.0 - guess*guess*x);  	/* now have > 53 sig bits		*/
		return x*guess ;
	}
	else if (x < 0.0)
		return NAN;
	else
		return x;
		
/* this is nonsense, since negatives come out as NAN while NANs are infinity */
/*
 else if ( x == 0 )
 	return 0;
 else if ( x )
 	return NAN;

	return INFINITY;
*/

}

#endif /*__USING_INTRINSIC_SQRT__*/

#else /* _No_Floating_Point_Regs */

#if !__USING_IEEE_MATH__ 

/*	Zen has optimized sqrt routines */
#if !defined(__PPCZen__) && !defined(__PPCe500__) && !defined(__PPCe500v2)

#define __HAS_SQRTF	1
extern _EWL_INLINE float sqrtf(float);
extern _EWL_INLINE float sqrtf(float x)
{
	if (x > 0.0)
		return x * _inv_sqrtf(x);
	else if (x == 0.0)
		return (float)0.0;
	else if (isnan(x))
	{
		errno = EDOM;
		return NAN;
	}
	else
		return x;
}

#endif /*!defined(__PPCZen__) && !defined(__PPCe500__) && !defined(__PPCe500v2)*/

#endif /* __USING_IEEE_MATH__*/

#endif /*_No_Floating_Point_Regs*/

#if 0
#pragma mark -- fabs --
#endif

#ifdef __USING_INTRINSIC_FABS__

#define __HAS_FABS	1

extern _EWL_INLINE double fabs(double x)
{
   return __fabs(x) ;
}

#else

#define __HAS_FABSF	1

#pragma only_std_keywords off
#pragma ANSI_strict off
extern _EWL_INLINE float _EWL_MATH_CDECL fabsf(float x)
{
#if defined(__CWCC__) && (__option(e500_floatingpoint) || __option(e500v2_floatingpoint))
    register float rD, rA=x;
    asm {.nonvolatile; efsabs rD,rA; }
    return rD;
#else
	typedef union {
		float	f;
		int		i;
	} f2i;
	f2i z;
	z.f = x;
	z.i = z.i&0x7fffffffL;
	return  z.f;
#endif
}
#pragma ANSI_strict reset
#pragma only_std_keywords reset

#endif /* __USING_INTRINSIC_FABS__ */


