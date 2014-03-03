/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:39 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_WCTYPE_API_H
#define _EWL_WCTYPE_API_H

#include <ansi_parms.h>

#if _EWL_WIDE_CHAR

	#include <eof.h>
	#include <weof.h>
	#include <wchar_t.h>
	#include <wctrans.h>

	_EWL_BEGIN_EXTERN_C

		#if !_EWL_C_LOCALE_ONLY
			#define __ewl_wcmap_size 256
		#else
			#define __ewl_wcmap_size 128
		#endif

		#if !_EWL_C_LOCALE_ONLY
			extern _EWL_IMP_EXP_C const unsigned short __ewl_wctype_map[__ewl_wcmap_size];
			extern _EWL_IMP_EXP_C const wchar_t __wlower_map[__ewl_wcmap_size];
			extern _EWL_IMP_EXP_C const wchar_t __wupper_map[__ewl_wcmap_size];
		#endif /* _EWL_C_LOCALE_ONLY */

		extern _EWL_IMP_EXP_C const unsigned short __wctype_mapC[__ewl_wcmap_size];
		extern _EWL_IMP_EXP_C const wchar_t __wlower_mapC[__ewl_wcmap_size];
		extern _EWL_IMP_EXP_C const wchar_t __wupper_mapC[__ewl_wcmap_size];

	_EWL_END_EXTERN_C

	#define __ewl_walpha	0x0001u
	#define __ewl_wblank	0x0002u
	#define __ewl_wcntrl	0x0004u
	#define __ewl_wdigit	0x0008u
	#define __ewl_wgraph	0x0010u
	#define __ewl_wlower	0x0020u
	#define __ewl_wprint	0x0040u
	#define __ewl_wpunct	0x0080u
	#define __ewl_wspace	0x0100u
	#define __ewl_wupper	0x0200u
	#define __ewl_wxdigit	0x0400u

	#define __ewl_walnum	(__ewl_walpha | __ewl_wdigit)

#endif /* _EWL_WIDE_CHAR */

#endif /* _EWL_WCTYPE_API_H */
