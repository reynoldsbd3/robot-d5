/* CodeWarrior PowerPC EABI Runtime Support Library 
 * Copyright © 1995-2006 Freescale Semiconductor, Inc.  All rights reserved.
 *
 * $Date: 2012/06/01 16:04:53 $
 * $Revision: 1.1 $
 */

/* 		spe.h
 *
 *		Implements spe-pim Version 0.93 (11Dec2002)
 *
 *		Copyright © 2002-2006 Freescale Semiconductor, Inc.
 *		All rights reserved.
 */

#ifndef __SPE_H__
#define __SPE_H__

#if __SPE__

#include <stddef.h>
#include <stdint.h>

#include <errno.h>

#if !(__option(c9x) && !__option(cplusplus))
#if !__option(bool)
	typedef unsigned char _Bool;
#else
	typedef bool _Bool;
#endif
#else
#include <stdbool.h>
#endif

#if __option(sfp_emulation)
typedef float		__spe_float;
#define __float_to_spe_float(f) (f)
#define __spe_float_to_float(f) (f)
#else
typedef uint32_t	__spe_float;
#define __float_to_spe_float(f) ({float fmem = (f); *(float *)&fmem;})
#define __spe_float_to_float(f) ({__spe_float fmem = (f); *(float *)&fmem;})
#endif

#pragma gcc_extensions on

/*	Several Macro for internal use only	*/


#define __ev_gen_std_3op(instr, a, b)				({register __ev64_opaque__ rR, rA, rB; rA = a; rB = b; asm {.nonvolatile; instr rR, rA, rB;} rR;})
#define __ev_gen_std_3op_ui32(instr, a, b)			({register uint32_t rR, rA, rB; rA = a; rB = b; asm {.nonvolatile; instr rR, rA, rB;} rR;})
#define __ev_gen_std_3op_imm(instr, a, imm)			({register __ev64_opaque__ rD, rA; rA = a; asm {.nonvolatile; instr rD,rA,imm}; rD;})
#define __ev_gen_std_3op_from_imm(instr, a, imm)	({register __ev64_opaque__ rD, rA; rA = a; asm {.nonvolatile; instr rD,imm,rA}; rD;})
#define __ev_gen_std_2op(instr, a)					({register __ev64_opaque__ rR, rA; rA = a; asm	{.nonvolatile; instr rR, rA;} rR;})
#define __ev_gen_std_2op_imm(instr, imm)			({register __ev64_opaque__ rR; asm	{.nonvolatile; instr rR, imm;} rR;})

#define __ev_op(prefix,cmp,cond,a,b, bit)	({register uint32_t r; register __ev64_opaque__ rA, rB; rA = a; rB = b; asm{.nonvolatile; prefix##cmp##cond cr0, rA, rB;mfcr r;} (_Bool) ((r >> bit) & 1);})

#define __ev_any(prefix, cmp, cond, a, b)	__ev_op(prefix, cmp, cond, a, b, 29)
#define __ev_all(prefix, cmp, cond, a, b)	__ev_op(prefix, cmp, cond, a, b, 28)
#define __ev_upper(prefix, cmp, cond, a, b)	__ev_op(prefix, cmp, cond, a, b, 31)
#define __ev_lower(prefix, cmp, cond, a, b)	__ev_op(prefix, cmp, cond, a, b, 30)

#define __ev_cond(cond,comp,a,b)	__ev_##comp(ev, cmp, cond, a, b)
#define __ev_fscond(cond,comp,a,b)	__ev_##comp(evfs, cmp, cond, a, b)

