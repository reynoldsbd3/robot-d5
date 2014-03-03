/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:42:21 $
 * $Revision: 1.2 $
 */

#ifndef _SSTREAM_H   
#define _SSTREAM_H

#include <sstream>

#ifndef _EWL_NO_CPP_NAMESPACE
#ifndef _EWL_NO_IO
	using std::basic_stringbuf;
	using std::stringbuf;
	using std::basic_istringstream;
	using std::istringstream;
	using std::basic_ostringstream;
	using std::ostringstream;
	using std::basic_stringstream;
	using std::stringstream;
#ifndef _EWL_NO_WCHART_CPP_SUPPORT
	using std::wstringbuf;
	using std::wistringstream;
	using std::wostringstream;
	using std::wstringstream;
#endif  // _EWL_NO_WCHART_CPP_SUPPORT
#endif  // _EWL_NO_IO
#endif  // _EWL_NO_CPP_NAMESPACE       
#endif  // _SSTREAM_H   
