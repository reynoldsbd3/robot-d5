/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:38 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_CPP_STD_INTTYPES_H 
#define _EWL_CPP_STD_INTTYPES_H

#if _EWL_C99

#ifdef __cplusplus

#if _EWL_OS_X_VERSION > 2

namespace std {

	using ::imaxdiv_t;
	using ::imaxabs;
	using ::imaxdiv;
	using ::strtoimax;
	using ::strtoumax;
#if _EWL_WIDE_CHAR
	using ::wcstoimax;
	using ::wcstoumax;
#endif /* _EWL_WIDE_CHAR */

} /* namespace std */

#endif /* _EWL_OS_X_VERSION > 2 */

#endif /* __cplusplus */

#endif /* _EWL_C99 */

#endif /* _EWL_CPP_STD_INTTYPES_H */

