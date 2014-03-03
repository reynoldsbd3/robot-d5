/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:50:18 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		strxfrm
 */

#include <ansi_parms.h>
_MISRA_EXCEPTION_RULE_19_6()
#undef __STDC_WANT_LIB_EXT1__
_MISRA_RESTORE()
#define __STDC_WANT_LIB_EXT1__ 1

#include <ewl_misra_types.h>

#include <string.h>

#if !(defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE)
#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <locale_api.h>
#include <stdlib.h>
#include <ewl_thread_local_data.h>
#endif

MISRA_ALLOW_POINTER_CASTS()

MISRA_QUIET_LINT_04()
size_t _EWL_CDECL strxfrm(char_t * _EWL_RESTRICT str1, const char_t * _EWL_RESTRICT str2, size_t n)
{
#if defined(_EMBEDDED_WARRIOR_HAS_NO_LOCALE) && _EMBEDDED_WARRIOR_HAS_NO_LOCALE

	size_t reslen = strlen(str2);
	if (str1 != NULL) {
		(void)strncpy(str1, str2, n);
	}
	return(reslen);

#else

	uchar_t * chptr,
			* collptr,
			* weight1,
			* weight2,
			* localstr1,
			* wt1ptr, *wt2ptr, *tempptr;
	struct __locale *    current_locale_ptr;
	int_t 	offset;
	int_t	tabsize;
	int_t 	i;
	uint16_t ch;
	int_t	chrval;
	size_t  reslen;
	int_t	lenstr2 = (int_t)strlen(str2);

	if (lenstr2 == 0) {
		MISRA_EXCEPTION_RULE_14_7()
		return 0u;
	}

	MISRA_EXCEPTION_RULE_10_3()
	MISRA_EXCEPTION_RULE_20_4()
	localstr1 		   = (uchar_t*)malloc((size_t)(MB_LEN_MAX * (lenstr2+1)));
	current_locale_ptr = &_EWL_LOCALDATA(_current_locale);
	offset             = current_locale_ptr->coll_cmpt_ptr->char_start_value;
	tabsize            = current_locale_ptr->coll_cmpt_ptr->char_coll_tab_size;
	chptr              = (uchar_t*)str2;
	MISRA_EXCEPTION_RULE_20_4()
	weight1            = (uchar_t*)malloc((size_t)lenstr2);
	MISRA_EXCEPTION_RULE_20_4()
	weight2            = (uchar_t*)malloc((size_t)lenstr2);

	MISRA_EXCEPTION_RULE_20_4()
	if ((localstr1 == 0L) || (weight1 == 0L) || (weight2 == 0L)) {
		/* malloc failure */
		if (localstr1) { free(localstr1); }		
		if (weight1) { free(weight1); }		
		if (weight2) { free(weight2); }		
		MISRA_EXCEPTION_RULE_14_7()
		return 0u;
	}
	collptr            = localstr1;
	
	MISRA_EXCEPTION_RULE_17_3()
	for (wt1ptr = weight1; wt1ptr < (weight1 + lenstr2); wt1ptr++) {
		*wt1ptr = (uchar_t)'\0';
	}
	MISRA_EXCEPTION_RULE_17_3()
	for (wt2ptr = weight2; wt2ptr < (weight2 + lenstr2); wt2ptr++) {
		*wt2ptr = (uchar_t)'\0';
	}
	wt1ptr = weight1;
	wt2ptr = weight2;

	for (i = 0; i < lenstr2; i++) {
		chrval = (int_t)*chptr++;
		if ((chrval >= offset) && (chrval <= (offset + tabsize))) {
			ch = *(current_locale_ptr->coll_cmpt_ptr->char_coll_table_ptr + (chrval - offset));
			if (ch != 0u) {
				*collptr++ = (uchar_t)(ch & 0x00ffu);
				if ((ch & 0x0f00u) != 0u) {
					*wt1ptr++ = (uchar_t)((ch & 0x0f00u) >> 8);
				}
				if ((ch & 0xf000u) != 0u) {
					*wt2ptr++ = (uchar_t)((ch & 0xf000u) >> 12);
				}
			}
		}
	}
	if (current_locale_ptr->coll_cmpt_ptr->char_spec_accents) {
		MISRA_EXCEPTION_RULE_17_3()
		for (tempptr = wt1ptr - 1; tempptr >= weight1; tempptr--) {
			*collptr++ = *tempptr;
		}
	} else {
		MISRA_EXCEPTION_RULE_17_3()
		for (tempptr = weight1; tempptr < wt1ptr; tempptr++) {
			*collptr++ = *tempptr;
		}
	}

	MISRA_EXCEPTION_RULE_17_3()
	for (tempptr = weight2; tempptr < wt2ptr; tempptr++) {
		*collptr++ = *tempptr;
	}

	*collptr = (uchar_t)'\0';
	MISRA_EXCEPTION_RULE_20_4()
	free(weight1);
	MISRA_EXCEPTION_RULE_20_4()
	free(weight2);
	reslen = strlen((const char_t *)localstr1);
	if (str1 != NULL) {
		strncpy(str1, (const char_t *)localstr1, n);
	}
	MISRA_EXCEPTION_RULE_20_4()
	free(localstr1);
	return(reslen);
#endif /* _EMBEDDED_WARRIOR_HAS_NO_LOCALE */
}

