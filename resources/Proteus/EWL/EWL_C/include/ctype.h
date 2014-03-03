/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:37 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_CTYPE_H
#define _EWL_CTYPE_H

#include <ansi_parms.h>

#if !_EWL_USING_CW_C_HEADERS_
	#error You must have the non-EWL C header file access path before the EWL access path
#else

#include <cctype>

#if defined(__cplusplus) && defined(_EWL_USING_NAMESPACE)
	using std::isalnum;
	using std::isalpha;

#if _EWL_C99 || _EMBEDDED_WARRIOR_CTYPE
	using std::isblank;
#endif /* _EWL_C99 || _EMBEDDED_WARRIOR_CTYPE */

#if _EMBEDDED_WARRIOR_CTYPE
	using std::isascii;
	using std::toascii;
#endif /* _EMBEDDED_WARRIOR_CTYPE */

	using std::iscntrl;
	using std::isdigit;
	using std::isgraph;
	using std::islower;
	using std::isprint;
	using std::ispunct;
	using std::isspace;
	using std::isupper;
	using std::isxdigit;
	using std::tolower;
	using std::toupper;
#endif /* defined(__cplusplus) && defined(_EWL_USING_NAMESPACE) */

#endif /* _EWL_USING_CW_C_HEADERS_ */

#endif /* _EWL_CTYPE_H */
