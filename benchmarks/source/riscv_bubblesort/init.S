.globl _start
.globl __errno
.align	4

_start:
init:
.option push
.option norelax
	la		gp, __global_pointer$
.option pop
	# j init;
	call	main;
	call	uncertain_exper;
	call 	exit;

__errno:
	.long	0

.fp_5:
  .word 1084227584

uncertain_exper:
	lui 	a5, %hi(.fp_5);
	flw 	fa0, %lo(.fp_5)(a5);
	unsvar.s fa0, fa0;

	unfadd.s	fa1, fa0, fa0;

	unfmul.s	fa2, fa1, fa0;

	li		t0, 19;
	fcvt.s.w fa0, t0;
	uncvar.s fa0;

	unfdiv.s	fa3, fa2, fa0;

	fmv.s fa4, fa3;

