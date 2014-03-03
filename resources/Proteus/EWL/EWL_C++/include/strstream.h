/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:42:28 $
 * $Revision: 1.2 $
 */

#ifndef _STRSTREAM_H
#define _STRSTREAM_H

#include <strstream>

#ifndef _EWL_NO_CPP_NAMESPACE
#ifndef _EWL_NO_IO
	using std::strstreambuf;
	using std::istrstream;
	using std::ostrstream;
	using std::strstream;
#endif  // _EWL_NO_IO
#endif  // _EWL_NO_CPP_NAMESPACE

#endif  // _STRSTREAM_H
