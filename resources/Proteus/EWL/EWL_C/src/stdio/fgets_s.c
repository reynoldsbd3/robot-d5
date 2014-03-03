/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:35 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		gets_s
 *
 */

#include <ansi_parms.h>

#include <stdlib.h>
#include <char_io.h>
#include <critical_regions.h>

#include <ewl_misra_types.h>

#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__
char_t * _EWL_CDECL gets_s(char_t * s, rsize_t n)
{
	char_t *	p = s;
	int_t			c;
	MISRA_EXCEPTION_RULE_11_4()
	struct _FILE *	file = (struct _FILE *)stdin;

	if ((n < 1U) || (s == NULL) || (n > (rsize_t)RSIZE_MAX)) {
		__ewl_runtime_constraint_violation_s(NULL, NULL, -1);
		if ((s != NULL) && (n > 0U) && (n <= (rsize_t)RSIZE_MAX)) {
			s[0] = 0;
		}
		__begin_critical_region(stdin_access);
		for (;;) {
			c = __getc((struct _FILE*)file);
			if ((c == (int_t)EOF) || (c == '\n')) {
				break;
			}
		}
		__end_critical_region(stdin_access);
		MISRA_EXCEPTION_RULE_14_7()
		return NULL;
	}

  	__begin_critical_region(stdin_access);

	MISRA_EXCEPTION_RULE_14_6()
	for (;n;n--) {
		c = __getc(file);

		if (c == (int_t)EOF) {
			if (file->state.eof && (p != s)) {
				break;
			} else {
  				__end_critical_region(stdin_access);
				MISRA_EXCEPTION_RULE_14_7()
				return(NULL);
			}
		} else if (c == '\n') {
			break;
		} else if (n == 1U) {
			MISRA_EXCEPTION_RULE_11_4()
			(void)__ungetc(c, (FILE *)file);
			break;
		} else {}

		*p++ = (char_t)c;
	}
  	__end_critical_region(stdin_access);

	*p = 0;

	return(s);
}
#endif /* defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ */
