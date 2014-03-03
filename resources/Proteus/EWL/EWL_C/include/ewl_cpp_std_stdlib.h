/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:38 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_CPP_STD_STDLIB_H
#define _EWL_CPP_STD_STDLIB_H

#ifdef __cplusplus

namespace std {

	using ::size_t;
	using ::abort;
	using ::atexit;
	using ::exit;
#if _EWL_C99
#if _EWL_OS_X_VERSION > 3
	using ::_Exit;
#endif /* _EWL_OS_X_VERSION > 3 */
#endif /* _EWL_C99 */
	using ::getenv;
	using ::system;
	using ::malloc;
	using ::calloc;
	using ::realloc;
	using ::free;
	using ::atol;
#if _EWL_LONGLONG
#if _EWL_C99
#if _EWL_OS_X_VERSION > 3
	using ::atoll;
#endif /* _EWL_OS_X_VERSION > 3 */
#endif /* _EWL_C99 */
#endif /* _EWL_LONGLONG */
#ifndef _EWL_NO_MATH_LIB
	using ::atof;
	using ::strtod;
#if _EWL_OS_X_VERSION > 3
	using ::strtold;
#if _EWL_C99
	using ::strtof;
#endif /* _EWL_C99 */
#endif /* _EWL_OS_X_VERSION > 3 */
#endif /* _EWL_NO_MATH_LIB */
	using ::atoi;
	using ::strtol;
	using ::strtoul;
#if _EWL_LONGLONG
#if _EWL_C99
#if _EWL_OS_X_VERSION > 1
	using ::strtoll;
	using ::strtoull;
#endif /* _EWL_OS_X_VERSION > 1 */
#endif /* _EWL_C99 */
#endif /* _EWL_LONGLONG */
#if _EWL_WIDE_CHAR
	using ::mblen;
	using ::mbstowcs;
	using ::mbtowc;
	using ::wctomb;
	using ::wcstombs;
#endif /* _EWL_WIDE_CHAR */
	using ::bsearch;
	using ::qsort;
	using ::div_t;
	using ::ldiv_t;
	using ::abs;
	using ::div;
	using ::labs;
#if _EWL_LONGLONG
#if _EWL_C99
#if _EWL_OS_X_VERSION > 3
	using ::llabs;
	using ::lldiv_t;
	using ::lldiv;
#endif /* _EWL_OS_X_VERSION > 3 */
#endif /* _EWL_C99 */
#endif /* _EWL_LONGLONG */
	using ::ldiv;
	using ::srand;
	using ::rand;
#if 0 /* Do not include AltiVec extensions when using non-EWL C */
#if __VEC__
	using ::vec_calloc;
	using ::vec_free;
	using ::vec_malloc;
	using ::vec_realloc;
#endif /* __VEC__ */
#endif /* 0 */
#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
	using ::errno_t;
	using ::rsize_t;
	using ::getenv_s;
	using ::bsearch_s;
	using ::qsort_s;
#if _EWL_WIDE_CHAR
	using ::wctomb_s;
	using ::mbstowcs_s;
	using ::wcstombs_s;
#endif /* _EWL_WIDE_CHAR */
#endif /* __STDC_WANT_LIB_EXT1__ */

} /* namespace std */

#endif /* __cplusplus */

#endif /* _EWL_CPP_STD_STDLIB_H */
