/*									*/
/*		  Hitachi SH specific assembler routines		*/
/*									*/
/*	See section 3-112 of rej05b0463_superh.pdf for some ABI 	*/
/*	information (this is a new Renesas doc), and also our 		*/
/*	comments for syscalls in op-hitachi-sh.c. See also		*/
/*		newlib-1.9.0/newlib/libc/machine/sh/setjmp.S		*/
/*	The above setjmp can't (???) be used to correctly switch	*/
/*	threads, since, e.g., T bit (in SR) is not saved, neither	*/
/*	are MACH and MACL, with partial results of an ongoing 		*/
/*	computation.							*/
/*									*/
#define	SIMAGIC		1
#define	SEMAGIC		2
#define	TRA_ADDR	0xFFFFFFD0

	/*								*/
	/*	While _tas and _restoreregs can/should be called	*/
	/*	from C code, saveregs0 must be called from intrhdlr	*/
	/*	(see comments at saveregs0 and _restoreregs below)	*/
	/*	_saveregs is called syncronously, and it raises a	*/
	/*	SW exception, which leads to mode switch and then	*/
	/*	eventually saveregs0 executing.				*/
	/*								*/
	.global	_spthr_tas
	.global	_spthr_saveregs
	.global	_spthr_restoreregs
	.global	_spthr_getcallerpc
	.global	_spthr_getcallerstk
	.global	_spthr_dosleep
	.global	_spthr_splget
	.global	_spthr_splset
	.global	_spthr_splhi
	.global	_spthr_spllo
	.global _intr_vec_stub_begin
	.global _intr_vec_stub_end
	.global	_sleep



	.align	2
_sleep:
_spthr_dosleep:
	SLEEP

	RTS
	NOP


_spthr_splhi:
	MOV.L	splhi_mask, r0
	STC	sr, r1
	OR	r0, r1
	LDC	r1, sr

	RTS
	NOP


_spthr_spllo:
	MOV.L	splhi_mask, r0
	NOT	r0, r0
	STC	sr, r1
	AND	r0, r1
	LDC	r1, sr

	RTS
	NOP


_spthr_splget:
	STC	sr, r0

	RTS
	NOP


_spthr_splset:
	LDC	r4, sr

	RTS
	NOP


_spthr_getcallerpc:
	MOVA	@(0, PC), r0
	MOV	#4, r1
	SUB	r1, r0

	RTS
	NOP


_spthr_getcallerstk:
	MOV	r15, r0
	
	RTS
	NOP


_spthr_tas:
	TAS.B	@r4
	STC	sr, r0
	AND	#1, r0
	RTS
	NOP

_spthr_saveregs:
	TRAPA	#99

	RTS
	NOP