#define __ev_fstst(cond,comp,a,b)	__ev_##comp(evfs, tst, cond, a, b)
#define __ev_select_cmp_cond(prefix, cond, a, b, c, d)	\
		({register __ev64_opaque__ rR, rA, rB, rC, rD; rA = a; rB = b; rC = c; rD = d;	\
		  asm {.nonvolatile; prefix##cmp##cond cr0, rA, rB; evsel rR, rC, rD, cr0} rR;})

#define __ev_select_tst_cond(prefix, cond, a, b, c, d)	\
		({register __ev64_opaque__ rR, rA, rB, rC, rD; rA = a; rB = b; rC = c; rD = d;	\
		  asm {.nonvolatile; prefix##tst##cond cr0, rA, rB; evsel rR, rC, rD, cr0} rR;})

#define __ev_load_t(instr, b, i, t) ({ register __ev64_opaque__ rR; register uint32_t rI; register t *rB; rB = b; rI = i; asm {.nonvolatile; instr rR, rB, rI;} rR;})
#define __ev_load_displ_t(instr, b, displ, t)  ({ register __ev64_opaque__ rR; register t * rB; rB = b; asm {.nonvolatile; instr rR, (displ)(rB);} rR;})
#define __ev_store_t(instr, d, b, i, t)  do {register __ev64_opaque__ rD; register t *rB; register int rI; rD = d; rB = b; rI = i; asm {.nonvolatile; instr rD, rB, rI;}} while (0)
#define __ev_store_displ_t(instr, d, b, displ, t) do {register __ev64_opaque__ rD; register t *rB; rD = d; rB = b; asm {.nonvolatile; instr rD, (displ)(rB);}} while (0)

#define __ev_load(instr, b, i) __ev_load_t(instr, b, i, __ev64_opaque__)
#define __ev_load_16(instr, b, i) __ev_load_t(instr, b, i, uint16_t)
#define __ev_load_32(instr, b, i) __ev_load_t(instr, b, i, uint32_t)
#define __ev_load_displ(instr, b, displ)  __ev_load_displ_t(instr, b, displ, __ev64_opaque__)
#define __ev_load_displ_16(instr, b, displ)  __ev_load_displ_t(instr, b, displ, uint16_t)
#define __ev_load_displ_32(instr, b, displ)  __ev_load_displ_t(instr, b, displ, uint32_t)
#define __ev_store(instr, d, b, i)  __ev_store_t(instr, d, b, i, __ev64_opaque__)
#define __ev_store_32(instr, d, b, i)  __ev_store_t(instr, d, b, i, uint32_t)
#define __ev_store_displ(instr, d, b, displ) __ev_store_displ_t(instr, d, b, displ, __ev64_opaque__)
#define __ev_store_displ_32(instr, d, b, displ) __ev_store_displ_t(instr, d, b, displ, uint32_t)


#define __define_ev_load(func, instr) \
		inline __ev64_opaque__ func(register __ev64_opaque__ *ptr, register int32_t offset) \
		{ return __ev_load_t(instr, ptr, offset, __ev64_opaque__); }
#define __define_ev_load_16(func, instr) \
		inline __ev64_opaque__ func(register uint16_t *ptr, register int32_t offset) \
		{ return __ev_load_t(instr, ptr, offset, uint16_t); }
#define __define_ev_load_32(func, instr) \
		inline __ev64_opaque__ func(register uint32_t *ptr, register int32_t offset) \
		{ return __ev_load_t(instr, ptr, offset, uint32_t); }

#define __define_ev_store(func, instr)  \
		inline void func(register __ev64_opaque__ v, register __ev64_opaque__ *p, register int32_t offset) \
		{ __ev_store_t(instr, v, p, offset, __ev64_opaque__); }
#define __define_ev_store_32(func, instr)  \
		inline void func(register __ev64_opaque__ v, register uint32_t *p, register int32_t offset) \
		{ __ev_store_t(instr, v, p, offset, uint32_t); }


/* End Internal Macros	*/

/*	Basic Instruction Mapping	*/

#define __ev_addw(a, b)		__ev_gen_std_3op(evaddw, a, b)
#define __ev_subfw(a, b)	__ev_gen_std_3op(evsubfw, a, b)
#define __ev_subw(a, b)		__ev_gen_std_3op(evsubfw, b, a)
#define __ev_and(a, b)		__ev_gen_std_3op(evand, a, b)
#define __ev_or(a, b)		__ev_gen_std_3op(evor, a, b)
#define __ev_xor(a, b)		__ev_gen_std_3op(evxor, a, b)
#define __ev_nand(a, b)		__ev_gen_std_3op(evnand, a, b)
#define __ev_nor(a, b)		__ev_gen_std_3op(evnor, a, b)
#define __ev_eqv(a, b)		__ev_gen_std_3op(eveqv, a, b)
#define __ev_andc(a, b)		__ev_gen_std_3op(evandc, a, b)
#define __ev_orc(a, b)		__ev_gen_std_3op(evorc, a, b)
#define __ev_rlw(a, b)		__ev_gen_std_3op(evrlw, a, b)
#define __ev_slw(a, b)		__ev_gen_std_3op(evslw, a, b)
#define __ev_srws(a, b)		__ev_gen_std_3op(evsrws, a, b)
#define __ev_srwu(a, b)		__ev_gen_std_3op(evsrwu, a, b)
#define __ev_mergehi(a, b)	__ev_gen_std_3op(evmergehi, a, b)
#define __ev_mergelo(a, b)	__ev_gen_std_3op(evmergelo, a, b)
#define __ev_mergelohi(a, b)	__ev_gen_std_3op(evmergelohi, a, b)
#define __ev_mergehilo(a, b)	__ev_gen_std_3op(evmergehilo, a, b)

#define __ev_divws(a, b)	__ev_gen_std_3op(evdivws, a, b)
#define __ev_divwu(a, b)	__ev_gen_std_3op(evdivwu, a, b)


#define __ev_addiw(a, uimm) __ev_gen_std_3op_imm(evaddiw, a, uimm)
#define __ev_subiw(a, uimm)	__ev_gen_std_3op_from_imm(evsubifw, a, uimm)
#define __ev_subifw(uimm, a) __ev_gen_std_3op_from_imm(evsubifw, a, uimm)
#define __ev_rlwi(a, uimm)	__ev_gen_std_3op_imm(evrlwi, a, uimm)
#define __ev_slwi(a, uimm)	__ev_gen_std_3op_imm(evslwi, a, uimm)
#define __ev_srwis(a, uimm)	__ev_gen_std_3op_imm(evsrwis, a, uimm)
#define __ev_srwiu(a, uimm)	__ev_gen_std_3op_imm(evsrwiu, a, uimm)

#define __ev_abs(a)			__ev_gen_std_2op(evabs, a)
#define __ev_neg(a)			__ev_gen_std_2op(evneg, a)
#define __ev_extsb(a)		__ev_gen_std_2op(evextsb, a)
#define __ev_extsh(a)		__ev_gen_std_2op(evextsh, a)
#define __ev_cntlzw(a)		__ev_gen_std_2op(evcntlzw, a)
#define __ev_cntlsw(a)		__ev_gen_std_2op(evcntlsw, a)
#define __ev_rndw(a)		__ev_gen_std_2op(evrndw, a)
#define __ev_mra(a)			__ev_gen_std_2op(evmra, a)

#define __ev_splati(imm)	__ev_gen_std_2op_imm(evsplati, imm)
#define __ev_splatfi(imm)	__ev_gen_std_2op_imm(evsplatfi, imm)

#define __brinc(a, b)		__ev_gen_std_3op_ui32(brinc, a, b)


/*	Compare Predicates	
	Note: all the compare functions use CR0
*/

#define __ev_any_gts(a, b)		__ev_cond(gts,any,a,b)
#define __ev_all_gts(a, b)		__ev_cond(gts,all,a,b)
#define __ev_upper_gts(a, b)	__ev_cond(gts,upper,a,b)
#define __ev_lower_gts(a, b)	__ev_cond(gts,lower,a,b)

#define __ev_any_gtu(a, b)		__ev_cond(gtu,any,a,b)
#define __ev_all_gtu(a, b)		__ev_cond(gtu,all,a,b)
#define __ev_upper_gtu(a, b)	__ev_cond(gtu,upper,a,b)
#define __ev_lower_gtu(a, b)	__ev_cond(gtu,lower,a,b)

#define __ev_any_lts(a, b)		__ev_cond(lts,any,a,b)
#define __ev_all_lts(a, b)		__ev_cond(lts,all,a,b)
#define __ev_upper_lts(a, b)	__ev_cond(lts,upper,a,b)
#define __ev_lower_lts(a, b)	__ev_cond(lts,lower,a,b)

#define __ev_any_ltu(a, b)		__ev_cond(ltu,any,a,b)
#define __ev_all_ltu(a, b)		__ev_cond(ltu,all,a,b)
#define __ev_upper_ltu(a, b)	__ev_cond(ltu,upper,a,b)
#define __ev_lower_ltu(a, b)	__ev_cond(ltu,lower,a,b)

#define __ev_any_eq(a, b)		__ev_cond(eq,any,a,b)
#define __ev_all_eq(a, b)		__ev_cond(eq,all,a,b)
#define __ev_upper_eq(a, b)		__ev_cond(eq,upper,a,b)
#define __ev_lower_eq(a, b)		__ev_cond(eq,lower,a,b)

#define __ev_any_fs_gt(a, b)	__ev_fscond(gt,any,a,b)
#define __ev_all_fs_gt(a, b)	__ev_fscond(gt,all,a,b)
#define __ev_upper_fs_gt(a, b)	__ev_fscond(gt,upper,a,b)
#define __ev_lower_fs_gt(a, b)	__ev_fscond(gt,lower,a,b)

#define __ev_any_fs_lt(a, b)	__ev_fscond(lt,any,a,b)
#define __ev_all_fs_lt(a, b)	__ev_fscond(lt,all,a,b)
#define __ev_upper_fs_lt(a, b)	__ev_fscond(lt,upper,a,b)
#define __ev_lower_fs_lt(a, b)	__ev_fscond(lt,lower,a,b)

#define __ev_any_fs_eq(a, b)	__ev_fscond(eq,any,a,b)
#define __ev_all_fs_eq(a, b)	__ev_fscond(eq,all,a,b)
#define __ev_upper_fs_eq(a, b)	__ev_fscond(eq,upper,a,b)
#define __ev_lower_fs_eq(a, b)	__ev_fscond(eq,lower,a,b)

#define __ev_any_fs_tst_gt(a, b)	__ev_fstst(gt, any, a, b)
#define __ev_all_fs_tst_gt(a, b)	__ev_fstst(gt, all, a, b)
#define __ev_upper_fs_tst_gt(a, b)	__ev_fstst(gt, upper, a, b)
#define __ev_lower_fs_tst_gt(a, b)	__ev_fstst(gt, lower, a, b)

#define __ev_any_fs_tst_lt(a, b)	__ev_fstst(lt, any, a, b)
#define __ev_all_fs_tst_lt(a, b)	__ev_fstst(lt, all, a, b)
#define __ev_upper_fs_tst_lt(a, b)	__ev_fstst(lt, upper, a, b)
#define __ev_lower_fs_tst_lt(a, b)	__ev_fstst(lt, lower, a, b)

#define __ev_any_fs_tst_eq(a, b)	__ev_fstst(eq, any, a, b)
#define __ev_all_fs_tst_eq(a, b)	__ev_fstst(eq, all, a, b)
#define __ev_upper_fs_tst_eq(a, b)	__ev_fstst(eq, upper, a, b)
#define __ev_lower_fs_tst_eq(a, b)	__ev_fstst(eq, lower, a, b)

#define __ev_select_gts(a, b, c, d)	__ev_select_cmp_cond(ev, gts, a, b, c, d)
#define __ev_select_gtu(a, b, c, d)	__ev_select_cmp_cond(ev, gtu, a, b, c, d)
#define __ev_select_lts(a, b, c, d)	__ev_select_cmp_cond(ev, lts, a, b, c, d)
#define __ev_select_ltu(a, b, c, d)	__ev_select_cmp_cond(ev, ltu, a, b, c, d)
#define __ev_select_eq(a, b, c, d)	__ev_select_cmp_cond(ev, eq, a, b, c, d)

#define __ev_select_fs_gt(a, b, c, d)	__ev_select_cmp_cond(evfs, gt, a, b, c, d)
#define __ev_select_fs_lt(a, b, c, d)	__ev_select_cmp_cond(evfs, lt, a, b, c, d)
#define __ev_select_fs_eq(a, b, c, d)	__ev_select_cmp_cond(evfs, eq, a, b, c, d)

#define __ev_select_fs_tst_gt(a, b, c, d)	__ev_select_tst_cond(evfs, gt, a, b, c, d)
#define __ev_select_fs_tst_lt(a, b, c, d)	__ev_select_tst_cond(evfs, lt, a, b, c, d)
#define __ev_select_fs_tst_eq(a, b, c, d)	__ev_select_tst_cond(evfs, eq, a, b, c, d)


/*	Load & Store	*/

#define __ev_ldd(b,displ)   __ev_load_displ(evldd, b, (displ)*8)
#define __ev_ldw(b,displ)   __ev_load_displ(evldw, b, (displ)*8)
#define __ev_ldh(b,displ)   __ev_load_displ(evldh, b, (displ)*8)
#define __ev_lwhe(b,displ)   __ev_load_displ_32(evlwhe, b, (displ)*4)
#define __ev_lwhou(b,displ)   __ev_load_displ_32(evlwhou, b, (displ)*4)
#define __ev_lwhos(b,displ)   __ev_load_displ_32(evlwhos, b, (displ)*4)
#define __ev_lwwsplat(b,displ)  __ev_load_displ_32(evlwwsplat, b, (displ)*4)
#define __ev_lwhsplat(b,displ)  __ev_load_displ_32(evlwhsplat, b, (displ)*4)
#define __ev_lhhesplat(b,displ)  __ev_load_displ_16(evlhhesplat, b, (displ)*2)
#define __ev_lhhousplat(b,displ) __ev_load_displ_16(evlhhousplat, b, (displ)*2)
#define __ev_lhhossplat(b,displ) __ev_load_displ_16(evlhhossplat, b, (displ)*2)

#define __ev_lddx(b, i)			__ev_load(evlddx, b, i)
#define __ev_ldwx(b, i)			__ev_load(evldwx, b, i)
#define __ev_ldhx(b, i)			__ev_load(evldhx, b, i)
#define __ev_lwhex(b, i)		__ev_load_32(evlwhex, b, i)
#define __ev_lwhoux(b, i)		__ev_load_32(evlwhoux, b, i)
#define __ev_lwhosx(b, i)		__ev_load_32(evlwhosx, b, i)
#define __ev_lwwsplatx(b, i)	__ev_load_32(evlwwsplatx, b, i)
#define __ev_lwhsplatx(b, i)	__ev_load_32(evlwhsplatx, b, i)
#define __ev_lhhesplatx(b, i)	__ev_load_16(evlhhesplatx, b, i)
#define __ev_lhhousplatx(b, i)	__ev_load_16(evlhhousplatx, b, i)
#define __ev_lhhossplatx(b, i)	__ev_load_16(evlhhossplatx, b, i)

/*
__define_ev_load(__ev_lddx, evlddx)
__define_ev_load(__ev_ldwx, evldwx)
__define_ev_load(__ev_ldhx, evldhx)
__define_ev_load_32(__ev_lwhex, evlwhex)
__define_ev_load_32(__ev_lwhoux, evlwhoux)
__define_ev_load_32(__ev_lwhosx, evlwhosx)
__define_ev_load_32(__ev_lwwsplatx, evlwwsplatx)
__define_ev_load_32(__ev_lwhsplatx, evlwhsplatx)
__define_ev_load_16(__ev_lhhesplatx, evlhhesplatx)
__define_ev_load_16(__ev_lhhousplatx, evlhhousplatx)
__define_ev_load_16(__ev_lhhossplatx, evlhhossplatx)
*/

#define __ev_stdd(d, b, imm)	__ev_store_displ(evstdd, d, b, (imm)*8)
#define __ev_stdw(d, b, imm)	__ev_store_displ(evstdw, d, b, (imm)*8)
#define __ev_stdh(d, b, imm) 	__ev_store_displ(evstdh, d, b, (imm)*8)
#define __ev_stwwe(d, b, imm)	__ev_store_displ_32(evstwwe, d, b, (imm)*4)
#define __ev_stwwo(d, b, imm)	__ev_store_displ_32(evstwwo, d, b, (imm)*4)
#define __ev_stwhe(d, b, imm)	__ev_store_displ_32(evstwhe, d, b, (imm)*4)
#define __ev_stwho(d, b, imm)	__ev_store_displ_32(evstwho, d, b, (imm)*4)

#define __ev_stddx(a, b, i)		__ev_store(evstddx, a, b, i)
#define __ev_stdwx(a, b, i)		__ev_store(evstdwx, a, b, i)
#define __ev_stdhx(a, b, i)		__ev_store(evstdhx, a, b, i)
#define __ev_stwwex(a, b, i)	__ev_store_32(evstwwex, a, b, i)
#define __ev_stwwox(a, b, i)	__ev_store_32(evstwwox, a, b, i)
#define __ev_stwhex(a, b, i)	__ev_store_32(evstwhex, a, b, i)
#define __ev_stwhox(a, b, i)	__ev_store_32(evstwhox, a, b, i)

/*
__define_ev_store(__ev_stddx, evstddx)
__define_ev_store(__ev_stdwx, evstdwx)
__define_ev_store(__ev_stdhx, evstdhx)
__define_ev_store_32(__ev_stwwex, evstwwex)
__define_ev_store_32(__ev_stwwox, evstwwox)
__define_ev_store_32(__ev_stwhex, evstwhex)
__define_ev_store_32(__ev_stwhox, evstwhox)
*/

/*	Fixed Point Complex	*/
#define __ev_mhossf(a, b) __ev_gen_std_3op(evmhossf, a, b)
#define __ev_mhosmf(a, b) __ev_gen_std_3op(evmhosmf, a, b)
#define __ev_mhosmi(a, b) __ev_gen_std_3op(evmhosmi, a, b)
#define __ev_mhoumi(a, b) __ev_gen_std_3op(evmhoumi, a, b)
#define __ev_mhessf(a, b) __ev_gen_std_3op(evmhessf, a, b)
#define __ev_mhesmf(a, b) __ev_gen_std_3op(evmhesmf, a, b)
#define __ev_mhesmi(a, b) __ev_gen_std_3op(evmhesmi, a, b)
#define __ev_mheumi(a, b) __ev_gen_std_3op(evmheumi, a, b)
#define __ev_mhossfa(a, b) __ev_gen_std_3op(evmhossfa, a, b)
#define __ev_mhosmfa(a, b) __ev_gen_std_3op(evmhosmfa, a, b)
#define __ev_mhosmia(a, b) __ev_gen_std_3op(evmhosmia, a, b)
#define __ev_mhoumia(a, b) __ev_gen_std_3op(evmhoumia, a, b)
#define __ev_mhessfa(a, b) __ev_gen_std_3op(evmhessfa, a, b)
#define __ev_mhesmfa(a, b) __ev_gen_std_3op(evmhesmfa, a, b)
#define __ev_mhesmia(a, b) __ev_gen_std_3op(evmhesmia, a, b)
#define __ev_mheumia(a, b) __ev_gen_std_3op(evmheumia, a, b)

#define __ev_mhoumf(a, b)		__ev_gen_std_3op(evmhoumi, a, b)
#define __ev_mheumf(a, b)		__ev_gen_std_3op(evmheumi, a, b)
#define __ev_mhoumfa(a, b)		__ev_gen_std_3op(evmhoumia, a, b)
#define __ev_mheumfa(a, b)		__ev_gen_std_3op(evmheumia, a, b)
#define __ev_mhossfaaw(a, b)	__ev_gen_std_3op(evmhossfaaw, a, b)
#define __ev_mhossiaaw(a, b)	__ev_gen_std_3op(evmhossiaaw, a, b)
#define __ev_mhosmfaaw(a, b)	__ev_gen_std_3op(evmhosmfaaw, a, b)

#define __ev_mhosmiaaw(a, b)	__ev_gen_std_3op(evmhosmiaaw, a, b)
#define __ev_mhousiaaw(a, b)	__ev_gen_std_3op(evmhousiaaw, a, b)
#define __ev_mhoumiaaw(a, b)	__ev_gen_std_3op(evmhoumiaaw, a, b)
#define __ev_mhessfaaw(a, b)	__ev_gen_std_3op(evmhessfaaw, a, b)
#define __ev_mhessiaaw(a, b)	__ev_gen_std_3op(evmhessiaaw, a, b)
#define __ev_mhesmfaaw(a, b)	__ev_gen_std_3op(evmhesmfaaw, a, b)
#define __ev_mhesmiaaw(a, b)	__ev_gen_std_3op(evmhesmiaaw, a, b)
#define __ev_mheusiaaw(a, b)	__ev_gen_std_3op(evmheusiaaw, a, b)
#define __ev_mheumiaaw(a, b)	__ev_gen_std_3op(evmheumiaaw, a, b)

#define __ev_mhousfaaw(a, b)	__ev_mhousiaaw(a, b)
#define __ev_mhoumfaaw(a, b)	__ev_mhoumiaaw(a, b)
#define __ev_mheusfaaw(a, b) 	__ev_mheusiaaw(a, b)
#define __ev_mheumfaaw(a, b)	__ev_mheumiaaw(a, b)

#define __ev_mhossfanw(a, b)	__ev_gen_std_3op(evmhossfanw, a, b)
#define __ev_mhossianw(a, b)	__ev_gen_std_3op(evmhossianw, a, b)
#define __ev_mhosmfanw(a, b)	__ev_gen_std_3op(evmhosmfanw, a, b)
#define __ev_mhosmianw(a, b)	__ev_gen_std_3op(evmhosmianw, a, b)
#define __ev_mhousianw(a, b)	__ev_gen_std_3op(evmhousianw, a, b)
#define __ev_mhoumianw(a, b)	__ev_gen_std_3op(evmhoumianw, a, b)
#define __ev_mhessfanw(a, b)	__ev_gen_std_3op(evmhessfanw, a, b)
#define __ev_mhessianw(a, b)	__ev_gen_std_3op(evmhessianw, a, b)
#define __ev_mhesmfanw(a, b)	__ev_gen_std_3op(evmhesmfanw, a, b)
#define __ev_mhesmianw(a, b)	__ev_gen_std_3op(evmhesmianw, a, b)
#define __ev_mheusianw(a, b)	__ev_gen_std_3op(evmheusianw, a, b)
#define __ev_mheumianw(a, b)	__ev_gen_std_3op(evmheumianw, a, b)

#define __ev_mhousfanw(a, b)	__ev_mhousianw(a, b)
#define __ev_mhoumfanw(a, b)	__ev_mhoumianw(a, b)
#define __ev_mheusfanw(a, b)	__ev_mheusianw(a, b)
#define __ev_mheumfanw(a, b)	__ev_mheumianw(a, b)

#define __ev_mhogsmfaa(a, b)	__ev_gen_std_3op(evmhogsmfaa, a, b)
#define __ev_mhogsmiaa(a, b)	__ev_gen_std_3op(evmhogsmiaa, a, b)
#define __ev_mhogumiaa(a, b)	__ev_gen_std_3op(evmhogumiaa, a, b)
#define __ev_mhegsmfaa(a, b)	__ev_gen_std_3op(evmhegsmfaa, a, b)
#define __ev_mhegsmiaa(a, b)	__ev_gen_std_3op(evmhegsmiaa, a, b)
#define __ev_mhegumiaa(a, b)	__ev_gen_std_3op(evmhegumiaa, a, b)

#define __ev_mhogumfaa(a, b)	__ev_mhogumiaa(a, b)
#define __ev_mhegumfaa(a, b)	__ev_mhegumiaa(a, b)

#define __ev_mhogsmfan(a, b)	__ev_gen_std_3op(evmhogsmfan, a, b)
#define __ev_mhogsmian(a, b)	__ev_gen_std_3op(evmhogsmian, a, b)
#define __ev_mhogumian(a, b)	__ev_gen_std_3op(evmhogumian, a, b)
#define __ev_mhegsmfan(a, b)	__ev_gen_std_3op(evmhegsmfan, a, b)
#define __ev_mhegsmian(a, b)	__ev_gen_std_3op(evmhegsmian, a, b)
#define __ev_mhegumian(a, b)	__ev_gen_std_3op(evmhegumian, a, b)

#define __ev_mhogumfan(a, b)	__ev_mhogumian(a, b)
#define __ev_mhegumfan(a, b)	__ev_mhegumian(a, b)

#define __ev_mwhssf(a, b)	__ev_gen_std_3op(evmwhssf, a, b)
#define __ev_mwhsmf(a, b)	__ev_gen_std_3op(evmwhsmf, a, b)
#define __ev_mwhsmi(a, b)	__ev_gen_std_3op(evmwhsmi, a, b)
#define __ev_mwhumi(a, b)	__ev_gen_std_3op(evmwhumi, a, b)
#define __ev_mwhssfa(a, b)	__ev_gen_std_3op(evmwhssfa, a, b)
#define __ev_mwhsmfa(a, b)	__ev_gen_std_3op(evmwhsmfa, a, b)
#define __ev_mwhsmia(a, b)	__ev_gen_std_3op(evmwhsmia, a, b)
#define __ev_mwhumia(a, b)	__ev_gen_std_3op(evmwhumia, a, b)

#define __ev_mwhumf(a, b)	__ev_mwhumi(a, b)
#define __ev_mwhumfa(a, b)	__ev_mwhumia(a, b)


#define __ev_mwlumi(a, b)	__ev_gen_std_3op(evmwlumi, a, b)

#define __ev_mwlumia(a, b)	__ev_gen_std_3op(evmwlumia, a, b)

#define __ev_mwlssiaaw(a, b)	__ev_gen_std_3op(evmwlssiaaw, a, b)
#define __ev_mwlsmiaaw(a, b)	__ev_gen_std_3op(evmwlsmiaaw, a, b)
#define __ev_mwlusiaaw(a, b)	__ev_gen_std_3op(evmwlusiaaw, a, b)
#define __ev_mwlumiaaw(a, b)	__ev_gen_std_3op(evmwlumiaaw, a, b)


#define __ev_mwlssianw(a, b)	__ev_gen_std_3op(evmwlssianw, a, b)
#define __ev_mwlsmianw(a, b)	__ev_gen_std_3op(evmwlsmianw, a, b)
#define __ev_mwlusianw(a, b)	__ev_gen_std_3op(evmwlusianw, a, b)
#define __ev_mwlumianw(a, b)	__ev_gen_std_3op(evmwlumianw, a, b)

#define __ev_mwssf(a, b)	__ev_gen_std_3op(evmwssf, a, b)
#define __ev_mwsmf(a, b)	__ev_gen_std_3op(evmwsmf, a, b)
#define __ev_mwsmi(a, b)	__ev_gen_std_3op(evmwsmi, a, b)
#define __ev_mwumi(a, b)	__ev_gen_std_3op(evmwumi, a, b)
#define __ev_mwssfa(a, b)	__ev_gen_std_3op(evmwssfa, a, b)
#define __ev_mwsmfa(a, b)	__ev_gen_std_3op(evmwsmfa, a, b)
#define __ev_mwsmia(a, b)	__ev_gen_std_3op(evmwsmia, a, b)
#define __ev_mwumia(a, b)	__ev_gen_std_3op(evmwumia, a, b)

#define __ev_mwumf(a, b)	__ev_mwumi(a, b);
#define __ev_mwumfa(a, b)	__ev_mwumia(a, b);

#define __ev_mwssfaa(a, b)	__ev_gen_std_3op(evmwssfaa, a, b)
#define __ev_mwsmfaa(a, b)	__ev_gen_std_3op(evmwsmfaa, a, b)
#define __ev_mwsmiaa(a, b)	__ev_gen_std_3op(evmwsmiaa, a, b)
#define __ev_mwumiaa(a, b)	__ev_gen_std_3op(evmwumiaa, a, b)

#define __ev_mwumfaa(a, b)	__ev_mwumiaa(a, b);

#define __ev_mwssfan(a, b)	__ev_gen_std_3op(evmwssfan, a, b)
#define __ev_mwsmfan(a, b)	__ev_gen_std_3op(evmwsmfan, a, b)
#define __ev_mwsmian(a, b)	__ev_gen_std_3op(evmwsmian, a, b)
#define __ev_mwumian(a, b)	__ev_gen_std_3op(evmwumian, a, b)

#define __ev_mwumfan(a, b)	__ev_mwumian(a, b)

#define __ev_addssiaaw(a)	__ev_gen_std_2op(evaddssiaaw, a)
#define __ev_addsmiaaw(a)	__ev_gen_std_2op(evaddsmiaaw, a)
#define __ev_addusiaaw(a)	__ev_gen_std_2op(evaddusiaaw, a)
#define __ev_addumiaaw(a)	__ev_gen_std_2op(evaddumiaaw, a)

#define __ev_addusfaaw(a) __ev_addusiaaw(a)
#define __ev_addumfaaw(a) __ev_addumiaaw(a)
#define __ev_addsmfaaw(a) __ev_addsmiaaw(a)
#define __ev_addssfaaw(a) __ev_addssiaaw(a)

#define __ev_subfssiaaw(a)	__ev_gen_std_2op(evsubfssiaaw, a)
#define __ev_subfsmiaaw(a)	__ev_gen_std_2op(evsubfsmiaaw, a)
#define __ev_subfusiaaw(a)	__ev_gen_std_2op(evsubfusiaaw, a)
#define __ev_subfumiaaw(a)	__ev_gen_std_2op(evsubfumiaaw, a)

#define __ev_subfusfaaw(a) __ev_subfusiaaw(a)
#define __ev_subfumfaaw(a) __ev_subfumiaaw(a)
#define __ev_subfsmfaaw(a) __ev_subfsmiaaw(a)
#define __ev_subfssfaaw(a) __ev_subfssiaaw(a)

/*	Not supported in first e500, must use a 2 instructions form instead	*/

#define __ev_mwhssfaaw(a, b)	__ev_addssiaaw(__ev_mwhssf(a, b))
#define __ev_mwhssiaaw(a, b)	__ev_addssiaaw(__ev_mwhsmi(a,b))
#define __ev_mwhsmfaaw(a, b)	__ev_addsmiaaw(__ev_mwhsmf(a,b))
#define __ev_mwhsmiaaw(a, b)	__ev_addsmiaaw(__ev_mwhsmi(a,b))
#define __ev_mwhusiaaw(a, b)	__ev_addusiaaw(__ev_mwhumi(a,b))
#define __ev_mwhumiaaw(a, b)	__ev_addumiaaw(__ev_mwhumi(a,b))

#define __ev_mwhusfaaw(a, b)	__ev_mwhusiaaw(a, b)
#define __ev_mwhumfaaw(a, b)	__ev_mwhumiaaw(a, b)

#define __ev_mwhssfanw(a, b)	__ev_subfssiaaw(__ev_mwhssf(a,b))
#define __ev_mwhssianw(a, b)	__ev_subfssiaaw(__ev_mwhsmi(a,b))
#define __ev_mwhsmfanw(a, b)	__ev_subfsmiaaw(__ev_mwhsmf(a,b))
#define __ev_mwhsmianw(a, b)	__ev_subfsmiaaw(__ev_mwhsmi(a,b))
#define __ev_mwhusianw(a, b)	__ev_subfusiaaw(__ev_mwhumi(a,b))
#define __ev_mwhumianw(a, b)	__ev_subfumiaaw(__ev_mwhumi(a,b))
#define __ev_mwhusfanw(a, b)	__ev_mwhusianw(a, b)
#define __ev_mwhumfanw(a, b)	__ev_mwhumianw(a, b)
#define __ev_mwhgssfaa(a, b)	__ev_mwsmiaa(__ev_mwhssf(a, b), __ev_splati(1))
#define __ev_mwhgsmfaa(a, b)	__ev_mwsmiaa(__ev_mwhsmf(a, b), __ev_splati(1))
#define __ev_mwhgsmiaa(a, b)	__ev_mwsmiaa(__ev_mwhsmi(a, b), __ev_splati(1))
#define __ev_mwhgumiaa(a, b)	__ev_mwumiaa(__ev_mwhumi(a, b), __ev_splati(1))
#define __ev_mwhgumfaa(a, b)	__ev_mwhgumiaa(a, b)
#define __ev_mwhgssfan(a, b)	__ev_mwsmian(__ev_mwhssf(a, b), __ev_splati(1))
#define __ev_mwhgsmfan(a, b)	__ev_mwsmian(__ev_mwhsmf(a, b), __ev_splati(1))
#define __ev_mwhgsmian(a, b)	__ev_mwsmian(__ev_mwhsmi(a, b), __ev_splati(1))
#define __ev_mwhgumian(a, b)	__ev_mwsmian(__ev_mwhumi(a, b), __ev_splati(1))
#define __ev_mwhgumfan(a, b)	__ev_mwhgumian(a, b)


/*	End of not supported	*/

/*	Floating Point SIMD instructions	*/

#define __ev_fsabs(a)	__ev_gen_std_2op(evfsabs, a)
#define __ev_fsnabs(a)	__ev_gen_std_2op(evfsnabs, a)
#define __ev_fsneg(a)	__ev_gen_std_2op(evfsneg, a)
#define __ev_fsadd(a, b)	__ev_gen_std_3op(evfsadd, a, b)
#define __ev_fssub(a, b)	__ev_gen_std_3op(evfssub, a, b)
#define __ev_fsmul(a, b)	__ev_gen_std_3op(evfsmul, a, b)
#define __ev_fsdiv(a, b)	__ev_gen_std_3op(evfsdiv, a, b)
#define __ev_fscfui(a)	__ev_gen_std_2op(evfscfui, a)
#define __ev_fscfsi(a)	__ev_gen_std_2op(evfscfsi, a)
#define __ev_fscfuf(a)	__ev_gen_std_2op(evfscfuf, a)
#define __ev_fscfsf(a)	__ev_gen_std_2op(evfscfsf, a)
#define __ev_fsctui(a)	__ev_gen_std_2op(evfsctui, a)
#define __ev_fsctsi(a)	__ev_gen_std_2op(evfsctsi, a)
#define __ev_fsctuf(a)	__ev_gen_std_2op(evfsctuf, a)
#define __ev_fsctsf(a)	__ev_gen_std_2op(evfsctsf, a)
#define __ev_fsctuiz(a)	__ev_gen_std_2op(evfsctuiz, a)
#define __ev_fsctsiz(a)	__ev_gen_std_2op(evfsctsiz, a)

/*	Creation, Insertion, Extraction	*/

#define __ev_create_u64(i) ({register __ev64_opaque__ r; register uint64_t rLL = i; asm {.nonvolatile; evmergelo r, rLL@hiword, rLL@loword;} r;})
#define __ev_create_s64(i) ({register __ev64_opaque__ r; register int64_t rLL = i; asm {.nonvolatile; evmergelo r, rLL@hiword, rLL@loword;} r;})

#define __ev_create_fs(a, b)		({register __ev64_opaque__ r; register __spe_float rA, rB; rA = __float_to_spe_float(a); rB = __float_to_spe_float(b);		\
									asm {.nonvolatile; evmergelo r, rA, rB} r;})
#define __ev_create_sfix32_fs(a, b)	({register __ev64_opaque__ r, rfs; register __spe_float rA, rB; rA = __float_to_spe_float(a); rB = __float_to_spe_float(b);	\
									asm {.nonvolatile; evmergelo rfs, rA, rB; evfsctsf r, rfs} r;})
#define __ev_create_ufix32_fs(a, b)	({register __ev64_opaque__ r, rfs; register __spe_float rA, rB; rA = __float_to_spe_float(a); rB = __float_to_spe_float(b);	\
									asm {.nonvolatile; evmergelo rfs, rA, rB; evfsctuf r, rfs} r;})

