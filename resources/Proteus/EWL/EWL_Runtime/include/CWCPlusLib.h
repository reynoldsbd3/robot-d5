/* CodeWarrior Runtime Library
 * Copyright © 1993-2005 Metrowerks Corporation.  All rights reserved.
 * * Copyright © 2012 Freescale Inc.  All rights reserved.
 *
 * $Date: 2012/06/01 16:04:06 $
 * $Revision: 1.1 $
 */

#ifndef __CWCPLUSLIB_H__
#define __CWCPLUSLIB_H__

#include <ansi_parms.h>
#include <stddef.h>
#if __arm__
#include <arm/CWRuntime.h>
#elif __COLDFIRE__
#include <coldfire/CWRuntime.h>
#elif __PPC_EABI__
#include <pa/CWRuntime.h>
#else
#endif

#if defined(__CWCC__) && !defined(__IA64_CPP_ABI_ARM__)

typedef struct PTMF {
	long	this_delta;				//	delta to this pointer
	long	vtbl_offset;				//	offset of virtual function pointer in vtable (<0: non-virtual function address)
	union {
		void	*func_addr;			//	non-virtual function address
		long	ventry_offset;			//	offset of vtable pointer in class
	}	func_data;
}	PTMF;

_EWL_BEGIN_EXTERN_C

extern long						__ptmf_test(const PTMF *ptmf);
extern long						__ptmf_cmpr(const PTMF *ptmf1,const PTMF *ptmf2);
extern void						__ptmf_call(...);
extern void						__ptmf_call4(...);
extern void						__ptmf_scall(...);
extern void						__ptmf_scall4(...);

extern PTMF						*__ptmf_cast(long offset,const PTMF *ptmfrom,PTMF *ptmto);

_EWL_IMP_EXP_RUNTIME extern void	*__copy(char *to,char *from,size_t size);
_EWL_IMP_EXP_RUNTIME extern void	*__clear(char *mem,size_t size);
_EWL_IMP_EXP_RUNTIME extern void	*__init_arr(void *memptr,ConstructorDestructor constructor,size_t object_size,size_t nobjects);
_EWL_IMP_EXP_RUNTIME extern void	*__new_arr(ConstructorDestructor constructor,size_t object_size,size_t objects);
_EWL_IMP_EXP_RUNTIME extern void	__del_arr(void *memptr,ConstructorDestructor destructor);
_EWL_IMP_EXP_RUNTIME extern void	__dc_arr(void *mem,ConstructorDestructor con_des,short object_size,short objects);

_EWL_IMP_EXP_RUNTIME extern void	__construct_array(void *block,ConstructorDestructor ctor,ConstructorDestructor dtor,size_t size,size_t n);
_EWL_IMP_EXP_RUNTIME extern void	__destroy_arr(void *block,ConstructorDestructor dtor,size_t size,size_t n);
_EWL_IMP_EXP_RUNTIME extern void	*__construct_new_array(void *block,ConstructorDestructor ctor,ConstructorDestructor dtor,size_t size,size_t n);

extern void	__destroy_new_array(void *block,ConstructorDestructor dtor);
extern void	*__destroy_new_array2(void *block,ConstructorDestructor dtor);
extern void	__destroy_new_array3(void *block,ConstructorDestructor dtor,void *dealloc_func,short has_size_t_param);

_EWL_END_EXTERN_C
#endif /* __CWCC__ */

#endif
