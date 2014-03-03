/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_THREAD_LOCAL_DATA_PTHREADS_H
#define _EWL_THREAD_LOCAL_DATA_PTHREADS_H

#include <ansi_parms.h>

#if _EWL_PTHREADS && _EWL_LOCALDATA_AVAILABLE
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <clocale>
#include <locale_api.h>

#pragma options align=native

_EWL_BEGIN_EXTERN_C

	typedef struct _ThreadLocalData 
	{
		struct 					_ThreadLocalData *next;
		int 					_errno;
		unsigned long int 		random_next;
		unsigned char *			strtok_dn;
		unsigned char *			strtok_ds;
		#if _EWL_OS_TIME_SUPPORT	
		struct __std(tm) 		gmtime_tm;
		struct __std(tm)    	localtime_tm;
		#endif
		char 					asctime_result[26];
		char					temp_name[L_tmpnam];
		char *					locale_name;
		struct __locale         _current_locale;
		#if !_EWL_C_LOCALE_ONLY
		struct __std(lconv)		__lconv;
		#endif
		#if ((__dest_os == __mac_os) || (__dest_os == __mac_os_x))
		short					MacOSErrNo;
		#endif
		
#if _EWL_WFILEIO_AVAILABLE
		wchar_t					wtemp_name[L_tmpnam];
#endif
	} _ThreadLocalData;
	
	_EWL_IMP_EXP_C int _EWL_CDECL __ewl_InitializeMainThreadData(void) _EWL_CANT_THROW;
	_EWL_IMP_EXP_C void _EWL_CDECL __ewl_DisposeAllThreadData(void) _EWL_CANT_THROW;
	
	_EWL_IMP_EXP_C _ThreadLocalData * _EWL_CDECL __ewl_GetThreadLocalData(void) _EWL_CANT_THROW;

_EWL_END_EXTERN_C

#pragma options align=reset
#endif /*_EWL_PTHREADS && _EWL_LOCALDATA_AVAILABLE */
#endif /* _EWL_THREAD_LOCAL_DATA_PTHREADS_H */