/*
#define __ev_create_u32(a, b)			({register __ev64_opaque__ r; register uint32_t rA = (a), rB = (b); asm {.nonvolatile; evmergelo r, rA, rB} r;})
#define __ev_create_s32(a, b)			({register __ev64_opaque__ r; register int32_t rA = (a), rB = (b); asm {.nonvolatile; evmergelo r, rA, rB} r;})
#define __ev_create_u16(a, b, c, d)		__ev_create_u32((((a)<<16) | ((b)&0x0000ffff)),  (((c)<<16) | ((d)&0x0000ffff)))
#define __ev_create_s16(a, b, c, d)		__ev_create_s32((((a)<<16) | ((b)&0x0000ffff)),  (((c)<<16) | ((d)&0x0000ffff)))
*/
inline __ev64_opaque__ __ev_create_u32(register uint32_t rA, register uint32_t rB)
{
	register __ev64_opaque__ r; 
	asm {.nonvolatile; evmergelo r, rA, rB}
	return r;
}

inline __ev64_opaque__ __ev_create_s32(register int32_t rA, register int32_t rB)
{
	register __ev64_opaque__ r; 
	asm {.nonvolatile; evmergelo r, rA, rB}
	return r;
}

inline __ev64_opaque__ __ev_create_u16(register uint16_t a, register uint16_t b, register uint16_t c, register uint16_t d)
{
	return __ev_create_u32((((a)<<16) | ((b)&0x0000ffff)),  (((c)<<16) | ((d)&0x0000ffff)));
}

