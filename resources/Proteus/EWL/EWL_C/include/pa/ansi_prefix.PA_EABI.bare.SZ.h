/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:43:58 $
 * $Revision: 1.1 $
 */

#ifndef __ansi_prefix_SZ__
#define __ansi_prefix_SZ__

/************************************************/
/* 		Flags to reduce code and data size 		*/
/************************************************/

#ifndef _EWL_STRERROR_KNOWS_ERROR_NAMES
	#define _EWL_STRERROR_KNOWS_ERROR_NAMES	0	/* 0: for reduced code and data size by reducing MSL functionality */
#endif

#ifndef _EWL_ASSERT_DISPLAYS_FUNC
	#define _EWL_ASSERT_DISPLAYS_FUNC	0		/* 0: for reduced code and data size by reducing MSL functionality */
#endif

#ifndef _EWL_C_LOCALE_ONLY
	#define _EWL_C_LOCALE_ONLY	1				/* 1: for reduced code and data size by reducing MSL functionality */
#endif

#ifndef _EWL_C99
	#define _EWL_C99	0						/* 0: for reduced code and data size by reducing MSL functionality */
#endif

#ifndef _EWL_C99_PRINTF_SCANF
	#define _EWL_C99_PRINTF_SCANF	0			/* 0: for reduced code and data size by reducing MSL functionality */
#endif

#ifndef _EWL_C_HAS_CPP_EXTENSIONS
	#define _EWL_C_HAS_CPP_EXTENSIONS 1
#endif

#ifndef _EWL_FLOATING_POINT_IO
	#define _EWL_FLOATING_POINT_IO	0			/* 0: for reduced code and data size by reducing MSL functionality */
#endif

#include <pa/ansi_prefix.PA_EABI.bare.h>

#endif /* __ansi_prefix_SZ__ */
