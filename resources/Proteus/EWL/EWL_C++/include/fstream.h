/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:41:50 $
 * $Revision: 1.2 $
 */

#ifndef _FSTREAM_H
#define _FSTREAM_H

#include <fstream>

#ifndef _EWL_NO_CPP_NAMESPACE
#ifndef _EWL_NO_IO
	using std::basic_filebuf;
	using std::filebuf;
	using std::basic_ifstream;
	using std::ifstream;
	using std::basic_ofstream;
	using std::ofstream;
	using std::basic_fstream;
	using std::fstream;
#ifndef _EWL_NO_WCHART_CPP_SUPPORT
	using std::wfilebuf;
	using std::wifstream;
	using std::wofstream;
	using std::wfstream;
#endif  // _EWL_NO_WCHART_CPP_SUPPORT
#endif  // _EWL_NO_IO
#endif  // _EWL_NO_CPP_NAMESPACE

#endif  // _FSTREAM_H