inline __ev64_opaque__ __ev_create_s16(register int16_t a, register int16_t b, register int16_t c, register int16_t d)
{
	return __ev_create_s32((((a)<<16) | ((b)&0x0000ffff)),  (((c)<<16) | ((d)&0x0000ffff)));
}

#define __ev_create_ufix32_u32(a, b)	__ev_create_u32(a, b)
#define __ev_create_sfix32_s32(a, b)	__ev_create_s32(a, b)

inline uint64_t	__ev_convert_u64(register __ev64_opaque__ a)
{
	register uint64_t r;
	register __ev64_opaque__ rtemp;
	asm {.nonvolatile; mr r@loword, a;}
	asm {.nonvolatile; evmergehi rtemp, a, a;}
	asm {.nonvolatile; mr r@hiword, rtemp;}
	return r;
}

inline int64_t	__ev_convert_s64(register __ev64_opaque__ a)
{
	register int64_t r;
	register __ev64_opaque__ rtemp;
	asm {.nonvolatile; mr r@loword, a;}
	asm {.nonvolatile; evmergehi rtemp, a, a;}
	asm {.nonvolatile; mr r@hiword, rtemp;}
	return r;
}

/* Internal Use Macros	*/

#define	__ev_get_upper(type,a)	({register type r; register __ev64_opaque__ rA = a, rtemp; asm {.nonvolatile; evmergehi rtemp, rA, rA;mr r, rtemp;} r;})
#define	__ev_get_lower(type,a)	({register type r; register __ev64_opaque__ rA = a; asm {.nonvolatile; mr r, rA;} r;})
#define	__ev_get_upper_fs(a)	({register __spe_float r; register __ev64_opaque__ rA = a, rtemp; asm {.nonvolatile; evmergehi rtemp, rA, rA;mr r, rtemp;} __spe_float_to_float(r);})
#define	__ev_get_lower_fs(a)	({register __spe_float r; register __ev64_opaque__ rA = a; asm {.nonvolatile; mr r, rA;} __spe_float_to_float(r);})
#define __ev_set_upper(type,a,upper)	({register __ev64_opaque__ r, rA = a; register type rU = upper; asm{.nonvolatile; evmergelo r, rU, rA;} r;})
#define	__ev_set_lower(type,a,lower)	({register __ev64_opaque__ r, rA = a; register type rL = lower; asm{.nonvolatile; evmergehilo r, rA, rL;}  r;})
#define __ev_set_upper_fs(a,upper)	({register __ev64_opaque__ r, rA = a; register __spe_float rU = __float_to_spe_float(upper); asm{.nonvolatile; evmergelo r, rU, rA;} r;})
#define	__ev_set_lower_fs(a,lower)	({register __ev64_opaque__ r, rA = a; register __spe_float rL = __float_to_spe_float(lower); asm{.nonvolatile; evmergehilo r, rA, rL;}  r;})

