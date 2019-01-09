#include "asm.h"

        .global _getpsr
        .global _getssr
        .global _getspc
        .global _getgbr
        .global _getvbr
        .global _getsp
        .global _getmach
        .global _getmacl
        .global _getpr
	.global _sleep
	.global _vec_stub_begin
	.global _vec_stub_end
	.global	_splhi
	.global	_spllo
	.global	_spldone


	.align	2
start:
	/*	Clear Status Reg	*/
	AND	#0, r0
	LDC	r0, sr

	/*	Go !	*/
	MOVL	app_stack_addr, r15
	MOVL	start_addr, r0
	JSR	@r0
	NOP

        /*      SYSCALL SYS_exit        */
        mov     #1, r4
        trapa   #34


_splhi:
	MOV	#(1<<4), r0
	SHLL16	r0
	SHLL8	r0
	STC	sr, r1
	OR	r0, r1
	LDC	r1, sr
        RTS
	NOP


_spllo:
	MOV	#(1<<4), r0
	SHLL16	r0
	SHLL8	r0
	NOT	r0, r0
	STC	sr, r1
	AND	r0, r1
	LDC	r1, sr
        RTS
	NOP


_spldone:
	/*	Argument is in R4:	*/
	LDC	r4, sr
        RTS
	NOP


_getpsr:
        STC     sr, r0
        RTS
	NOP


_getssr:
        STC     ssr, r0
        RTS
	NOP


_getspc:
        STC     spc, r0
        RTS
	NOP


_getgbr:
        STC     gbr, r0
        RTS
	NOP


_getvbr:
        STC     vbr, r0
        RTS
	NOP


_getsp:
        MOV     r15, r0
        RTS
	NOP


_getpr:
        STS     pr, r0
        RTS
	NOP


_getmach:
        STS     mach, r0
        RTS
	NOP


_getmacl:
        STS     macl, r0
        RTS
	NOP



	.align	4
_vec_stub_begin:
	/*	     Save PR		*/
	MOVL	savestack_addr, r0
	ADD	#36, r0
	STS.L	pr, @-r0

	/*	   Save R8 - R15	*/
	BSR	saveregs
	NOP

	/*	It's now safe to call	*/
	MOVL	intr_stack_addr, r15
	MOVL	hdlr_addr, r0
	JSR	@r0
	NOP

	/*	   Restore R8 - R15	*/
	BSR	restoreregs
	NOP

	/*	     Restore PR		*/
	MOVL	savestack_addr, r0
	ADD	#32, r0
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
	/*	Addr of bottom of save area	*/
	MOVL	savestack_addr, r0

	/*	  Get addr _end_ of stack	*/
	ADD	#32, r0

	/*	  Now store items bkwrds 	*/
	MOVL	r15, @-r0
	MOVL	r14, @-r0
	MOVL	r13, @-r0
	MOVL	r12, @-r0
	MOVL	r11, @-r0
	MOVL	r10, @-r0
	MOVL	r9, @-r0
	MOVL	r8, @-r0

	RTS
	NOP


	/*								*/
	/*		    SR.RB == 1.  Restore R8-R15. 		*/
	/*								*/
restoreregs:
	/*	Addr of bottom of save area	*/
	MOVL	savestack_addr, r0

	/*	    Pop into approp. reg	*/
	MOVL	@r0+, r8
	MOVL	@r0+, r9
	MOVL	@r0+, r10
	MOVL	@r0+, r11
	MOVL	@r0+, r12
	MOVL	@r0+, r13
	MOVL	@r0+, r14
	MOVL	@r0+, r15

	RTS
	NOP

	.align	4
	hdlr_addr:
	.long	_intr_hdlr

	.align	4
	savestack_addr:
	.long	_REGSAVESTACK

	.align	4
	intr_stack_addr:
	.long	(0x8000000 + (1 << 21))
_vec_stub_end:



_sleep:
	SLEEP
	RTS
	NOP



	.align	2
app_stack_addr:	
	.long	(0x8000000 + (1 << 20))

	.align	2
start_addr:
	.long	_main
