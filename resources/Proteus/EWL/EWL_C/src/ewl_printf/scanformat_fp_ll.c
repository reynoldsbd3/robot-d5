#define __sformatter __sformatter_FPLL

#ifdef __STDC_WANT_LIB_EXT1__
#undef __STDC_WANT_LIB_EXT1__
#endif

#define __STDC_WANT_LIB_EXT1__ 0    // securelib

#define _EWL_FLOATING_POINT_PRINTF_SCANF 1
#define _EWL_LONGLONG_PRINTF_SCANF   	   1
#define _EWL_USES_SUN_MATH_LIB 1

#include <stdio/__scanformat.c>
