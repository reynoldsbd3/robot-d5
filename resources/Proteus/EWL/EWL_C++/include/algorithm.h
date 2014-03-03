/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:41:28 $
 * $Revision: 1.2 $
 */

#ifndef _ALGORITHM_H
#define _ALGORITHM_H

#include <algorithm>

#ifndef _EWL_NO_CPP_NAMESPACE

	#ifdef min
	#undef min
	#endif

	#ifdef max
	#undef max
	#endif

	using std::for_each;
	using std::find;
	using std::find_if;
	using std::equal;
	using std::search;
	using std::find_end;
	using std::find_first_of;
	using std::adjacent_find;
	using std::count;
	using std::count_if;
	using std::mismatch;
	using std::search_n;
	using std::min;
	using std::max;
	using std::min_element;
	using std::max_element;
	using std::lexicographical_compare;
	using std::copy;
	using std::copy_backward;
	using std::swap;
	using std::iter_swap;
	using std::swap_ranges;
	using std::transform;
	using std::replace;
	using std::replace_if;
	using std::replace_copy;
	using std::replace_copy_if;
	using std::fill;
	using std::fill_n;
	using std::generate;
	using std::generate_n;
	using std::remove_copy;
	using std::remove_copy_if;
	using std::remove;
	using std::remove_if;
	using std::unique_copy;
	using std::unique;
	using std::reverse;
	using std::reverse_copy;
	using std::rotate;
	using std::rotate_copy;
	using std::random_shuffle;
	using std::partition;
	using std::stable_partition;
	using std::lower_bound;
	using std::upper_bound;
	using std::equal_range;
	using std::binary_search;
	using std::merge;
	using std::inplace_merge;
	using std::push_heap;
	using std::pop_heap;
	using std::make_heap;
	using std::sort_heap;
	using std::sort;
	using std::stable_sort;
	using std::partial_sort;
	using std::partial_sort_copy;
	using std::nth_element;
	using std::includes;
	using std::set_union;
	using std::set_intersection;
	using std::set_difference;
	using std::set_symmetric_difference;
	using std::next_permutation;
	using std::prev_permutation;
#endif  // _EWL_NO_CPP_NAMESPACE

#endif  // _ALGORITHM_H