#define __ev_get_upper_u32(a)	__ev_get_upper(uint32_t, a)
#define __ev_get_lower_u32(a)	__ev_get_lower(uint32_t, a)
#define __ev_get_upper_s32(a)	__ev_get_upper(int32_t, a)
#define __ev_get_lower_s32(a)	__ev_get_lower(int32_t, a)

#define __ev_get_upper_u16(a)	__ev_get_upper(uint16_t, a)
#define __ev_get_lower_u16(a)	__ev_get_lower(uint16_t, a);
#define __ev_get_upper_s16(a)	__ev_get_upper(int16_t, a)
#define __ev_get_lower_s16(a)	__ev_get_lower(int16_t, a);

#define __ev_get_upper_sfix32_fs(a)		__ev_get_upper_fs(__ev_fscfsf(a))
#define __ev_get_lower_sfix32_fs(a)		__ev_get_lower_fs(__ev_fscfsf(a))
#define __ev_get_upper_ufix32_fs(a)		__ev_get_upper_fs(__ev_fscfuf(a))
#define __ev_get_lower_ufix32_fs(a)		__ev_get_lower_fs(__ev_fscfuf(a))

#define __ev_get_upper_ufix32_u32(a)	__ev_get_upper(uint32_t,a)
#define __ev_get_lower_ufix32_u32(a)	__ev_get_lower(uint32_t,a)
#define __ev_get_upper_sfix32_s32(a)	__ev_get_upper(int32_t,a)
#define __ev_get_lower_sfix32_s32(a)	__ev_get_lower(int32_t,a)

