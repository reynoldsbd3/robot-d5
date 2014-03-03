/* CodeWarrior Runtime Library
 * Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
 *
 * Purpose:  Implementation of the portable Itanium C++ ABI
 *           see http://www.codesourcery.com/cxx-abi/
 *
 * $Date: 2012/06/01 16:06:35 $
 * $Revision: 1.1 $
 */

#include <cxxabi.h>
#include <exception>

extern "C" void __cxa_pure_virtual()
{
	std::terminate();
}
