/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:41:59 $
 * $Revision: 1.2 $
 */

#ifndef _IOS_H
#define _IOS_H

#include <ios>

#ifndef _EWL_NO_CPP_NAMESPACE
#ifndef _EWL_NO_IO
	using std::streamoff;
	using std::streamsize;
	using std::fpos;
	using std::ios_base;
	using std::basic_ios;
	using std::ios;
#ifndef _EWL_NO_WCHART_CPP_SUPPORT
	using std::wios;
#endif
	using std::boolalpha;
	using std::noboolalpha;
	using std::showbase;
	using std::noshowbase;
	using std::showpoint;
	using std::noshowpoint;
	using std::showpos;
	using std::noshowpos;
	using std::skipws;
	using std::noskipws;
	using std::uppercase;
	using std::nouppercase;
	using std::internal;
	using std::left;
	using std::right;
	using std::dec;
	using std::hex;
	using std::oct;
	using std::fixed;
	using std::scientific;
#endif  // _EWL_NO_IO
#endif  // _EWL_NO_CPP_NAMESPACE  

#endif  // _IOS_H    
