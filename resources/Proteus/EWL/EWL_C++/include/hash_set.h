/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:41:55 $
 * $Revision: 1.2 $
 */

/*
	WARNING - WARNING - WARNING

	This header is NON-STANDARD

	The classes herein are offered as extensions to the C++ standard.
	They are marked as such by the namespace CodeWarrior.
*/

#ifndef _HASH_SET_H
#define _HASH_SET_H

#include <hash_set>
#include <hash_fun.h>

#ifndef _EWL_NO_CPP_NAMESPACE
	using CodeWarrior::hash_set;
	using CodeWarrior::hash_multiset;
#endif  // _EWL_NO_CPP_NAMESPACE

#endif  // _HASH_SET_H
