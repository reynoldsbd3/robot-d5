/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:37 $
 * $Revision: 1.1 $
 */

#ifdef __CWCC__
	#if __CWCC__ >= 0x3000
		#pragma notonce
	#endif /* __CWCC__ >= 0x3000 */
#endif /* __CWCC__ */

#include <ansi_parms.h>

#if !_EWL_USING_CW_C_HEADERS_
	#error You must have the non-EWL C header file access path before the EWL access path
#else
	#include <cassert>
#endif /* _EWL_USING_CW_C_HEADERS_ */
