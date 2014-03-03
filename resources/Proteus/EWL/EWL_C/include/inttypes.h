/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:38 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_INTTYPES_H
#define _EWL_INTTYPES_H

#include <ansi_parms.h>

#if !_EWL_USING_CW_C_HEADERS_
	#error You must have the non-EWL C header file access path before the EWL access path
#else

#include <cinttypes>
#include <stdint.h>

#if _EWL_C99

#if defined(__cplusplus) && defined(_EWL_USING_NAMESPACE)
	using std::imaxdiv_t;
	using std::imaxabs;
	using std::imaxdiv;
	using std::strtoimax;
	using std::strtoumax;
	#if _EWL_WIDE_CHAR
		using std::wcstoimax;
		using std::wcstoumax;
	#endif
#endif

#endif /* _EWL_C99 */
#endif /* _EWL_USING_CW_C_HEADERS_ */
#endif /* _EWL_INTTYPES_H */
