/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:42:00 $
 * $Revision: 1.2 $
 */

#ifndef _IOSFWD_H   
#define _IOSFWD_H

#include <iosfwd>

#ifndef _EWL_NO_CPP_NAMESPACE    
	using std::char_traits;
	using std::allocator;
#ifndef _EWL_NO_IO
	using std::basic_ios;
	using std::basic_streambuf;
	using std::basic_istream;
	using std::basic_ostream;
	using std::basic_iostream;
	using std::basic_stringbuf;
	using std::basic_istringstream;
	using std::basic_ostringstream;
	using std::basic_stringstream;
	using std::basic_filebuf;
	using std::basic_ifstream;
	using std::basic_ofstream;
	using std::basic_fstream;
	using std::istreambuf_iterator;
	using std::ostreambuf_iterator;
	using std::ios;
	using std::streambuf;
	using std::istream;
	using std::ostream;
	using std::iostream;
	using std::stringbuf;
	using std::istringstream;
	using std::ostringstream;
	using std::stringstream;
	using std::filebuf;
	using std::ifstream;
	using std::ofstream;
	using std::fstream;
	using std::fpos;
	using std::streampos;
#ifndef _EWL_NO_WCHART_CPP_SUPPORT
	using std::wios;
	using std::wstreambuf;
	using std::wistream;
	using std::wostream;
	using std::wiostream;
	using std::wstringbuf;
	using std::wistringstream;
	using std::wostringstream;
	using std::wstringstream;
	using std::wfilebuf;
	using std::wifstream;
	using std::wofstream;
	using std::wfstream;
	using std::wstreampos;
#endif  // _EWL_NO_WCHART_CPP_SUPPORT
#endif  // _EWL_NO_IO
#endif  // _EWL_NO_CPP_NAMESPACE 

#endif  // _IOSFWD_H  
