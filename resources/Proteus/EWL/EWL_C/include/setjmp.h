/* EWL
 * Copyright � 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_SETJMP_H
#define _EWL_SETJMP_H

#include <ansi_parms.h>

#if !_EWL_USING_CW_C_HEADERS_
	#error You must have the non-EWL C header file access path before the EWL access path
#else

#include <csetjmp>

#if defined(__cplusplus) && defined(_EWL_USING_NAMESPACE)
	using std::jmp_buf;
	using std::longjmp;
#endif /* defined(__cplusplus) && defined(_EWL_USING_NAMESPACE) */

#endif /* _EWL_USING_CW_C_HEADERS_ */

#endif /* _EWL_SETJMP_H */
