/********************************************************************************************/
/*	Project...:	Standard C++ Library														*/
/*	Name......:	NewMore.cp																	*/
/*	Purpose...:	standard C++ library														*/
/*  Copyright.: Copyright © 1993-2006 Freescale Semiconductor, Inc.  All rights reserved.   */
/*  $Date: 2012/06/01 16:06:51 $                                                            */
/*  $Revision: 1.1 $                                                        				*/
/********************************************************************************************/

#if __MWERKS__ && !__embedded_cplusplus
#pragma exceptions on
#endif

#include <new>
#if __dest_os == __mac_os
#include <MacMemory.h>
#include <CPlusLib.h>
#else __PPC_EABI__
#include <MWMemory.h>
#include <MWCPlusLib.h>
#endif
#ifdef _EWL_NO_EXCEPTIONS
#include <stdio.h>
#include <stdlib.h>
#endif

#ifndef _EWL_NO_CPP_NAMESPACE        // hh 971207 Added namespace support
	namespace std {
#endif

char			__throws_bad_alloc = 1;		//	default: throw bad_alloc exception
new_handler		__new_handler;

/************************************************************************/
/*	Purpose..: 	throw a bad_alloc excpetion								*/
/*	Input....:	---														*/
/*	Return...:	---														*/
/************************************************************************/
extern void __throw_bad_alloc()
{
	if(__throws_bad_alloc) 
#ifndef _EWL_NO_EXCEPTIONS
		throw bad_alloc ();
#else
	{
#if	_EWL_OS_DISK_FILE_SUPPORT
		fprintf(stderr,"Could not allocate memory\n");
		exit(1);
#else
		abort();
#endif
	}
#endif
}

/************************************************************************/
/*	Purpose..: 	Set new_handler function								*/
/*	Input....:	new_handler function pointer (or NULL)					*/
/*	Return...:	old new_handler function pointer						*/
/************************************************************************/
extern new_handler set_new_handler(new_handler new_new_handler) _EWL_THROW
{
	new_handler old_new_handler = __new_handler;
	__new_handler = new_new_handler;
	return old_new_handler;
}

#ifndef _EWL_NO_CPP_NAMESPACE
	}
#endif

#if __dest_os == __mac_os

/************************************************************************/
/*	Purpose..: 	Allocate memory	(handle)								*/
/*	Input....:	size of memory to allocate								*/
/*	Return...:	handle to memory or 0L									*/
/************************************************************************/
void *__new_hdl(size_t size)
{
	void *ptr;

	for(;;)
	{
		if((ptr=NewHandle(size))!=NULL) break;
		if(!_STD::__new_handler)
		{
			_STD::__throw_bad_alloc();
			break;
		}
		_STD::__new_handler();
	}
	return ptr;
}

/************************************************************************/
/*	Purpose..: 	Dispose memory (handle)									*/
/*	Input....:	handle to memory or 0L (no action if 0L)				*/
/*	Return...:	---														*/
/************************************************************************/
void __del_hdl(void *hdl)
{
	if(hdl) DisposeHandle((Handle)hdl);
}

#endif /* __dest_os == __mac_os */

// hh  971207 Added namespace support
// bds 990322 ifdefed MacOS-specific code so this compiles on BeOS
// JWW 010531 Removed 68K conditional & synchronized Mach-O and PEF runtime code