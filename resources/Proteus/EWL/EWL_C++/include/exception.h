/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:41:50 $
 * $Revision: 1.2 $
 */

// exception.h

#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <exception>

#ifndef _EWL_NO_CPP_NAMESPACE
	using std::exception;
	using std::bad_exception;
	using std::unexpected_handler;
	using std::set_unexpected;
	using std::unexpected;
#ifndef _EWL_NO_UNCAUGHT_EXCEPTION
	using std::uncaught_exception;
#endif
	using std::terminate_handler;
	using std::set_terminate;
	using std::terminate;
#endif  // _EWL_NO_CPP_NAMESPACE

#endif  // _EXCEPTION_H
