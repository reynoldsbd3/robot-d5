/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:38 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_T_H
#define _EWL_T_H

#ifdef _INT32
	#undef _INT32
#endif

#ifdef _UINT32
	#undef _UINT32
#endif

#ifndef _EWL_INT32_TYPE
	#define _EWL_INT32_TYPE long
#endif

typedef _EWL_INT32_TYPE _INT32;
typedef unsigned _EWL_INT32_TYPE _UINT32;


#if _EWL_LONGLONG
	#ifdef _INT64
		#undef _INT64
	#endif

	#ifdef _UINT64
		#undef _UINT64
	#endif

	#ifndef _EWL_INT64_TYPE
		#define _EWL_INT64_TYPE long long
	#endif

	typedef _EWL_INT64_TYPE _INT64;
	typedef unsigned _EWL_INT64_TYPE _UINT64;
#endif /*_EWL_LONGLONG */

#endif /* _EWL_T_H */
