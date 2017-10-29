#include "asm.h"

	/*	fake errno	*/
	.global	___errno

	.align	2
start:
	/*	Clear Status Reg	*/
	AND	#0, r0
	LDC	r0, sr

	/*	Go !	*/

	MOVL	stack_addr, r15
	MOVL	start_addr, r0
	JSR	@r0
	NOP

	/*	SYSCALL SYS_exit	*/
	mov	#1, r4
	trapa	#34


		.align	2
	/*	Stack is 64M above us	*/
stack_addr:	
	.long	(0x8001000 + 1<<26)
start_addr:
	.long	_main

___errno:
	.long	0