saveregs0:
	/*								*/
	/*	Arguments: in R4, has addr of stack to save regs to.	*/
	/*	Called from intr/excp handlr.				*/
	/*		Because we don't want to clobber the extant 	*/
	/*	PR, must be called before any other subroutine call,	*/
	/*	and must not be called via a BSR. Thus, we assume 	*/
	/*	we're called via a JMP or BRA*, and we always return	*/
	/*	via BRA to label SAVEREGS_RETURN.			*/
	/*								*/
	/*	Need to save all regs, because thread might have been 	*/
	/*	asynchronously interrupted, as opposed to synchronous 	*/
	/*	entry in the case of a setjmp-style state-saving, 	*/
	/*	where we'd only have to save the callee-saved regs.	*/
	/*								*/

	/*								*/
	/*	R4 is banked R4, so first copy in the user-space R4	*/
	/*	BANK0 regs accesed via STC when SR.RB == 1, vice versa	*/
	/*								*/
	STC	r4_bank, r4
	ADD	#96, r4

	/*		System registers	*/	
	STS.L	mach,	@-r4
	STS.L	macl,	@-r4
	STS.L	pr,	@-r4

	/*		Control Registers	*/
	STC.L	gbr,	@-r4
	STC.L	spc,	@-r4
	STC.L	sr,	@-r4
	STC.L	ssr,	@-r4
	STC.L	vbr,	@-r4

	/*								*/
	/*	SR.RB == 1 when we are called, because we are either	*/
	/*	being called from timer interrupt handler, or due to	*/
	/*	a software-initiated synchronous exception. Therefore	*/
	/*	have to save all registers, including the user-bank	*/
	/*	(when SR.RB == 1, r0--r8 are from the system-bank. 	*/
	/*								*/
	/*	We store items in revers so that we can later use 	*/
	/*	MOV.L Rm, @-Rn, rather than have to incr the memory	*/
	/*	store address ourselves.				*/
	/*								*/
	MOV.L	r15, @-r4
	MOV.L	r14, @-r4
	MOV.L	r13, @-r4
	MOV.L	r12, @-r4
	MOV.L	r11, @-r4
	MOV.L	r10, @-r4
	MOV.L	r9, @-r4
	MOV.L	r8, @-r4

	/*					*/
	/*	BANK0 regs accesed via STC	*/
	/*	when SR.RB == 1, vice versa	*/
	/*					*/
	STC.L	r7_bank, @-r4
	STC.L	r6_bank, @-r4
	STC.L	r5_bank, @-r4
	STC.L	r4_bank, @-r4
	STC.L	r3_bank, @-r4
	STC.L	r2_bank, @-r4
	STC.L	r1_bank, @-r4
	STC.L	r0_bank, @-r4

	/*	Check whether called from intr hdlr or excp hdlr	*/
	CMP/EQ	#SIMAGIC, r0
//	BT	saveregs_intr_return
//	BRA	saveregs_excp_return
	NOP



_spthr_restoreregs:
	/*								*/
	/*	Argument: R4, has addr of stack of saved regs. Call	*/
	/*	to restoreregs does not return to caller;  instead it	*/
	/*	switches to saved thread.				*/
	/*								*/
	/*	Recall that SR.RB == 1 (see _savergs above).  Pop 	*/
	/*	in reverse order. We don't restore PC directly,		*/
	/*	instead restore saved SSR-->SSR, and saved SPC-->SPC	*/
	/*	Then, when RTE happens, machine will automatically	*/
	/*	does PC = SPC, SR = SSR. Note that PR has to be 	*/
	/*	restored normally, since we have nothing to do with	*/
	/*	it: we are always entered via exception or interrupt,	*/
	/*	so PR is the valid PR before excp/intr, and we exit	*/
	/*	via RTE, so PR is not used. 				*/
	/*								*/
	/*					*/
	/*	BANK0 regs accesed via LDC	*/
	/*	when SR.RB == 1, vice versa	*/
	/*					*/
	LDC.L	@r4+, r0_bank
	LDC.L	@r4+, r1_bank
	LDC.L	@r4+, r2_bank
	LDC.L	@r4+, r3_bank
	LDC.L	@r4+, r4_bank
	LDC.L	@r4+, r5_bank
	LDC.L	@r4+, r6_bank
	LDC.L	@r4+, r7_bank

	MOV.L	@r4+, r8
	MOV.L	@r4+, r9
	MOV.L	@r4+, r10
	MOV.L	@r4+, r11
	MOV.L	@r4+, r12
	MOV.L	@r4+, r13
	MOV.L	@r4+, r14
	MOV.L	@r4+, r15

	/*		Control Registers	*/
	LDC.L	@r4+, vbr
	LDC.L	@r4+, ssr
	LDC.L	@r4+, sr
	LDC.L	@r4+, spc
	LDC.L	@r4+, gbr

	/*		System registers	*/
	LDS.L	@r4+, pr
	LDS.L	@r4+, macl
	LDS.L	@r4+, mach

	/*								*/
	/*	Note: we exit via RTE, effecting a switch to old	*/
	/*	context.						*/
	/*								*/
	RTE
	NOP




	/*								*/
	/*	Handlers for Software traps are installed by the	*/
	/*	library initialization at VBR + 0x100			*/
	/*								*/
