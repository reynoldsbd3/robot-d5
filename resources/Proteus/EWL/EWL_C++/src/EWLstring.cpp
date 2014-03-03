/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:03:13 $
 * $Revision: 1.1 $
 */

#include <EWLstring>
#include <ewl_utility>

#undef NDEBUG

#include <cassert>

#ifndef _EWL_NO_EXCEPTIONS

#include <string>
#include <cstring>

#ifdef _EWL_FORCE_ENUMS_ALWAYS_INT
	#if _EWL_FORCE_ENUMS_ALWAYS_INT
		#pragma enumsalwaysint on
	#else
		#pragma enumsalwaysint off
	#endif
#endif  // _EWL_FORCE_ENUMS_ALWAYS_INT

#ifdef _EWL_FORCE_ENABLE_BOOL_SUPPORT
	#if _EWL_FORCE_ENABLE_BOOL_SUPPORT
		#pragma bool on
	#else
		#pragma bool off
	#endif
#endif  // _EWL_FORCE_ENABLE_BOOL_SUPPORT

_STD::_EWLstring::_EWLstring(const string& value)
	: data_(new char [value.size()+1], detail::default_delete<char[]>())
{
	*copy(value.begin(), value.end(), data_.get()) = char();
}

#endif // _EWL_NO_EXCEPTIONS

#ifndef _EWL_NO_CPP_NAMESPACE
	namespace CodeWarrior {
#else
	#ifndef CodeWarrior
		#define CodeWarrior
	#endif
#endif  // _EWL_NO_CPP_NAMESPACE

_EWL_IMP_EXP_CPP bool operator==(const ewl_settings& x, const ewl_settings& y)
{
	if (x.data_[0] != y.data_[0])
		assert(!"ewl_settings: inconsistent setting for _EWL_NO_LOCALE");
	if (x.data_[1] != y.data_[1])
		assert(!"ewl_settings: inconsistent setting for _EWL_SINGLE_THREAD");
	if (x.data_[2] != y.data_[2])
		assert(!"ewl_settings: inconsistent setting for _EWL_NO_EXCEPTIONS");
	if (x.data_[3] != y.data_[3])
		assert(!"ewl_settings: inconsistent setting for _EWL_NO_WCHART_CPP_SUPPORT");
	if (x.data_[4] != y.data_[4])
		assert(!"ewl_settings: inconsistent setting for _EWL_FORCE_ENUMS_ALWAYS_INT");
	if (x.data_[5] != y.data_[5])
		assert(!"ewl_settings: inconsistent setting for _EWL_FORCE_ENABLE_BOOL_SUPPORT");
	if (x.data_[6] != y.data_[6])
		assert(!"ewl_settings: inconsistent setting for _EWL_DEBUG");
	if (x.data_[7] != y.data_[7])
		assert(!"ewl_settings: inconsistent setting for _EWL_USING_EWL_C");
	if (x.data_[8] != y.data_[8])
		assert(!"ewl_settings: inconsistent setting for _EWL_EXTENDED_PRECISION_OUTP");
	if (x.data_[9] != y.data_[9])
		assert(!"ewl_settings: inconsistent setting for _Inhibit_Container_Optimization");
	if (x.data_[10] != y.data_[10])
		assert(!"ewl_settings: inconsistent setting for _EWL_RAW_ITERATORS");
	if (x.data_[11] != y.data_[11])
		assert(!"ewl_settings: inconsistent setting for _EWL_NO_IO");
	if (x.data_[12] != y.data_[12])
		assert(!"ewl_settings: inconsistent setting for _EWL_NO_CONSOLE_IO");
	if (x.data_[13] != y.data_[13])
		assert(!"ewl_settings: inconsistent setting for _EWL_LL_FILE_SUPPORT");
	if (x.data_[14] != y.data_[14])
		assert(!"ewl_settings: inconsistent setting for _EWL_NO_FLOATING_POINT");
	if (x.data_[15] != y.data_[15])
		assert(!"ewl_settings: inconsistent setting for _EWL_SHARED_PTR_HAS_MUTEX");
	if (x.data_[16] != y.data_[16])
		assert(!"ewl_settings: inconsistent setting for _EWL_NO_RTTI");
	return true;
}

#undef check

_EWL_IMP_EXP_CPP bool check(const ewl_settings& t)
{
	return ewl_settings() == t;
}

#ifndef _EWL_NO_CPP_NAMESPACE
	} // namespace CodeWarrior
#endif
