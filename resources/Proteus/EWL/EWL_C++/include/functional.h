/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/28 06:41:52 $
 * $Revision: 1.2 $
 */

#ifndef _FUNCTIONAL_H
#define _FUNCTIONAL_H

#include <functional>

#ifndef _EWL_NO_CPP_NAMESPACE
	using std::unary_function;
	using std::binary_function;
	using std::plus;
	using std::minus;
	using std::multiplies;
	using std::divides;
	using std::modulus;
	using std::negate;
	using std::equal_to;
	using std::not_equal_to;
	using std::greater;
	using std::less;
	using std::greater_equal;
	using std::less_equal;
	using std::logical_and;
	using std::logical_or;
	using std::logical_not;
	using std::unary_negate;
	using std::not1;
	using std::binary_negate;
	using std::not2;
	using std::binder1st;
	using std::bind1st;
	using std::binder2nd;
	using std::bind2nd;
	using std::pointer_to_unary_function;
	using std::pointer_to_binary_function;
	using std::ptr_fun;
	using std::mem_fun_t;
	using std::mem_fun1_t;
	using std::mem_fun;
	using std::mem_fun_ref_t;
	using std::mem_fun1_ref_t;
	using std::mem_fun_ref;
#if !defined(__CWCC__) || (defined(__CWCC__) && __CWCC__ >= 0x2400)
	using std::const_mem_fun_t;
	using std::const_mem_fun1_t;
	using std::const_mem_fun_ref_t;
	using std::const_mem_fun1_ref_t;
#endif  // !defined(__CWCC__) || (defined(__CWCC__) && __CWCC__ >= 0x2400)
#endif  // _EWL_NO_CPP_NAMESPACE  

#endif  // _FUNCTIONAL_H 
