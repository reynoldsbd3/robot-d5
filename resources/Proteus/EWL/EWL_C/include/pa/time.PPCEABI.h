/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:43:58 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_TIME_PPCEABI_H
#define _EWL_TIME_PPCEABI_H

#if _EWL_CLOCK_T_AVAILABLE
#if defined(_Generic_PPC_EABI_OS)
	#define _EWL_CLOCKS_PER_SEC 60
	#define _EWL_CLOCK_T unsigned long
#endif
#endif

#endif /* _EWL_TIME_PPCEABI_H */

/* Change record:
 * JWW 011101 New file to make time information platform independent
 */