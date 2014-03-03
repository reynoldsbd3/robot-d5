/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:38 $
 * $Revision: 1.1 $
 */

/*
 *  Non-standard integral overloads for math functions
 *
 *  Not for direct client use.
 */

#ifndef _EWL_MATH_INTEGRAL_H
#define _EWL_MATH_INTEGRAL_H

#include <ansi_parms.h>

#if _EWL_FLOATING_POINT			 \
  && !defined(_EWL_NO_MATH_LIB)  \
  && defined(__cplusplus)        \
  && defined(__ANSI_OVERLOAD__)  \
  && defined(_EWL_INTEGRAL_MATH) \
  && !(__dest_os==__mac_os && defined(__FP__))\
  && !defined(__m56800E__)

_EWL_BEGIN_NAMESPACE_STD

#if defined(__CWCC__) && !__option(bool) && !defined(_EWL_NO_BOOL)
	#ifndef bool
		#ifdef _EWL_BOOL_TYPE
			#define bool _EWL_BOOL_TYPE
		#else
			#define bool unsigned char
		#endif
		#define __EWL_MATH_BOOL
	#endif
	#ifndef false
		#define false 0
		#define __EWL_MATH_FALSE
	#endif
	#ifndef true
		#define true 1
		#define __EWL_MATH_TRUE
	#endif
#endif

template <class _T> struct __ewl_is_integral              {static const bool value = false;};

#if __option(bool)
template <> struct __ewl_is_integral<bool>               {static const bool value = true;};
#endif
template <> struct __ewl_is_integral<char>               {static const bool value = true;};
template <> struct __ewl_is_integral<signed char>        {static const bool value = true;};
template <> struct __ewl_is_integral<unsigned char>      {static const bool value = true;};
#if __option(wchar_type)
template <> struct __ewl_is_integral<wchar_t>            {static const bool value = true;};
#endif
template <> struct __ewl_is_integral<short>              {static const bool value = true;};
template <> struct __ewl_is_integral<unsigned short>     {static const bool value = true;};
template <> struct __ewl_is_integral<int>                {static const bool value = true;};
template <> struct __ewl_is_integral<unsigned int>       {static const bool value = true;};
template <> struct __ewl_is_integral<long>               {static const bool value = true;};
template <> struct __ewl_is_integral<unsigned long>      {static const bool value = true;};
#if _EWL_LONGLONG
template <> struct __ewl_is_integral<long long>          {static const bool value = true;};
template <> struct __ewl_is_integral<unsigned long long> {static const bool value = true;};
#endif

template <class _T> struct __ewl_is_floating      {static const bool value = false;};
template <> struct __ewl_is_floating<float>       {static const bool value = true;};
template <> struct __ewl_is_floating<double>      {static const bool value = true;};
template <> struct __ewl_is_floating<long double> {static const bool value = true;};

template <class _T> struct __ewl_is_arithmetic
	{static const bool value = __ewl_is_integral<_T>::value ||
	                           __ewl_is_floating<_T>::value;};


template <bool, class _T = void> struct __ewl_enable_if {};
template <class _T> struct __ewl_enable_if<true, _T> {typedef _T type;};

template <bool _B, class _If, class _Then> struct __ewl_if           {typedef _If type;};
template <class _If, class _Then> struct __ewl_if<false, _If, _Then> {typedef _Then type;};

template <class _A1, class _A2 = void, class _A3 = void>
class __ewl_promote
{
private:
	typedef typename __ewl_promote<_A1>::type type1;
	typedef typename __ewl_promote<_A2>::type type2;
	typedef typename __ewl_promote<_A3>::type type3;
public:
	typedef __typeof__(type1() + type2() + type3()) type;
};

template <class _A1>
class __ewl_promote<_A1, void, void>
{
public:
	typedef typename __ewl_if<__ewl_is_arithmetic<_A1>::value,
	                 typename __ewl_if<__ewl_is_integral<_A1>::value, double, _A1>::type,
	                 char
	        >::type type;
};

template <class _A1, class _A2>
class __ewl_promote<_A1, _A2, void>
{
private:
	typedef typename __ewl_promote<_A1>::type type1;
	typedef typename __ewl_promote<_A2>::type type2;
public:
	typedef __typeof__(type1() + type2()) type;
};

template <class _A1, class _A2>
struct __ewl_is_same
{
	static const bool value = false;
};

template <class _A1>
struct __ewl_is_same<_A1, _A1>
{
	static const bool value = true;
};

#ifdef __EWL_MATH_BOOL
	#undef bool
	#undef __EWL_MATH_BOOL
#endif

#ifdef __EWL_MATH_FALSE
	#undef false
	#undef __EWL_MATH_FALSE
#endif

#ifdef __EWL_MATH_TRUE
	#undef true
	#undef __EWL_MATH_TRUE
