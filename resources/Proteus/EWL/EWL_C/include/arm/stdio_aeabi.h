#ifndef _EWL_STDIO_AEABI
#define _EWL_STDIO_AEABI

#include <ansi_parms.h>

#if !_EWL_USING_CW_C_HEADERS_
	#error You must have the non-EWL C header file access path before the EWL access path
#else

/* mandatory portatibility declaration C Library ABI for ARM architecture section 5.1.1 */
#if defined(_AEABI_PORTABILITY_LEVEL) && !defined(_AEABI_PORTABLE)
#define _AEABI_PORTABLE 1
#endif

#endif /*end of _EWL_USING_CW_C_HEADERS_ */ 

#endif /* end of _EWL_STDIO_AEABI*/