#define __ev_set_upper_u32(a, b)		__ev_set_upper(uint32_t,a,b)
#define __ev_set_lower_u32(a, b)		__ev_set_lower(uint32_t,a,b)
#define __ev_set_upper_s32(a, b)		__ev_set_upper(int32_t,a,b)
#define __ev_set_lower_s32(a, b)		__ev_set_lower(int32_t,a,b)

#define __ev_set_upper_ufix32_u32(a, b)	__ev_set_upper(uint32_t,a,b)
#define __ev_set_lower_ufix32_u32(a, b)	__ev_set_lower(uint32_t,a,b)
#define __ev_set_upper_sfix32_s32(a, b)	__ev_set_upper(int32_t,a,b)
#define __ev_set_lower_sfix32_s32(a, b)	__ev_set_lower(int32_t,a,b)


/* End Macros	*/



inline __ev64_opaque__ __ev_set_upper_sfix32_fs(register __ev64_opaque__ a, register float b)
{
	register int32_t temp;
	register __spe_float f;
	f = __float_to_spe_float(b);
	asm {.nonvolatile; efsctsf temp, f;}
	return __ev_set_upper(int32_t,a,temp);
}

inline __ev64_opaque__ __ev_set_lower_sfix32_fs(register __ev64_opaque__ a, register float b)
{
	register int32_t temp;
	register __spe_float f;
	f = __float_to_spe_float(b);
	asm {.nonvolatile; efsctsf temp, f;}
	return __ev_set_lower(int32_t,a,temp);
}