_excp_vec_stub_begin:
	/*								*/
	/*	If exception number is SAVEREGS_EXCP_CODE, set r0	*/
	/*	to SAVEREGS_EXCP_RETURN, and then jump to saveregs0.	*/
	/*	Otherwise, we've got an unhandled exception: just RTE	*/
	/*	Note that the exception code is defined by arch. to	*/
	/*	have been quadrupled. NOTE: TRA is a memmapped reg;	*/
	/*								*/
        MOV.L	tra_addr, r1
        MOV.L	@r1, r0
	SHLR2	r0
	CMP/EQ	#SAVEREGS_EXCP_CODE, r0
	BT	do_saveregs0
	RTE
	NOP

do_saveregs0:
	MOV	#SEMAGIC, r0
	BRA	saveregs0
	NOP
_excp_vec_stub_end:



	/*								*/
	/*	Handlers for timer interrupts are installed by the	*/
	/*	library initialization at VBR + 0x600			*/
	/*								*/
	.align	4
_intr_vec_stub_begin:
	/*	Determine thread ID	*/
        MOV.L	threadid_addr, r1
        MOV.L	@r1, r0

	MOV.L	regstackptrs_addr, r2
	SHLL2	r0
	ADD	r0, r2
	MOV.L	@r2, r0
	ADD	#36, r0

	/*	     Save PR		*/
	STS.L	pr, @-r0

	/*	   Save R8 - R15	*/
	BSR	saveregs
	NOP

	/*	It's now safe to call	*/
	MOV.L	hdlr_addr, r0
	JSR	@r0
	NOP

	/*	   Restore R8 - R15	*/
	BSR	restoreregs
	NOP

	/*	     Restore PR		*/
	LDS.L	@r0+, pr

	/*	Return from exception	*/
	RTE
	NOP


	/*								*/
	/*	SR.RB == 1. Save R8-R15. We store items in reverse	*/
	/*	so that we can use MOV.L Rm, @-Rn, rather than have	*/
	/*	to incr the memory store address ourselves.		*/
	/*								*/
saveregs:
	/*	  Now store items bkwrds 	*/
	MOV.L	r15, @-r0
	MOV.L	r14, @-r0
	MOV.L	r13, @-r0
	MOV.L	r12, @-r0
	MOV.L	r11, @-r0
	MOV.L	r10, @-r0
	MOV.L	r9, @-r0
	MOV.L	r8, @-r0

	RTS
	NOP


	/*								*/
	/*		    SR.RB == 1.  Restore R8-R15. 		*/
	/*								*/
restoreregs:
	/*	Determine thread ID	*/
        MOV.L	threadid_addr, r1
        MOV.L	@r1, r0

	MOV.L	regstackptrs_addr, r2
	SHLL2	r0
	ADD	r0, r2
	MOV.L	@r2, r0

	/*	    Pop into approp. reg	*/
	MOV.L	@r0+, r8
	MOV.L	@r0+, r9
	MOV.L	@r0+, r10
	MOV.L	@r0+, r11
	MOV.L	@r0+, r12
	MOV.L	@r0+, r13
	MOV.L	@r0+, r14
	MOV.L	@r0+, r15

	RTS
	NOP

	/*						*/
	/*	Always sets SR.RB;  Our convention	*/
	/*	Sets MD,RB,BL and all bits of IMASK	*/
	/*						*/
	.align	2
		splhi_mask:	
			.long	0x700000F0
		tra_addr:
			.long   TRA_ADDR
		threadid_addr:
			.long   THREADID_ADDR
		hdlr_addr:
			.long	_intr_hdlr
		regstackptrs_addr:
			.long	_spthr_regstackptrs
_intr_vec_stub_end:







