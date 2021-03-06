/*	$NetBSD: frame.h,v 1.29 2012/08/01 22:46:07 matt Exp $	*/

/*
 * Copyright (c) 1994-1997 Mark Brinicombe.
 * Copyright (c) 1994 Brini.
 * All rights reserved.
 *
 * This code is derived from software written for Brini by Mark Brinicombe
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Brini.
 * 4. The name of the company nor the name of the author may be used to
 *    endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY BRINI ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL BRINI OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * RiscBSD kernel project
 *
 * frame.h
 *
 * Stack frames structures
 *
 * Created      : 30/09/94
 */

#ifndef _ARM32_FRAME_H_
#define _ARM32_FRAME_H_

#include <arm/frame.h>		/* Common ARM stack frames */

#ifndef _LOCORE

/*
 * System stack frames.
 */

typedef struct irqframe {
	unsigned int if_spsr;
	unsigned int if_fill;	/* fill here so r0 will dword aligned */
	unsigned int if_r0;
	unsigned int if_r1;
	unsigned int if_r2;
	unsigned int if_r3;
	unsigned int if_r4;
	unsigned int if_r5;
	unsigned int if_r6;
	unsigned int if_r7;
	unsigned int if_r8;
	unsigned int if_r9;
	unsigned int if_r10;
	unsigned int if_r11;
	unsigned int if_r12;
	unsigned int if_usr_sp;
	unsigned int if_usr_lr;
	unsigned int if_svc_sp;
	unsigned int if_svc_lr;
	unsigned int if_pc;
} irqframe_t;

struct clockframe {
	struct irqframe cf_if;
};

/*
 * Switch frame.
 *
 * Should be a multiple of 8 bytes for dumpsys.
 */

struct switchframe {
	u_int	sf_r4;
	u_int	sf_r5;
	u_int	sf_r6;
	u_int	sf_r7;
	u_int	sf_sp;
	u_int	sf_pc;
};
 
/*
 * Stack frame. Used during stack traces (db_trace.c)
 */
struct frame {
	u_int	fr_fp;
	u_int	fr_sp;
	u_int	fr_lr;
	u_int	fr_pc;
};

#ifdef _KERNEL
void validate_trapframe(trapframe_t *, int);
#endif /* _KERNEL */

#else /* _LOCORE */

#include "opt_compat_netbsd.h"
#include "opt_execfmt.h"
#include "opt_multiprocessor.h"
#include "opt_cpuoptions.h"
#include "opt_arm_debug.h"
#include "opt_cputypes.h"

#include <machine/cpu.h>

/*
 * This macro is used by DO_AST_AND_RESTORE_ALIGNMENT_FAULTS to process
 * any pending softints.
 */