#endif

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL acos(_A1 x) {return acos((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL acosh(_A1 x) {return acosh((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL asin(_A1 x) {return asin((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL asinh(_A1 x) {return asinh((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL atan(_A1 x) {return atan((double)x);}

template <class _A1, class _A2>
inline
typename __ewl_enable_if<
	__ewl_is_arithmetic<_A1>::value && __ewl_is_arithmetic<_A2>::value,
	typename __ewl_promote<_A1, _A2>::type
>::type 
_EWL_MATH_CDECL atan2(_A1 x, _A2 y)
{
	typedef typename __ewl_promote<_A1, _A2>::type r_type;
	char test[!(__ewl_is_same<_A1, r_type>::value && __ewl_is_same<_A2, r_type>::value)];
	(void)test;
	return atan2((r_type)x, (r_type)y);
}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL atanh(_A1 x) {return atanh((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL cbrt(_A1 x) {return cbrt((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL ceil(_A1 x) {return ceil((double)x);}

template <class _A1, class _A2>
inline
typename __ewl_enable_if<
	__ewl_is_arithmetic<_A1>::value && __ewl_is_arithmetic<_A2>::value,
	typename __ewl_promote<_A1, _A2>::type
>::type 
_EWL_MATH_CDECL copysign(_A1 x, _A2 y)
{
	typedef typename __ewl_promote<_A1, _A2>::type r_type;
	char test[!(__ewl_is_same<_A1, r_type>::value && __ewl_is_same<_A2, r_type>::value)];
	(void)test;
	return copysign((r_type)x, (r_type)y);
}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL cos(_A1 x) {return cos((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL cosh(_A1 x) {return cosh((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL erf(_A1 x) {return erf((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL erfc(_A1 x) {return erfc((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL exp(_A1 x) {return exp((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL exp2(_A1 x) {return exp2((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL expm1(_A1 x) {return expm1((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL fabs(_A1 x) {return fabs((double)x);}


template <class _A1, class _A2>
inline
typename __ewl_enable_if<
	__ewl_is_arithmetic<_A1>::value && __ewl_is_arithmetic<_A2>::value,
	typename __ewl_promote<_A1, _A2>::type
>::type 
_EWL_MATH_CDECL fdim(_A1 x, _A2 y)
{
	typedef typename __ewl_promote<_A1, _A2>::type r_type;
	char test[!(__ewl_is_same<_A1, r_type>::value && __ewl_is_same<_A2, r_type>::value)];
	(void)test;
	return fdim((r_type)x, (r_type)y);
}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL floor(_A1 x) {return floor((double)x);}

template <class _A1, class _A2, class _A3>
inline
typename __ewl_enable_if<
	__ewl_is_arithmetic<_A1>::value && __ewl_is_arithmetic<_A2>::value && __ewl_is_arithmetic<_A3>::value,
	typename __ewl_promote<_A1, _A2, _A3>::type
>::type 
_EWL_MATH_CDECL fma(_A1 x, _A2 y, _A3 z)
{
	typedef typename __ewl_promote<_A1, _A2, _A3>::type r_type;
	char test[!(__ewl_is_same<_A1, r_type>::value && __ewl_is_same<_A2, r_type>::value
	                                              && __ewl_is_same<_A3, r_type>::value)];
	(void)test;
	return fma((r_type)x, (r_type)y, (r_type)z);
}

template <class _A1, class _A2>
inline
typename __ewl_enable_if<
	__ewl_is_arithmetic<_A1>::value && __ewl_is_arithmetic<_A2>::value,
	typename __ewl_promote<_A1, _A2>::type
>::type 
_EWL_MATH_CDECL fmax(_A1 x, _A2 y)
{
	typedef typename __ewl_promote<_A1, _A2>::type r_type;
	char test[!(__ewl_is_same<_A1, r_type>::value && __ewl_is_same<_A2, r_type>::value)];
	(void)test;
	return fmax((r_type)x, (r_type)y);
}

template <class _A1, class _A2>
inline
typename __ewl_enable_if<
	__ewl_is_arithmetic<_A1>::value && __ewl_is_arithmetic<_A2>::value,
	typename __ewl_promote<_A1, _A2>::type
>::type 
_EWL_MATH_CDECL fmin(_A1 x, _A2 y)
{
	typedef typename __ewl_promote<_A1, _A2>::type r_type;
	char test[!(__ewl_is_same<_A1, r_type>::value && __ewl_is_same<_A2, r_type>::value)];
	(void)test;
	return fmin((r_type)x, (r_type)y);
}

template <class _A1, class _A2>
inline
typename __ewl_enable_if<
	__ewl_is_arithmetic<_A1>::value && __ewl_is_arithmetic<_A2>::value,
	typename __ewl_promote<_A1, _A2>::type
>::type 
_EWL_MATH_CDECL fmod(_A1 x, _A2 y)
{
	typedef typename __ewl_promote<_A1, _A2>::type r_type;
	char test[!(__ewl_is_same<_A1, r_type>::value && __ewl_is_same<_A2, r_type>::value)];
	(void)test;
	return fmod((r_type)x, (r_type)y);
}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL frexp(_A1 x, int* y) {return frexp((double)x, y);}

template <class _A1, class _A2>
inline
typename __ewl_enable_if<
	__ewl_is_arithmetic<_A1>::value && __ewl_is_arithmetic<_A2>::value,
	typename __ewl_promote<_A1, _A2>::type
>::type 
_EWL_MATH_CDECL hypot(_A1 x, _A2 y)
{
	typedef typename __ewl_promote<_A1, _A2>::type r_type;
	char test[!(__ewl_is_same<_A1, r_type>::value && __ewl_is_same<_A2, r_type>::value)];
	(void)test;
	return hypot((r_type)x, (r_type)y);
}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, int>::type 
_EWL_MATH_CDECL ilogb(_A1 x) {return ilogb((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL ldexp(_A1 x, int y) {return ldexp((double)x, y);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL lgamma(_A1 x) {return lgamma((double)x);}

#if _EWL_LONGLONG

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, long long>::type 
_EWL_MATH_CDECL llrint(_A1 x) {return llrint((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, long long>::type 
_EWL_MATH_CDECL llround(_A1 x) {return llround((double)x);}

#endif

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL log(_A1 x) {return log((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL log10(_A1 x) {return log10((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL log1p(_A1 x) {return log1p((double)x);}


template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL log2(_A1 x) {return log2((double)x);}


template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL logb(_A1 x) {return logb((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, long>::type 
_EWL_MATH_CDECL lrint(_A1 x) {return lrint((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, long>::type 
_EWL_MATH_CDECL lround(_A1 x) {return lround((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL modf(_A1 x, double* y) {return modf((double)x, y);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL nearbyint(_A1 x) {return nearbyint((double)x);}

template <class _A1, class _A2>
inline
typename __ewl_enable_if<
	__ewl_is_arithmetic<_A1>::value && __ewl_is_arithmetic<_A2>::value,
	typename __ewl_promote<_A1, _A2>::type
>::type 
_EWL_MATH_CDECL nextafter(_A1 x, _A2 y)
{
	typedef typename __ewl_promote<_A1, _A2>::type r_type;
	char test[!(__ewl_is_same<_A1, r_type>::value && __ewl_is_same<_A2, r_type>::value)];
	(void)test;
	return nextafter((r_type)x, (r_type)y);
}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL nexttoward(_A1 x, long double y) {return nexttoward((double)x, y);}

template <class _A1, class _A2>
inline
typename __ewl_enable_if<
	__ewl_is_arithmetic<_A1>::value && __ewl_is_arithmetic<_A2>::value,
	typename __ewl_promote<_A1, _A2>::type
>::type 
_EWL_MATH_CDECL pow(_A1 x, _A2 y)
{
	typedef typename __ewl_promote<_A1, _A2>::type r_type;
	char test[!(__ewl_is_same<_A1, r_type>::value && __ewl_is_same<_A2, r_type>::value)];
	(void)test;
	return pow((r_type)x, (r_type)y);
}


template <class _A1, class _A2>
inline
typename __ewl_enable_if<
	__ewl_is_arithmetic<_A1>::value && __ewl_is_arithmetic<_A2>::value,
	typename __ewl_promote<_A1, _A2>::type
>::type 
_EWL_MATH_CDECL remainder(_A1 x, _A2 y)
{
	typedef typename __ewl_promote<_A1, _A2>::type r_type;
	char test[!(__ewl_is_same<_A1, r_type>::value && __ewl_is_same<_A2, r_type>::value)];
	(void)test;
	return remainder((r_type)x, (r_type)y);
}

template <class _A1, class _A2>
inline
typename __ewl_enable_if<
	__ewl_is_arithmetic<_A1>::value && __ewl_is_arithmetic<_A2>::value,
	typename __ewl_promote<_A1, _A2>::type
>::type 
_EWL_MATH_CDECL remquo(_A1 x, _A2 y, int* z)
{
	typedef typename __ewl_promote<_A1, _A2>::type r_type;
	char test[!(__ewl_is_same<_A1, r_type>::value && __ewl_is_same<_A2, r_type>::value)];
	(void)test;
	return remquo((r_type)x, (r_type)y, z);
}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL rint(_A1 x) {return rint((double)x);}


#if !_EWL_USES_SUN_MATH_LIB

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL round(_A1 x) {return round((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL scalbln(_A1 x, long y) {return scalbln((double)x, y);}

#endif  /* !_EWL_USES_SUN_MATH_LIB */

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL scalbn(_A1 x, int y) {return scalbn((double)x, y);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL sin(_A1 x) {return sin((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL sinh(_A1 x) {return sinh((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL sqrt(_A1 x) {return sqrt((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL tan(_A1 x) {return tan((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL tanh(_A1 x) {return tanh((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL tgamma(_A1 x) {return tgamma((double)x);}

template <class _A1>
inline
typename __ewl_enable_if<__ewl_is_integral<_A1>::value, double>::type 
_EWL_MATH_CDECL trunc(_A1 x) {return trunc((double)x);}


_EWL_END_NAMESPACE_STD

#endif /* _EWL_FLOATING_POINT ... */

#endif  /* _EWL_MATH_INTEGRAL_H */
