/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:00:34 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *
 *	towctrans
 *	wctrans
 *
 */

#include <ansi_parms.h>

#if _EWL_WIDE_CHAR
#if _EWL_C99

#include <wctrans.h>
#include <string.h>
#include <wctype.h>

#include <ewl_misra_types.h>

static const struct wctable
{
	const char_t *string;
	wctype_t	value;
} wtable[] =
{
	{"tolower", (wctype_t)1},
	{"toupper", (wctype_t)2},
	{(const char_t *)0, (wctype_t)0}
};

wint_t _EWL_CDECL towctrans(wint_t c, wctrans_t value)
{
	if (value == (wctrans_t)1u) {
		MISRA_EXCEPTION_RULE_14_7()
		return towlower(c);
	} else {
		MISRA_EXCEPTION_RULE_14_7()
		return towupper(c);
	}
}

wctrans_t _EWL_CDECL wctrans(const char_t *name)
{
	int_t	i;

	for(i=0; wtable[i].string != 0; ++i) {
		if(strcmp(wtable[i].string, name) == 0) {
			MISRA_EXCEPTION_RULE_14_7()
			return (wtable[i].value);
		}
	}
	return (wctrans_t)0;
}

#endif /* _EWL_C99 */
#endif /* _EWL_WIDE_CHAR */