#ifdef __HAVE_FAST_SOFTINTS
#define	DO_PENDING_SOFTINTS						\
	ldr	r0, [r4, #CI_INTR_DEPTH]/* Get current intr depth */	;\
	teq	r0, #0			/* Test for 0. */		;\
	bne	10f			/*   skip softints if != 0 */	;\
	ldr	r0, [r4, #CI_CPL]	/* Get current priority level */;\
	ldr	r1, [r4, #CI_SOFTINTS]	/* Get pending softint mask */	;\
	movs	r0, r1, lsr r0		/* shift mask by cpl */		;\
	blne	_C_LABEL(dosoftints)	/* dosoftints(void) */		;\
10:
#else
#define	DO_PENDING_SOFTINTS		/* nothing */
#endif

/*
 * AST_ALIGNMENT_FAULT_LOCALS and ENABLE_ALIGNMENT_FAULTS
 * These are used in order to support dynamic enabling/disabling of
 * alignment faults when executing old a.out ARM binaries.
 *
 * Note that when ENABLE_ALIGNMENTS_FAULTS finishes r4 will contain
 * pointer to the cpu's cpu_info.  DO_AST_AND_RESTORE_ALIGNMENT_FAULTS
 * relies on r4 being preserved.
 */
#ifdef EXEC_AOUT
#define	AST_ALIGNMENT_FAULT_LOCALS					\
.Laflt_cpufuncs:							;\
	.word	_C_LABEL(cpufuncs)

/*
 * This macro must be invoked following PUSHFRAMEINSVC or PUSHFRAME at
 * the top of interrupt/exception handlers.
 *
 * When invoked, r0 *must* contain the value of SPSR on the current
 * trap/interrupt frame. This is always the case if ENABLE_ALIGNMENT_FAULTS
 * is invoked immediately after PUSHFRAMEINSVC or PUSHFRAME.
 */
#define	ENABLE_ALIGNMENT_FAULTS						\
	and	r0, r0, #(PSR_MODE)	/* Test for USR32 mode */	;\
	teq	r0, #(PSR_USR32_MODE)					;\
	GET_CURCPU(r4)			/* r4 = cpuinfo */		;\
	bne	1f			/* Not USR mode skip AFLT */	;\
	ldr	r1, [r4, #CI_CURPCB]	/* get curpcb from cpu_info */	;\
	ldr	r1, [r1, #PCB_FLAGS]	/* Fetch curpcb->pcb_flags */	;\
	tst	r1, #PCB_NOALIGNFLT					;\
	beq	1f			/* AFLTs already enabled */	;\
	ldr	r2, .Laflt_cpufuncs					;\
	ldr	r1, [r4, #CI_CTRL]	/* Fetch control register */	;\
	mov	r0, #-1							;\
	mov	lr, pc							;\
	ldr	pc, [r2, #CF_CONTROL]	/* Enable alignment faults */	;\
1:

/*
 * This macro must be invoked just before PULLFRAMEFROMSVCANDEXIT or
 * PULLFRAME at the end of interrupt/exception handlers.  We know that
 * r4 points to cpu_info since that is what ENABLE_ALIGNMENT_FAULTS did
 * for use.
 */
#define	DO_AST_AND_RESTORE_ALIGNMENT_FAULTS				\
	DO_PENDING_SOFTINTS						;\
	ldr	r0, [sp]		/* Get the SPSR from stack */	;\
	mrs	r5, cpsr		/* save CPSR */			;\
	orr	r1, r5, #(IF32_bits)					;\
	msr	cpsr_c, r1		/* Disable interrupts */	;\
	and	r0, r0, #(PSR_MODE)	/* Returning to USR mode? */	;\
	teq	r0, #(PSR_USR32_MODE)					;\
	bne	3f			/* Nope, get out now */		;\
1:	ldr	r1, [r4, #CI_ASTPENDING] /* Pending AST? */		;\
	teq	r1, #0x00000000						;\
	bne	2f			/* Yup. Go deal with it */	;\
	ldr	r1, [r4, #CI_CURPCB]	/* Get current PCB */		;\
	ldr	r0, [r1, #PCB_FLAGS]	/* Fetch curpcb->pcb_flags */	;\
	tst	r0, #PCB_NOALIGNFLT					;\
	beq	3f			/* Keep AFLTs enabled */	;\
	ldr	r1, [r4, #CI_CTRL]	/* Fetch control register */	;\
	ldr	r2, .Laflt_cpufuncs					;\
	mov	r0, #-1							;\
	bic	r1, r1, #CPU_CONTROL_AFLT_ENABLE  /* Disable AFLTs */	;\
	adr	lr, 3f							;\
	ldr	pc, [r2, #CF_CONTROL]	/* Set new CTRL reg value */	;\
	/* NOTREACHED */						\
2:	mov	r1, #0x00000000						;\
	str	r1, [r4, #CI_ASTPENDING] /* Clear astpending */		;\
	bic	r5, r5, #(IF32_bits)				;\
	msr	cpsr_c, r5		/* Restore interrupts */	;\
	mov	r0, sp							;\
	bl	_C_LABEL(ast)		/* ast(frame) */		;\
	orr	r0, r5, #(IF32_bits)	/* Disable IRQs */		;\
	msr	cpsr_c, r0						;\
	b	1b			/* Back around again */		;\
3:

#else	/* !EXEC_AOUT */

#define	AST_ALIGNMENT_FAULT_LOCALS

#define	ENABLE_ALIGNMENT_FAULTS		GET_CURCPU(r4)

#define	DO_AST_AND_RESTORE_ALIGNMENT_FAULTS				\
	DO_PENDING_SOFTINTS						;\
	ldr	r0, [sp]		/* Get the SPSR from stack */	;\
	mrs	r5, cpsr		/* save CPSR */			;\
	orr	r1, r5, #(IF32_bits)					;\
	msr	cpsr_c, r1		/* Disable interrupts */	;\
	and	r0, r0, #(PSR_MODE)	/* Returning to USR mode? */	;\
	teq	r0, #(PSR_USR32_MODE)					;\
	bne	2f			/* Nope, get out now */		;\
1:	ldr	r1, [r4, #CI_ASTPENDING] /* Pending AST? */		;\
	teq	r1, #0x00000000						;\
	beq	2f			/* Nope. Just bail */		;\
	mov	r1, #0x00000000						;\
	str	r1, [r4, #CI_ASTPENDING] /* Clear astpending */		;\
	bic	r5, r5, #(IF32_bits)					;\
	msr	cpsr_c, r5		/* Restore interrupts */	;\
	mov	r0, sp							;\
	bl	_C_LABEL(ast)		/* ast(frame) */		;\
	orr	r0, r5, #(IF32_bits)	/* Disable IRQs */		;\
	msr	cpsr_c, r0						;\
	b	1b							;\
2:
#endif /* EXEC_AOUT */

#ifdef ARM_LOCK_CAS_DEBUG
#define	LOCK_CAS_DEBUG_LOCALS						 \
.L_lock_cas_restart:							;\
	.word	_C_LABEL(_lock_cas_restart)

#if defined(__ARMEB__)
#define	LOCK_CAS_DEBUG_COUNT_RESTART					 \
	ble	99f							;\
	ldr	r0, .L_lock_cas_restart					;\
	ldmia	r0, {r1-r2}		/* load ev_count */		;\
	adds	r2, r2, #1		/* 64-bit incr (lo) */		;\
	adc	r1, r1, #0		/* 64-bit incr (hi) */		;\
	stmia	r0, {r1-r2}		/* store ev_count */
#else /* __ARMEB__ */
#define	LOCK_CAS_DEBUG_COUNT_RESTART					 \
	ble	99f							;\
	ldr	r0, .L_lock_cas_restart					;\
	ldmia	r0, {r1-r2}		/* load ev_count */		;\
	adds	r1, r1, #1		/* 64-bit incr (lo) */		;\
	adc	r2, r2, #0		/* 64-bit incr (hi) */		;\
	stmia	r0, {r1-r2}		/* store ev_count */
#endif /* __ARMEB__ */
#else /* ARM_LOCK_CAS_DEBUG */
#define	LOCK_CAS_DEBUG_LOCALS		/* nothing */
#define	LOCK_CAS_DEBUG_COUNT_RESTART	/* nothing */
#endif /* ARM_LOCK_CAS_DEBUG */

#define	LOCK_CAS_CHECK_LOCALS						 \
.L_lock_cas:								;\
	.word	_C_LABEL(_lock_cas)					;\
.L_lock_cas_end:							;\
	.word	_C_LABEL(_lock_cas_end)					;\
LOCK_CAS_DEBUG_LOCALS

#define	LOCK_CAS_CHECK							 \
	ldr	r0, [sp]		/* get saved PSR */		;\
	and	r0, r0, #(PSR_MODE)	/* check for SVC32 mode */	;\
	teq	r0, #(PSR_SVC32_MODE)					;\
	bne	99f			/* nope, get out now */		;\
	ldr	r0, [sp, #(IF_PC)]					;\
	ldr	r1, .L_lock_cas_end					;\
	cmp	r0, r1							;\
	bge	99f							;\
	ldr	r1, .L_lock_cas						;\
	cmp	r0, r1							;\
	strgt	r1, [sp, #(IF_PC)]					;\
	LOCK_CAS_DEBUG_COUNT_RESTART					;\
99:

/*
 * ASM macros for pushing and pulling trapframes from the stack
 *
 * These macros are used to handle the irqframe and trapframe structures
 * defined above.
 */

/*
 * PUSHFRAME - macro to push a trap frame on the stack in the current mode
 * Since the current mode is used, the SVC lr field is not defined.
 */

#ifdef CPU_SA110
/*
 * NOTE: r13 and r14 are stored separately as a work around for the
 * SA110 rev 2 STM^ bug
 */
#define	PUSHUSERREGS							   \
	stmia	sp, {r0-r12};		/* Push the user mode registers */ \
	add	r0, sp, #(4*13);	/* Adjust the stack pointer */	   \
	stmia	r0, {r13-r14}^		/* Push the user mode registers */
#else
#define	PUSHUSERREGS							   \
	stmia	sp, {r0-r14}^		/* Push the user mode registers */
#endif

#define PUSHFRAME							   \
	str	lr, [sp, #-4]!;		/* Push the return address */	   \
	sub	sp, sp, #(4*17);	/* Adjust the stack pointer */	   \
	PUSHUSERREGS;			/* Push the user mode registers */ \
	mov     r0, r0;                 /* NOP for previous instruction */ \
	mrs	r0, spsr_all;		/* Get the SPSR */		   \
	str	r0, [sp, #-8]!		/* Push the SPSR on the stack */

/*
 * PULLFRAME - macro to pull a trap frame from the stack in the current mode
 * Since the current mode is used, the SVC lr field is ignored.
 */

#define PULLFRAME							   \
	ldr     r0, [sp], #0x0008;      /* Pop the SPSR from stack */	   \
	msr     spsr_all, r0;						   \
	ldmia   sp, {r0-r14}^;		/* Restore registers (usr mode) */ \
	mov     r0, r0;                 /* NOP for previous instruction */ \
	add	sp, sp, #(4*17);	/* Adjust the stack pointer */	   \
 	ldr	lr, [sp], #0x0004	/* Pop the return address */

/*
 * PUSHFRAMEINSVC - macro to push a trap frame on the stack in SVC32 mode
 * This should only be used if the processor is not currently in SVC32
 * mode. The processor mode is switched to SVC mode and the trap frame is
 * stored. The SVC lr field is used to store the previous value of
 * lr in SVC mode.  
 *
 * NOTE: r13 and r14 are stored separately as a work around for the
 * SA110 rev 2 STM^ bug
 */

#ifdef _ARM_ARCH_6
#define	SET_CPSR_MODE(tmp, mode)	\
	cps	#(mode)
#else
#define	SET_CPSR_MODE(tmp, mode)	\
	mrs     tmp, cpsr; 		/* Get the CPSR */		   \
	bic     tmp, tmp, #(PSR_MODE);	/* Fix for SVC mode */		   \
	orr     tmp, tmp, #(mode);					   \
	msr     cpsr_c, tmp		/* Punch into SVC mode */
#endif

#define PUSHFRAMEINSVC							   \
	stmdb	sp, {r0-r3};		/* Save 4 registers */		   \
	mov	r0, lr;			/* Save xxx32 r14 */		   \
	mov	r1, sp;			/* Save xxx32 sp */		   \
	mrs	r3, spsr;		/* Save xxx32 spsr */		   \
	SET_CPSR_MODE(r2, PSR_SVC32_MODE);				   \
	bic	r2, sp, #7;		/* Align new SVC sp */		   \
	str	r0, [r2, #-4]!;		/* Push return address */	   \
	stmdb	r2!, {sp, lr};		/* Push SVC sp, lr */		   \
	mov	sp, r2;			/* Keep stack aligned */	   \
	msr     spsr_all, r3;		/* Restore correct spsr */	   \
	ldmdb	r1, {r0-r3};		/* Restore 4 regs from xxx mode */ \
	sub	sp, sp, #(4*15);	/* Adjust the stack pointer */	   \
	PUSHUSERREGS;			/* Push the user mode registers */ \
	mov     r0, r0;                 /* NOP for previous instruction */ \
	mrs	r0, spsr_all;		/* Get the SPSR */		   \
	str	r0, [sp, #-8]!		/* Push the SPSR onto the stack */

/*
 * PULLFRAMEFROMSVCANDEXIT - macro to pull a trap frame from the stack
 * in SVC32 mode and restore the saved processor mode and PC.
 * This should be used when the SVC lr register needs to be restored on
 * exit.
 */

#define PULLFRAMEFROMSVCANDEXIT						   \
	ldr     r0, [sp], #0x0008;	/* Pop the SPSR from stack */	   \
	msr     spsr_all, r0;		/* restore SPSR */		   \
	ldmia   sp, {r0-r14}^;		/* Restore registers (usr mode) */ \
	mov     r0, r0;	  		/* NOP for previous instruction */ \
	add	sp, sp, #(4*15);	/* Adjust the stack pointer */	   \
	ldmia	sp, {sp, lr, pc}^	/* Restore lr and exit */

#endif /* _LOCORE */

#endif /* _ARM32_FRAME_H_ */
