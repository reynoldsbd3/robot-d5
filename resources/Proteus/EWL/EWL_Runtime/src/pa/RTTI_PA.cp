/* CodeWarrior Runtime Library
 * Copyright © 1993-2005 Metrowerks Corporation.  All rights reserved.
/*  Copyright.: Copyright © 2006-2012 Freescale Semiconductor, Inc.  All rights reserved.   */
 *
 * $Date: 2012/06/01 16:06:51 $
 * $Revision: 1.1 $
 */

#ifdef __CWCC__
	#pragma exceptions on
#endif

#include <CPlusLib.h>
#define __NOSTRING__	//	do not include <string>
#include <typeinfo>
#include <string.h>


#ifndef _EWL_NO_CPP_NAMESPACE      // hh 980106
	namespace std {
#endif

/************************************************************************/
/* Purpose..: type_info destructor function								*/
/* Input....: ---														*/
/* Return...: ---														*/
/************************************************************************/
//type_info::~type_info() { }

/************************************************************************/
/* Purpose..: type_info == comparision function							*/
/* Input....: type_info reference										*/
/* Return...: true: same types											*/
/************************************************************************/
/*
bool type_info::operator==(const type_info& ti) const
{
	return (strequal(tname,ti.tname) == 0);
}
*/
/************************************************************************/
/* Purpose..: type_info != comparision function							*/
/* Input....: type_info reference										*/
/* Return...: true: same types											*/
/************************************************************************/
/*
bool type_info::operator!=(const type_info& ti) const
{
	return (strequal(tname,ti.tname) != 0);
}
*/
/************************************************************************/
/* Purpose..: type_info collation order function						*/
/* Input....: type_info reference										*/
/* Return...: true: *this precedes type_info							*/
/************************************************************************/
/*
bool type_info::before(const type_info& ti) const
{
	return (strequal(tname,ti.tname) < 0);
}
*/
/************************************************************************/
/* Purpose..: return type name											*/
/* Input....: ---														*/
/* Return...: pointer to type name										*/
/************************************************************************/
/*
const char*	type_info::name() const
{
	return tname;
}
*/
/************************************************************************/
/* Purpose..: type_info copy constructor								*/
/* Input....: type_info reference										*/
/* Return...: ---														*/
/************************************************************************/
/*
type_info::type_info(const type_info& ti)
{
	tname=ti.tname;
}
*/
/************************************************************************/
/* Purpose..: type_info assigment operator								*/
/* Input....: type_info reference										*/
/* Return...: type_info reference										*/
/************************************************************************/
/*
type_info& type_info::operator=(const type_info& ti)
{
	tname=ti.tname;
	return *this;
}
*/
#ifndef _EWL_NO_CPP_NAMESPACE      // hh 980106
	}
#endif
