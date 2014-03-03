/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:38 $
 * $Revision: 1.1 $
 */
 
#ifndef _EWL_ISO646_H
#define _EWL_ISO646_H

#include <ansi_parms.h>

#if _EWL_C99

#if !_EWL_USING_CW_C_HEADERS_
	#if !__MACH__
	#error You must have the non-EWL C header file access path before the EWL access path
	#endif
#else

#include <ciso646>

#endif /* _EWL_USING_CW_C_HEADERS_ */

#endif /* _EWL_C99 */

#endif /* _EWL_ISO646_H */
