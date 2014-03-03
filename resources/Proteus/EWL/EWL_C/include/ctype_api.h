/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:39:37 $
 * $Revision: 1.1 $
 */

#ifndef _EWL_CTYPE_API_H
#define	_EWL_CTYPE_API_H

#include <ansi_parms.h>

#if (!_EMBEDDED_WARRIOR_CTYPE && !_EMBEDDED_WARRIOR_HAS_NO_LOCALE)
#include <locale_api.h>
#if _EWL_LOCALDATA_AVAILABLE
#include <ewl_thread_local_data.h>
#endif /* _EWL_LOCALDATA_AVAILABLE */
#endif /* (!_EMBEDDED_WARRIOR_CTYPE && !_EMBEDDED_WARRIOR_HAS_NO_LOCALE) */

#if _EMBEDDED_WARRIOR_CTYPE

_EWL_BEGIN_EXTERN_C

_EWL_END_EXTERN_C

	/** _EMBEDDED_WARRIOR_CTYPE supports 127 ASCII values */
	
	#define _UC_   0x01U          /* UPPER CASE      */
	#define _LC_   0x02U          /* LOWER CASE      */
	#define _NM_   0x04U          /* NUMERICAL       */
	#define _SP_   0x08U          /* SPACE           */
	#define _PU_   0x10U          /* PUNCTUATION     */
	#define _CL_   0x20U          /* CONTROL         */
	#define _HX_   0x40U          /* HEX DIGIT       */
	#define _BL_   0x80U          /* BLANK           */
	#define _MXC_  0x7fU          /* MAX ASCII VALUE */

	/* abbreviations for combinations of flags */

	#define _CS_ (_CL_ | _SP_)    /* control or blankspace    */
	#define _HN_ (_NM_ | _HX_)    /* number or hex digit      */
	#define _HU_ (_UC_ | _HX_)    /* upper case or hex digit  */
	#define _HL_ (_LC_ | _HX_)    /* lower case or hex digit  */
	#define _SS_ (_SP_ | _BL_)    /* the REAL blank character */

	/** _ctypes_ stores flags for the isXXXXX ctype routines  */
	extern const unsigned char _ctypes_[];

	#if (_EWL_C_LOCALE_ONLY || _EMBEDDED_WARRIOR_HAS_NO_LOCALE)
		#define _EWL_CMAP_ACCESS   _ctypes_
	#else
		#error sanity check
	#endif /* _EWL_C_LOCALE_ONLY */

#else

_EWL_BEGIN_EXTERN_C

	/** C99 ctype supports other locale when defined */
	#if !_EWL_C_LOCALE_ONLY
		#define __ewl_cmap_size 256
	#else
		#define __ewl_cmap_size 128
	#endif

	#if !_EWL_C_LOCALE_ONLY && !_EMBEDDED_WARRIOR_HAS_NO_LOCALE
		extern _EWL_IMP_EXP_C const unsigned short __ewl_ctype_map[__ewl_cmap_size];
		extern _EWL_IMP_EXP_C const unsigned char  __lower_map[__ewl_cmap_size];
		extern _EWL_IMP_EXP_C const unsigned char  __upper_map[__ewl_cmap_size];
	#endif /* _EWL_C_LOCALE_ONLY */

	/** __ctype_mapC holds the flags for isXXXX ctype routines */
	extern _EWL_IMP_EXP_C const unsigned short __ctype_mapC[__ewl_cmap_size];
	/** __lower_mapC holds the values for ctype tolower */
	extern _EWL_IMP_EXP_C const unsigned char  __lower_mapC[__ewl_cmap_size];
	/** __upper_mapC holds the values for ctype toupper */
	extern _EWL_IMP_EXP_C const unsigned char  __upper_mapC[__ewl_cmap_size];

_EWL_END_EXTERN_C

_EWL_BEGIN_NAMESPACE_STD

	#if (_EWL_C_LOCALE_ONLY || _EMBEDDED_WARRIOR_HAS_NO_LOCALE)
		#define _EWL_CMAP_ACCESS   __ctype_mapC
		#define _EWL_CLOWER_ACCESS __lower_mapC
		#define _EWL_CUPPER_ACCESS __upper_mapC
	#else
		#define _EWL_CMAP_ACCESS   _EWL_LOCALDATA(_current_locale).ctype_cmpt_ptr->ctype_map_ptr
		#define _EWL_CLOWER_ACCESS _EWL_LOCALDATA(_current_locale).ctype_cmpt_ptr->lower_map_ptr
		#define _EWL_CUPPER_ACCESS _EWL_LOCALDATA(_current_locale).ctype_cmpt_ptr->upper_map_ptr
	#endif /* _EWL_C_LOCALE_ONLY || _EMBEDDED_WARRIOR_HAS_NO_LOCALE */

_EWL_END_NAMESPACE_STD

	#define __ewl_alpha		0x0001U
	#define __ewl_blank		0x0002U
	#define __ewl_cntrl		0x0004U
	#define __ewl_digit		0x0008U
	#define __ewl_graph		0x0010U
	#define __ewl_lower		0x0020U
	#define __ewl_print		0x0040U
	#define __ewl_punct		0x0080U
	#define __ewl_space		0x0100U
	#define __ewl_upper		0x0200U
	#define __ewl_xdigit	0x0400U

	#define __ewl_alnum		(__ewl_alpha | __ewl_digit)

#endif /* _EMBEDDED_WARRIOR_CTYPE */

#endif /* _EWL_CTYPE_API_H */
