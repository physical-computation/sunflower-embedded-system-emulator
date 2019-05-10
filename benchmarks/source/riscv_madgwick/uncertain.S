.globl gcovunf
.globl best_guessunf
.globl svarunf
.globl cvarunf
.globl atan2unf
.globl asinunf

.globl sqrtunf
.globl atan2unf

gcovunf:
	ungcov.s fa0, fa0, fa1;
	ret;

best_guessunf:
	ret;

svarunf:
	unsvar.s fa0, fa1;
	ret;

cvarunf:
	unclvar.s fa0;
	ret;

sqrtunf:
	unfsqrt.s fa0, fa0;
	ret;

atan2unf:
	addi 		sp, sp, -16
	sw 		ra, 12(sp)
	/*
	 * 	store a nonsense value in fs0 with the
	 *	correct uncertainty.
	 *
	 *	d/dx( atan(x) ) = 1 / (x^2 + 1)
	 *
	 */
	fsw		fs0, 8(sp)
	unfdiv.s	fs0, fa0, fa1
	li		a0, 1
	fcvt.s.w	fa3, a0
	fmul.s		fa4, fs0, fs0
	fadd.s		fa4, fa4, fa3
	fdiv.s		fa4, fa3, fa4
	unupg.s		fs0, fs0, fa4

	call 		atan2f
	uncpvar.s	fa0, fs0
	flw 		fs0, 8(sp)
	lw 		ra, 12(sp)
	addi 		sp, sp, 16
	ret

asinunf:
	addi 		sp, sp, -16
	sw 		ra, 12(sp)
	/*
	 * 	store a nonsense value in fs0 with the
	 *	correct uncertainty.
	 *
	 *	d/dx( atan(x) ) = 1 / sqrt(1 - x^2)
	 *
	 */
	fsw		fs0, 8(sp)
	unfdiv.s	fs0, fa0, fa1
	li		a0, 1
	fcvt.s.w	fa3, a0
	fmul.s		fa4, fs0, fs0
	fsub.s		fa4, fa3, fa4
	fsqrt.s		fa4, fa4
	unupg.s		fs0, fs0, fa4

	call 		asinf
	uncpvar.s	fa0, fs0
	flw 		fs0, 8(sp)
	lw 		ra, 12(sp)
	addi 		sp, sp, 16
	ret
