/* EWL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:49:55 $
 * $Revision: 1.1 $
 */

/*
 *	Routines
 *	--------
 *		bsearch
 *		bsearch_s
 */

#include <ansi_parms.h>

#include <ewl_misra_types.h>
#include <stdlib.h>

MISRA_EXCEPTION_RULE_19_7()
#define table_ptr(i)	(((char_t *) table_base) + (member_size * (i)))
_MISRA_RESTORE()

MISRA_EXCEPTION_RULE_16_2()
void * _EWL_CDECL bsearch(const void * key,
							 const void * table_base,
							 size_t num_members,
							 size_t member_size,
							 _compare_function compare_members)
{
	size_t	l, r, m;
	int_t	c;
	char_t *mp;

	if (!key || !table_base || !num_members || !member_size || !compare_members) {
		MISRA_EXCEPTION_RULE_14_7()		
		return(NULL);
	}

	mp = (char_t *)table_base;
	c = compare_members(key, mp);							/* We have to make sure 'key' doesn't compare   */
															/* less than the first element in the table.    */
	if (c == 0)												/* As long as we're comparing, if it happens to */
		{ 
			MISRA_EXCEPTION_RULE_14_7()		
			return(mp); 
		}													/* come out equal we'll forego discovering that */
	else {if (c < 0)										/* all over again via the binary search.        */
		{ 
			MISRA_EXCEPTION_RULE_14_7()		
			return(NULL); 
		}
	}

	l = 1U;
	r = num_members - 1U;
	while (l <= r) {
		m = (l + r) / 2U;
		mp = table_ptr(m);
		c = compare_members(key, mp);
		if (c == 0)
			{
				MISRA_EXCEPTION_RULE_14_7()		
				return(mp);
			}
		else if (c < 0)
			{r = m - 1U;}
		else
			{l = m + 1U;}

	}

	return(NULL);
}

