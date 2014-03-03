/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:41:32 $
 * $Revision: 1.2 $
 */

// WARNING
//
// cdeque is a non-standard, non-portable header and container.
//
// WARNING
//
// It's purpose here is to aid in the implementation of deque.
// cdeque is very similar to deque except that it has capacity
// and reserve methods like vector.  It guarantees amoritized
// constant time push_front and push_back, but it may invalidate
// references (when capacity is increased) during these operations.
//
// cdeque is short for "circular deque".  It is implemented with
// a contiguous chunk of memory like vector.  vector elements always
// start at the low memory address, and there is an uninitialized chunk
// of reserve memory at the high end to grow into.  In contrast, cdeque
// maps the memory into a circular path.  The first element can start
// anywhere on the circle, and the reserve memory extends from beyond
// the last element up to the first element.  That is:
// In use     : [first, last)
// In reserve : [last, first)
//

#ifndef _CDEQUE_H
#define _CDEQUE_H

#include <cdeque>

#ifndef _EWL_NO_CPP_NAMESPACE
	using CodeWarrior::cdeque;
#endif

#endif // _CDEQUE_H