inline __ev64_opaque__ __ev_set_upper_ufix32_fs(register __ev64_opaque__ a, register float b)
{
	register int32_t temp;
	register __spe_float f;
	f = __float_to_spe_float(b);
	asm {.nonvolatile; efsctuf temp, f;}
	return __ev_set_upper(int32_t,a,temp);
}

inline __ev64_opaque__ __ev_set_lower_ufix32_fs(register __ev64_opaque__ a, register float b)
{
	register int32_t temp;
	register __spe_float f;
	f = __float_to_spe_float(b);
	asm {.nonvolatile; efsctuf temp, f;}
	return __ev_set_lower(int32_t,a,temp);
}

#define __ev_set_acc_vec64(a)	__ev_gen_std_2op(evmra, a)

inline __ev64_opaque__ __ev_set_acc_u64(register uint64_t a)
{
	return __ev_set_acc_vec64(__ev_create_u64(a));
}

inline __ev64_opaque__ __ev_set_acc_s64(register int64_t a)
{
	return __ev_set_acc_vec64(__ev_create_s64(a));
}

inline __ev64_opaque__ __ev_set_u32(register __ev64_opaque__ a, register uint32_t b, register uint32_t pos)
{
	return (pos & 1) ? __ev_set_lower_u32(a,b) : __ev_set_upper_u32(a,b);
}

inline __ev64_opaque__ __ev_set_s32(register __ev64_opaque__ a, register int32_t b, register uint32_t pos)
{
	return (pos & 1) ? __ev_set_lower_s32(a,b) : __ev_set_upper_s32(a,b);
}

inline __ev64_opaque__ __ev_set_fs(register __ev64_opaque__ a, register float b, register uint32_t pos)
{
	return (pos & 1) ? __ev_set_lower_fs(a,b) : __ev_set_upper_fs(a,b);
}

inline __ev64_opaque__ __ev_set_u16(register __ev64_opaque__ a, register uint16_t b, register uint32_t pos)
{
#if 1
	__ev64_u16__ t = (__ev64_u16__)a;
	uint16_t *p = (uint16_t *) &t;
	p[pos & 3] = b;
	return (__ev64_opaque__)t;
#else
	/* test performance later (no memory accesses) */
	register uint32_t i, half;
	register uint32_t uhalf = __ev_get_upper_u32(a);
	register uint32_t lhalf = __ev_get_lower_u32(a);

	/* select the word to modify */
	half = pos >> 1;
	asm{.nonvolatile; cmpwi cr0, half, 0x0000;}
	asm{.nonvolatile; isel i, uhalf, lhalf, cr0_eq;}
	
	/* replace halfword in word */
	pos = (1 ^ (pos & 1)) << 4;
	i = i & (0xffff0000 >> pos) | (b << pos);

	asm{.nonvolatile; cmpwi cr0, half, 0x0000;}
	asm{.nonvolatile; isel uhalf, i, uhalf, cr0_eq;}
	asm{.nonvolatile; isel lhalf, lhalf, i, cr0_eq;}

	return __ev_create_u32(uhalf, lhalf);
#endif
}

inline __ev64_opaque__ __ev_set_s16(register __ev64_opaque__ a, register int16_t b, register uint32_t pos)
{
#if 1
	__ev64_s16__ t = (__ev64_s16__)a;
	int16_t *p = (int16_t *) &t;
	p[pos & 3] = b;
	return (__ev64_opaque__)t;
#else
	/* test performance later (no memory accesses) */
	register uint32_t i, half;
	register uint32_t uhalf = __ev_get_upper_u32(a);
	register uint32_t lhalf = __ev_get_lower_u32(a);

	/* select the word to modify */
	half = pos >> 1;
	asm{.nonvolatile; cmpwi cr0, half, 0x0000;}
	asm{.nonvolatile; isel i, uhalf, lhalf, cr0_eq;}
	
	/* replace halfword in word */
	pos = (1 ^ (pos & 1)) << 4;
	i = i & (0xffff0000 >> pos) | (b << pos);

	asm{.nonvolatile; cmpwi cr0, half, 0x0000;}
	asm{.nonvolatile; isel uhalf, i, uhalf, cr0_eq;}
	asm{.nonvolatile; isel lhalf, lhalf, i, cr0_eq;}

	return __ev_create_u32(uhalf, lhalf);
#endif
}

inline __ev64_opaque__ __ev_set_ufix32_u32(register __ev64_opaque__ a, register uint32_t b, register uint32_t pos)
{
	__ev64_s16__ t = (__ev64_s16__)a;
	uint32_t *p = (uint32_t *) &t;
	p[pos & 3] = b;
	return (__ev64_opaque__)t;
}

inline __ev64_opaque__ __ev_set_sfix32_s32(register __ev64_opaque__ a, register int32_t b, register uint32_t pos)
{
	__ev64_s16__ t = (__ev64_s16__)a;
	int32_t *p = (int32_t *) &t;
	p[pos & 3] = b;
	return (__ev64_opaque__)t;
}

inline __ev64_opaque__ __ev_set_ufix32_fs(register __ev64_opaque__ a, register float b, register uint32_t pos)
{
	return (pos & 1) ? __ev_set_lower_ufix32_fs(a,b) : __ev_set_upper_ufix32_fs(a,b);
}

inline __ev64_opaque__ __ev_set_sfix32_fs(register __ev64_opaque__ a, register float b, register uint32_t pos)
{
	return (pos & 1) ? __ev_set_lower_sfix32_fs(a,b) : __ev_set_upper_sfix32_fs(a,b);
}

inline uint32_t	__ev_get_u32(register __ev64_opaque__ a, register uint32_t pos)
{
	return (pos & 1) ? __ev_get_lower_u32(a) : __ev_get_upper_u32(a);
}

inline int32_t	__ev_get_s32(register __ev64_opaque__ a, register uint32_t pos)
{
	return (pos & 1) ? __ev_get_lower_s32(a) : __ev_get_upper_s32(a);
}

inline float	__ev_get_fs(register __ev64_opaque__ a, register uint32_t pos)
{
	return (pos & 1) ? __ev_get_lower_fs(a) : __ev_get_upper_fs(a);
}

inline uint16_t	__ev_get_u16(register __ev64_opaque__ a,register  uint32_t pos)
{
	uint32_t temp = __ev_get_u32(a, ((pos & 2) >> 1) );
	return ((uint16_t)(temp >>  (((pos^1) & 1)*16)));
}

inline int16_t	__ev_get_s16(register __ev64_opaque__ a, register uint32_t pos)
{
	int32_t temp = __ev_get_s32(a, ((pos & 2) >> 1) );
	return ((int16_t)(temp >>  (((pos^1) & 1)*16)));
}

inline uint32_t	__ev_get_ufix32_u32(register __ev64_opaque__ a, register uint32_t pos)
{
	return (pos & 1) ? __ev_get_lower_ufix32_u32(a) : __ev_get_upper_ufix32_u32(a);
}

