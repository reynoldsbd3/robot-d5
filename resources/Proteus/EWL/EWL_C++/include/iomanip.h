/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:41:57 $
 * $Revision: 1.2 $
 */

#ifndef _IOMANIP_H 
#define _IOMANIP_H

#include <iomanip>

#ifndef _EWL_NO_CPP_NAMESPACE     
#ifndef _EWL_NO_IO
	using std::resetiosflags;
	using std::setiosflags;
	using std::setbase;
	using std::setfill;
	using std::setprecision;
	using std::setw;
#endif  // _EWL_NO_IO
#endif  // _EWL_NO_CPP_NAMESPACE      

#endif  // _IOMANIP_H       
