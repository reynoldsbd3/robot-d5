/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_STDBOOL_H
#define _EWL_STDBOOL_H

#include <ansi_parms.h>

#if !_EWL_USING_CW_C_HEADERS_
	#error You must have the non-EWL C header file access path before the EWL access path
#else

#ifndef __cplusplus

#if !_EWL_C99
	#warning "EWL support for C99 is not enabled"
#else
	#if defined(__CWCC__)
		#if !__option(c99) 
			#pragma c99 on
			
			#pragma ANSI_strict off
				#warning "Compiler support for C99 has been turned on"
			#pragma ANSI_strict reset
		#endif /* !__option(c99) */
	#endif /* __CWCC__ */
	
	#define bool							_Bool
	#define true							1
	#define false							0
	#define __bool_true_false_are_defined	1
#endif /* _EWL_C99 */

#else /* __cplusplus */

#ifdef __CWCC__
	#if !__option(bool)

		#ifndef bool
			#ifndef _EWL_BOOL_TYPE
				#define _EWL_BOOL_TYPE unsigned char
			#endif
			
			typedef _EWL_BOOL_TYPE bool;
		#endif
		#ifndef true
			#define false static_cast<bool>(0)
			#define true  static_cast<bool>(1)
		#endif

	#endif /* !__option(bool) */

	#define __bool_true_false_are_defined	1

#endif /* __CWCC__ */

#endif /* !__cplusplus */

#endif /* _EWL_USING_CW_C_HEADERS_ */

#endif /* _EWL_STDBOOL_H */
