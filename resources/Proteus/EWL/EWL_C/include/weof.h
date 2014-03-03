/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_WEOF_H
#define _EWL_WEOF_H

#if !_EWL_USING_CW_C_HEADERS_
	#error You must have the non-EWL C header file access path before the EWL access path
#else

#if _EWL_WIDE_CHAR

#include <wchar_t.h>

#ifndef _EWL_WEOF
	#define _EWL_WEOF ((__std(wint_t))(-1))
#endif

#ifndef WEOF
	#define WEOF _EWL_WEOF
#endif /* WEOF */

#endif /* _EWL_WIDE_CHAR */

#endif /* _EWL_USING_CW_C_HEADERS_ */

#endif /* _EWL_WEOF_H */
