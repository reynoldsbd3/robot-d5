/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:47:06 $
 * $Revision: 1.1 $
 */

#include <ansi_parms.h>

#if defined(_AEABI_PORTABILITY_LEVEL) && (_AEABI_PORTABILITY_LEVEL != 0)

#include <locale.h>
#include <limits.h>
#include <ewl_misra_types.h>

const int_t 	__aeabi_LC_COLLATE  = 0x01;
const int_t 	__aeabi_LC_CTYPE    = 0x02;
const int_t 	__aeabi_LC_MONETARY = 0x04;
const int_t 	__aeabi_LC_NUMERIC  = 0x08;
const int_t 	__aeabi_LC_TIME     = 0x10;
const int_t 	__aeabi_LC_ALL      = 0x1F;

static const char_t nullstring[] = "";

MISRA_EXCEPTION_RULE_11_4()
struct __aeabi_lconv  __aeabi_lconv =
{
	"."			 	   /* decimal_point	    */,
	(char_t *)nullstring /* thousands_sep		*/,
	(char_t *)nullstring /* grouping			*/,
	(char_t *)nullstring /* int_curr_symbol		*/,
	(char_t *)nullstring /* currency_symbol	*/,
	(char_t *)nullstring /* mon_decimal_point	*/,
	(char_t *)nullstring /* mon_thousands_sep	*/,
	(char_t *)nullstring /* mon_grouping		*/,
	(char_t *)nullstring /* positive_sign		*/,
	(char_t *)nullstring /* negative_sign		*/,
	CHAR_MAX	 /* int_frac_digits		*/,
	CHAR_MAX	 /* frac_digits			*/,
	CHAR_MAX	 /* p_cs_precedes		*/,
	CHAR_MAX	 /* p_sep_by_space		*/,
	CHAR_MAX	 /* n_cs_precedes		*/,
	CHAR_MAX	 /* n_sep_by_space		*/,
	CHAR_MAX	 /* p_sign_posn			*/,
	CHAR_MAX	 /* n_sign_posn			*/,
	CHAR_MAX 	 /* int_p_cs_precedes	*/,
	CHAR_MAX 	 /* int_n_cs_precedes	*/,
	CHAR_MAX 	 /* int_p_sep_by_space	*/,
	CHAR_MAX 	 /* int_n_sep_by_space	*/,
	CHAR_MAX 	 /* int_p_sign_posn		*/,
	CHAR_MAX 	 /* int_n_sign_posn		*/
};

struct __aeabi_lconv *(__aeabi_localeconv)(void)
{
   return &__aeabi_lconv;
} /* localeconv */

#endif /* _AEABI_PORTABILITY_LEVEL */

/* __aeabi_localeconv() is defined.
 * __aeabi_LC_COLLATE,__aeabi_LC_CTYPE,__aeabi_LC_MONETARY,__aeabi_LC_NUMERIC,
 * __aeabi_LC_TIME,__aeabi_LC_ALL are defined for _AEABI_PORTABILITY_LEVEL macro
 */
