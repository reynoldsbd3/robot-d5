/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:42:03 $
 * $Revision: 1.2 $
 */

#ifndef _ISTREAM_H
#define _ISTREAM_H

#include <istream>

#ifndef _EWL_NO_CPP_NAMESPACE 
#ifndef _EWL_NO_IO
	using std::basic_istream;
	using std::istream;
	using std::basic_iostream;
	using std::iostream;
#ifndef _EWL_NO_WCHART_CPP_SUPPORT
	using std::wistream;
	using std::wiostream;
	using std::ws;
#endif  // _EWL_NO_WCHART_CPP_SUPPORT
#endif  // _EWL_NO_IO
#endif  // _EWL_NO_CPP_NAMESPACE 

#endif  // _ISTREAM_H
