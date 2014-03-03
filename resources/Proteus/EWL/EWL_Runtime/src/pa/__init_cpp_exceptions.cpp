/***************************************************************************/
/*

FILE
	__init_cpp_exceptions.cpp
	
	$Date: 2012/06/01 16:06:51 $
	$Revision: 1.1 $ 

DESCRIPTION

	Runtime file.  Contains calls to initialize c++ exceptions.  Contents
	will be included only if your project has exceptions.


COPYRIGHT	
	(c) 2002-2006 Freescale Semiconductor, Inc.
	All rights reserved.

HISTORY
	99 JAN 22 MEA	Created.

*/
/***************************************************************************/

#if __MWERKS__
#pragma exceptions off
#pragma internal on
#endif

#include <__ppc_eabi_init.h>
#include <__ppc_eabi_linker.h>		/* linker-generated symbol declarations */
#include <NMWException.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void __init_cpp_exceptions(void);
extern void __fini_cpp_exceptions(void);
extern void suspend(void);

#ifdef __cplusplus
}
#endif

static int fragmentID = -2;			/* ID given to fragment by exception-handling		*/
									/* valid numbers are 0 - MAX_INT; if -1 then		*/
									/* too many fragments were loaded					*/

/***************************************************************************/
/*
 *	__exception_info_constants
 *
 *	Called from __init_cpp_exceptions.cpp
 *
 *	info		address of linker generated exception table info
 *	R2			address of variable to hold the value in r2
 *
 *	we now overload the function in the event that the user wants pic pid
 *
 */
/***************************************************************************/
extern void __exception_info_constants(void **info, char **R2)
{
	register char *temp;				/* r2 register contents								*/

	asm {
		mr      temp,r2
	}
	*R2 = temp;
	
	*info = _eti_init_info;
}

/***************************************************************************/
/*
 *	__find_exception_addresses
 *
 *	Called from ExceptionPPC.cp
 *
 *	info		address of linker generated exception table info
 *	returnaddr	address within a code segment
 *	returnaddr	address within a code segment
 *	ex_start	address of variable to hold address of exception table info
 *	ex_end		address of variable to hold end address of exception table info
 *
 *	we now overload the function in the event that the user wants pic pid
 *
 */
/***************************************************************************/
extern int __find_exception_addresses(void *info, char *returnaddr, void **ex_start, void **ex_end)
{
	__eti_init_info *eti_info = (__eti_init_info*)info;
	
	while (1) {
		if (eti_info->code_size == 0) break;
		if(returnaddr>=eti_info->code_start && 
					returnaddr<(char*)eti_info->code_start+eti_info->code_size) {
			*ex_start = eti_info->eti_start;
			*ex_end = eti_info->eti_end;
			return(1);
		}
 		eti_info++;
	}
	
	return(0);
}


extern void __init_cpp_exceptions(void)
{
	char *R2;
	void *info;
	 
	if (fragmentID == -2) {
	/* use suspend with some OSes */
#ifdef TERMINATE_WITH_SUSPEND
		set_terminate(suspend);
#endif

		/* the variable _eti_init_info is a table with a row for each code segment
		 * that contains functions that have exception info.  _eti_init_info holds
		 * exception info for only one process.  register r2 and the (possible
		 * runtime/linktime delta are constant for each process.
		 */
		__exception_info_constants(&info, &R2);

		/*
		 *	initialize exception tables
		 */
		fragmentID = __register_fragment(info, R2);
	}
}

extern void __fini_cpp_exceptions(void)
{
	if (fragmentID != -2) {
		__unregister_fragment(fragmentID);
		fragmentID = -2;
	}
}

#if __MWERKS__ && __PPC_EABI__
#pragma section ".ctors$10"
__declspec(section ".ctors$10") 
	extern void * const __init_cpp_exceptions_reference = __init_cpp_exceptions;
#if __dest_os != __eppc_vxworks
#pragma section ".dtors$10"
__declspec(section ".dtors$10") 
	extern void * const __destroy_global_chain_reference = __destroy_global_chain;
#pragma section ".dtors$15"
__declspec(section ".dtors$15") 
	extern void * const __fini_cpp_exceptions_reference = __fini_cpp_exceptions;
#endif	/* __dest_os != __eppc_vxworks */
#endif	/* __MWERKS__ && __PPC_EABI__ */
