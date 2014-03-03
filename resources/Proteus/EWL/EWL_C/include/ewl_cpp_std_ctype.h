/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:38 $
 * $Revision: 1.1 $
 */
 
#ifndef _EWL_CPP_STD_CTYPE_H
#define _EWL_CPP_STD_CTYPE_H

#ifdef __cplusplus

namespace std {

#if _EWL_OS_X_VERSION > 1
	using ::isalnum;
	using ::isalpha;
#if _EWL_C99
	using ::isblank;
#endif /* _EWL_C99 */
	using ::iscntrl;
	using ::isdigit;
	using ::isgraph;
	using ::islower;
	using ::isprint;
	using ::ispunct;
	using ::isspace;
	using ::isupper;
	using ::isxdigit;
#endif /* _EWL_OS_X_VERSION > 1 */
	using ::tolower;
	using ::toupper;

} /* namespace std */

#endif /* __cplusplus */

#endif /* _EWL_CPP_STD_CTYPE_H */
