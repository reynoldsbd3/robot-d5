/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:37 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_ASSERT_API_H
#define	_EWL_ASSERT_API_H

_EWL_BEGIN_EXTERN_C

#ifndef _EWL_ASSERT_DISPLAYS_FUNC
	#ifdef __CWCC__
		#if __option(c99)
			#define _EWL_ASSERT_DISPLAYS_FUNC 1
		#else
			#define _EWL_ASSERT_DISPLAYS_FUNC 0
		#endif
	#else
		#define _EWL_ASSERT_DISPLAYS_FUNC 1
	#endif
#endif


#if (__dest_os == __win32_os) && defined(UNICODE)
	_EWL_IMP_EXP_C void _EWL_CDECL __ewl_wchar_assertion_failed(wchar_t const *, char const *, char const *, int) _EWL_CANT_THROW;
#endif

	_EWL_IMP_EXP_C void _EWL_CDECL __ewl_assertion_failed(char const *, char const *, char const *, int) _EWL_CANT_THROW;

_EWL_END_EXTERN_C

#endif /* _EWL_ASSERT_API_H */