inline int32_t	__ev_get_sfix32_s32(register __ev64_opaque__ a, register uint32_t pos)
{
	return (pos & 1) ? __ev_get_lower_sfix32_s32(a) : __ev_get_upper_sfix32_s32(a);
}

inline float	__ev_get_ufix32_fs(register __ev64_opaque__ a, register uint32_t pos)
{
	return (pos & 1) ? __ev_get_lower_ufix32_fs(a) : __ev_get_upper_ufix32_fs(a);
}

inline float	__ev_get_sfix32_fs(register __ev64_opaque__ a, register uint32_t pos)
{
	return (pos & 1) ? __ev_get_lower_sfix32_fs(a) : __ev_get_upper_sfix32_fs(a);
}

/*	SPEFSCR Low Level Accessors	*/

#define __ev_get_spefscr(field)	({register uint32_t r; asm {.nonvolatile; mfspr r,spefscr;} (r >> (63-field)) & 1;})
#define __ev_bit_from_64(bit) (1L << (63 - (bit)))
#define __ev_getnclr_spefscr(field)	({ register uint32_t r,rd; asm {.nonvolatile; mfspr r,spefscr;} rd = r & ~__ev_bit_from_64(field);	\
			asm {.nonvolatile; mtspr spefscr, rd;} (r >> (63-field)) & 1;})

#define __ev_get_spefscr_sovh() __ev_get_spefscr(32)
#define __ev_get_spefscr_ovh() __ev_get_spefscr(33)
#define __ev_get_spefscr_fgh() __ev_get_spefscr(34)
#define __ev_get_spefscr_fxh() __ev_get_spefscr(35)
#define __ev_get_spefscr_finvh() __ev_get_spefscr(36)
#define __ev_get_spefscr_fdbzh() __ev_get_spefscr(37)
#define __ev_get_spefscr_funfh() __ev_get_spefscr(38)
#define __ev_get_spefscr_fovfh() __ev_get_spefscr(39)
#define __ev_get_spefscr_finxs() __ev_get_spefscr(42)
#define __ev_get_spefscr_finvs() __ev_get_spefscr(43)
#define __ev_get_spefscr_fdbzs() __ev_get_spefscr(44)
#define __ev_get_spefscr_funfs() __ev_get_spefscr(45)
#define __ev_get_spefscr_fovfs() __ev_get_spefscr(46)

#define __ev_get_spefscr_mode() __ev_get_spefscr(47)

#define __ev_get_spefscr_sov() __ev_get_spefscr(48)
#define __ev_get_spefscr_ov() __ev_get_spefscr(49)
#define __ev_get_spefscr_fg() __ev_get_spefscr(50)
#define __ev_get_spefscr_fx() __ev_get_spefscr(51)
#define __ev_get_spefscr_finv() __ev_get_spefscr(52)
#define __ev_get_spefscr_fdbz() __ev_get_spefscr(53)
#define __ev_get_spefscr_funf() __ev_get_spefscr(54)
#define __ev_get_spefscr_fovf() __ev_get_spefscr(55)

#define __ev_get_spefscr_finxe() __ev_get_spefscr(57)
#define __ev_get_spefscr_finve() __ev_get_spefscr(58)
#define __ev_get_spefscr_fdbze() __ev_get_spefscr(59)
#define __ev_get_spefscr_funfe() __ev_get_spefscr(60)
#define __ev_get_spefscr_fovfe() __ev_get_spefscr(61)

#define __ev_get_spefscr_frmc() ({register uint32_t r; asm {.nonvolatile; mfspr r,spefscr;} return r & 3;})

/*	CLR & SET functions	*/
#define __ev_clr_spefscr_field(field) do {register uint32_t r; asm {.nonvolatile; mfspr r,spefscr;} r &= ~__ev_bit_from_64(field); asm {.nonvolatile; mtspr spefscr,r;}} while (0)
#define __ev_clr_spefscr_sovh() __ev_clr_spefscr_field(32)
#define __ev_clr_spefscr_ovh() __ev_clr_spefscr_field(33)
#define __ev_clr_spefscr_fgh() __ev_clr_spefscr_field(34)
#define __ev_clr_spefscr_fxh() __ev_clr_spefscr_field(35)
#define __ev_clr_spefscr_finvh() __ev_clr_spefscr_field(36)
#define __ev_clr_spefscr_fdbzh() __ev_clr_spefscr_field(37)
#define __ev_clr_spefscr_funfh() __ev_clr_spefscr_field(38)
#define __ev_clr_spefscr_fovfh() __ev_clr_spefscr_field(39)
#define __ev_clr_spefscr_finxs() __ev_clr_spefscr_field(42)
#define __ev_clr_spefscr_finvs() __ev_clr_spefscr_field(43)
#define __ev_clr_spefscr_fdbzs() __ev_clr_spefscr_field(44)
#define __ev_clr_spefscr_funfs() __ev_clr_spefscr_field(45)
#define __ev_clr_spefscr_fovfs() __ev_clr_spefscr_field(46)

#define __ev_clr_spefscr_mode() __ev_clr_spefscr_field(47)

#define __ev_clr_spefscr_sov() __ev_clr_spefscr_field(48)
#define __ev_clr_spefscr_ov() __ev_clr_spefscr_field(49)
#define __ev_clr_spefscr_fg() __ev_clr_spefscr_field(50)
#define __ev_clr_spefscr_fx() __ev_clr_spefscr_field(51)
#define __ev_clr_spefscr_finv() __ev_clr_spefscr_field(52)
#define __ev_clr_spefscr_fdbz() __ev_clr_spefscr_field(53)
#define __ev_clr_spefscr_funf() __ev_clr_spefscr_field(54)
#define __ev_clr_spefscr_fovf() __ev_clr_spefscr_field(55)
#define __ev_clr_spefscr_finxe() __ev_clr_spefscr_field(57)
#define __ev_clr_spefscr_finve() __ev_clr_spefscr_field(58)
#define __ev_clr_spefscr_fdbze() __ev_clr_spefscr_field(59)
#define __ev_clr_spefscr_funfe() __ev_clr_spefscr_field(60)
#define __ev_clr_spefscr_fovfe() __ev_clr_spefscr_field(61)

inline void __ev_set_spefscr_frmc(register uint32_t rnd)
{
	register uint32_t r;
	asm {.nonvolatile; mfspr r,spefscr;} 
	r = (r & ~0x3) | (rnd & 0x3); 
	asm {.nonvolatile; mtspr spefscr,r;}
}

/*	Additional Library Routines	*/

_EWL_BEGIN_EXTERN_C									

int16_t _EWL_CDECL	strtosfix16(const char *str, char **endptr);
int32_t _EWL_CDECL	strtosfix32(const char *str, char **endptr);
int64_t _EWL_CDECL	strtosfix64(const char *str, char **endptr);
uint16_t _EWL_CDECL	strtoufix16(const char *str, char **endptr);
uint32_t _EWL_CDECL	strtoufix32(const char *str, char **endptr);
uint64_t _EWL_CDECL	strtoufix64(const char *str, char **endptr);
int16_t _EWL_CDECL	atosfix16(const char *str);
int32_t _EWL_CDECL	atosfix32(const char *str);
int64_t _EWL_CDECL	atosfix64(const char *str);
uint16_t _EWL_CDECL	atoufix16(const char *str);
uint32_t _EWL_CDECL	atoufix32(const char *str);
uint64_t _EWL_CDECL	atoufix64(const char *str);

_EWL_END_EXTERN_C

#else	/*__SPE__*/

#warning "This header is only useful for targets using the e500 SPE PIM."

#endif	/*__SPE__*/

#endif

/* Change record:
 * BC  021112 created
 * EJS 030123 merged with msl_core_2
 */
