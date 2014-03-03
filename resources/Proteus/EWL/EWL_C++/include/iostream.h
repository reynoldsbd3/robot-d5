/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:42:02 $
 * $Revision: 1.2 $
 */

#ifndef _IOSTREAM_H
#define _IOSTREAM_H

#include <iostream>
#include <ostream.h>
#include <istream.h>
#include <streambuf.h>
#include <ios.h>

#ifndef _EWL_NO_CPP_NAMESPACE
#ifndef _EWL_NO_IO
	using std::cin;
	using std::cout;
	using std::cerr;
	using std::clog;
#ifndef _EWL_NO_WCHART_CPP_SUPPORT
	using std::wcin;
	using std::wcout;
	using std::wcerr;
	using std::wclog;
#endif  // _EWL_NO_WCHART_CPP_SUPPORT
#endif  // _EWL_NO_IO
#endif  // _EWL_NO_CPP_NAMESPACE  

#endif  // _IOSTREAM_H 
