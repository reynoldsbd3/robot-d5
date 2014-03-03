/************************************************************************/
/*	Project...:	Standard C++ Library									*/
/*	Name......:	NMWException.h											*/
/*	Purpose...:	portable exception handling								*/
/*  Copyright.: Copyright © 1993-2006                                   */
/*              Freescale Semiconductor, Inc. All rights reserved.	    */
/*  $Date: 2012/06/28 06:43:14 $                                        */
/*  $Revision: 1.2 $                                                    */
/************************************************************************/

#ifndef __NMWEXCEPTIONS__      
#define __NMWEXCEPTIONS__

#include <ansi_parms.h>

/*	[this section needs to be copied from CException.h	*/

typedef short	vbase_ctor_arg_type;
typedef char	local_cond_type;

typedef struct CatchInfo {
	void			*location;				/*	pointer to complete exception object	*/
	void			*typeinfo;				/*	pointer to complete exception object's typeinfo	*/
	void			*dtor;					/*	pointer to complete exception object's destructor function (0: no destructor)	*/
	void			*sublocation;			/*	pointer to handlers subobject in complete exception object	*/
	long			pointercopy;			/*	adjusted pointer copy for pointer matches (sublocation will point to the loaction)	*/
	void			*stacktop;				/*	pointer to handlers stack top (reused for exception specification in specification handler)	*/
}	CatchInfo;

/*	[this section needs to be copied from CException.h] */

typedef struct DestructorChain {			/*	global destructor chain	*/
	struct DestructorChain	*next;			/*	pointer to next destructor chain element	*/
	void					*destructor;	/*	pointer to destructor function	*/
	void					*object;		/*	pointer to memory location (0L: element is marker)	*/
}	DestructorChain;

#ifdef __cplusplus
extern "C" {
#endif

_EWL_IMP_EXP_RUNTIME extern DestructorChain	*__global_destructor_chain;
_EWL_IMP_EXP_RUNTIME extern void			*__register_global_object(void *object,void *destructor,void *regmem);
_EWL_IMP_EXP_RUNTIME extern void			__destroy_global_chain(void);

extern void									__init__catch(CatchInfo* catchinfo);
_EWL_IMP_EXP_RUNTIME extern void			__end__catch(CatchInfo* catchinfo);
_EWL_IMP_EXP_RUNTIME extern void			__throw(char *throwtype,void *location,void *dtor);
_EWL_IMP_EXP_RUNTIME extern void			__rethrow(void);
_EWL_IMP_EXP_RUNTIME extern char			__throw_catch_compare(const char *throwtype,const char *catchtype,long *offset_result);
_EWL_IMP_EXP_RUNTIME extern void			__unexpected(CatchInfo* catchinfo);
void __call_static_initializers(void);  /* hh 971215 added per John McEnerney's instructions */
#if defined(__PPC_EABI__)
_EWL_IMP_EXP_RUNTIME extern int				__register_fragment(void *info, char *TOC);
_EWL_IMP_EXP_RUNTIME extern void			__unregister_fragment(int fragmentID);
#elif defined(__POWERPC__)
_EWL_IMP_EXP_RUNTIME extern int				__register_fragment(char *code_start,char *code_end,char *data_start,char *data_end,
_EWL_IMP_EXP_RUNTIME extern char *exception_start,char *exception_end,char *TOC);
_EWL_IMP_EXP_RUNTIME extern void			__unregister_fragment(int fragmentID);
#endif

#ifdef __cplusplus
}
#endif
#endif
