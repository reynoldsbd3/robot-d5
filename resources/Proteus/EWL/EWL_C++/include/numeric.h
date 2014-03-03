/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:42:15 $
 * $Revision: 1.2 $
 */

#ifndef _NUMERIC_H  
#define _NUMERIC_H

#include <numeric>

#ifndef _EWL_NO_CPP_NAMESPACE
	using std::accumulate;
	using std::inner_product;
	using std::partial_sum;
	using std::adjacent_difference;
#endif  // _EWL_NO_CPP_NAMESPACE      

#endif  // _NUMERIC_H  
