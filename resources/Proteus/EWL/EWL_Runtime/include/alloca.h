/* CodeWarrior Runtime Support Library
 * Copyright © 1995-2006 Freescale Semiconductor, Inc.  All rights reserved.
 *
 * $Date: 2012/07/06 06:39:42 $
 * $Revision: 1.2 $
 */


#ifndef _ALLOCA
#define	_ALLOCA

#ifdef __CWCC__
	#define	alloca(x)	__alloca(x)
#elif defined(__GNUC__)
	#define alloca(x) __builtin_alloca(x)
#else
	#define	alloca(x)	__alloca(x)
#endif /*__CWCC__*/

#endif
