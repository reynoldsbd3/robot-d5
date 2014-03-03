/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:42:24 $
 * $Revision: 1.2 $
 */

#ifndef _STREAMBUF_H   
#define _STREAMBUF_H

#include <streambuf>

#ifndef _EWL_NO_CPP_NAMESPACE
#ifndef _EWL_NO_IO
	using std::basic_streambuf;
	using std::streambuf;
#ifndef _EWL_NO_WCHART_CPP_SUPPORT
	using std::wstreambuf;
#endif
#endif  // _EWL_NO_IO
#endif  // _EWL_NO_CPP_NAMESPACE 
#endif  // _STREAMBUF_H   
