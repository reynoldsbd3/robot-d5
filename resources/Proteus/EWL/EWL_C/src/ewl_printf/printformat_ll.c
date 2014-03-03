#define __pformatter __pformatterLL

#ifdef __STDC_WANT_LIB_EXT1__
#undef __STDC_WANT_LIB_EXT1__
#endif

#define __STDC_WANT_LIB_EXT1__ 0
#define _EWL_LONGLONG_PRINTF_SCANF       1
#define _EWL_FLOATING_POINT_PRINTF_SCANF 0

#include <stdio/__printformat.c>
