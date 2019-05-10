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

/*
 * 	We store a nonsense value in fa1 with the
 *	correct uncertainty.
 *
 *	d/dx( atan(x) ) = 1 / (x^2 + 1)
 *
 */
atan2unf:
	addi 		sp, sp, -16
	sw 		ra, 12(sp)
	unfsw		fa0, 8(sp), 8(sp)
	unfsw		fa1, 4(sp), 4(sp)
	call 		atan2f
	unflw		fa1, 8(sp), 8(sp)
	unflw		fa2, 4(sp), 4(sp)
	unfdiv.s	fa4, fa1, fa2
	li		a0, 1
	fcvt.s.w	fa3, a0
	fmul.s		fa2, fa4, fa4
	fadd.s		fa2, fa2, fa3
	fdiv.s		fa2, fa3, fa2
	unupg.s		fa1, fa1, fa4
	uncpvar.s	fa0, fa1
	lw 		ra, 12(sp)
	addi 		sp, sp, 16
	ret


/*
 *	We store a nonsense value in fa1 with the
 *	correct uncertainty.
 *
 *	d/dx( atan(x) ) = 1 / sqrt(1 - x^2)
 *
 */
asinunf:
	addi 		sp, sp, -16
	sw 		ra, 12(sp)
	unfsw		fa0, 8(sp), 8(sp)
	call 		asinf
	unflw		fa1, 8(sp), 8(sp)
	li		a0, 1
	fcvt.s.w	fa3, a0
	fmul.s		fa4, fa1, fa1
	fsub.s		fa4, fa3, fa4
	fsqrt.s		fa4, fa4
	unupg.s		fa1, fa1, fa4

	uncpvar.s	fa0, fa1
	lw 		ra, 12(sp)
	addi 		sp, sp, 16
	ret
