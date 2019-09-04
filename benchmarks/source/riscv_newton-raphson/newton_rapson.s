	.file	"newton_rapson.c"
	.option nopic
# GNU C17 (GCC) version 8.2.0 (riscv32-elf)
#	compiled by GNU C version 7.4.0, GMP version 6.1.2, MPFR version 4.0.1, MPC version 1.1.0, isl version none
# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed:  -I /home/samuelmswong/harrysun/tools/riscv/include
# -iprefix /home/samuelmswong/harrysun/sunflower-toolchain/tools/riscv/bin/../lib/gcc/riscv32-elf/8.2.0/
# -D M32 newton_rapson.c -march=rv32ifd -mabi=ilp32d
# -auxbase-strip newton_rapson.s -g -O0 -Wall -fverbose-asm
# options enabled:  -faggressive-loop-optimizations -fauto-inc-dec
# -fchkp-check-incomplete-type -fchkp-check-read -fchkp-check-write
# -fchkp-instrument-calls -fchkp-narrow-bounds -fchkp-optimize
# -fchkp-store-bounds -fchkp-use-static-bounds
# -fchkp-use-static-const-bounds -fchkp-use-wrappers -fcommon
# -fdelete-null-pointer-checks -fearly-inlining
# -feliminate-unused-debug-types -ffp-int-builtin-inexact -ffunction-cse
# -fgcse-lm -fgnu-runtime -fgnu-unique -fident -finline-atomics
# -fira-hoist-pressure -fira-share-save-slots -fira-share-spill-slots
# -fivopts -fkeep-static-consts -fleading-underscore -flifetime-dse
# -flto-odr-type-merging -fmath-errno -fmerge-debug-strings -fpeephole
# -fplt -fprefetch-loop-arrays -freg-struct-return
# -fsched-critical-path-heuristic -fsched-dep-count-heuristic
# -fsched-group-heuristic -fsched-interblock -fsched-last-insn-heuristic
# -fsched-rank-heuristic -fsched-spec -fsched-spec-insn-heuristic
# -fsched-stalled-insns-dep -fschedule-fusion -fsemantic-interposition
# -fshow-column -fshrink-wrap-separate -fsigned-zeros
# -fsplit-ivs-in-unroller -fssa-backprop -fstdarg-opt
# -fstrict-volatile-bitfields -fsync-libcalls -ftrapping-math
# -ftree-forwprop -ftree-loop-if-convert -ftree-loop-im -ftree-loop-ivcanon
# -ftree-loop-optimize -ftree-parallelize-loops= -ftree-phiprop
# -ftree-reassoc -ftree-scev-cprop -funit-at-a-time -fverbose-asm
# -fzero-initialized-in-bss -mexplicit-relocs -mfdiv -mplt -mstrict-align

	.text
.Ltext0:
	.globl	c
	.section	.sbss,"aw",@nobits
	.align	2
	.type	c, @object
	.size	c, 4
c:
	.zero	4
	.globl	b
	.align	2
	.type	b, @object
	.size	b, 4
b:
	.zero	4
	.text
	.align	2
	.globl	f
	.type	f, @function
f:
.LFB3:
	.file 1 "newton_rapson.c"
	.loc 1 12 1
	addi	sp,sp,-32	#,,
.LCFI0:
	sw	s0,28(sp)	#,
.LCFI1:
	addi	s0,sp,32	#,,
.LCFI2:
	unfsw	fa0,-20(s0),-20(s0)	# x, x
# newton_rapson.c:13: 	return x*x + b*x - c;
	.loc 1 13 10
	unflw	fa4,-20(s0),-20(s0)	# tmp79, x
	unflw	fa5,-20(s0),-20(s0)	# tmp80, x
	unfmul.s	fa4,fa4,fa5	# _1, tmp79, tmp80
# newton_rapson.c:13: 	return x*x + b*x - c;
	.loc 1 13 16
	lui	a5,%hi(b)	# tmp81,
	unflw	fa3,%lo(b)(a5),%lo(b)(a5)	# b.0_2, b
	unflw	fa5,-20(s0),-20(s0)	# tmp82, x
	unfmul.s	fa5,fa3,fa5	# _3, b.0_2, tmp82
# newton_rapson.c:13: 	return x*x + b*x - c;
	.loc 1 13 13
	unfadd.s	fa4,fa4,fa5	# _4, _1, _3
# newton_rapson.c:13: 	return x*x + b*x - c;
	.loc 1 13 19
	lui	a5,%hi(c)	# tmp83,
	unflw	fa5,%lo(c)(a5),%lo(c)(a5)	# c.1_5, c
	unfsub.s	fa5,fa4,fa5	# _8, _4, c.1_5
# newton_rapson.c:14: }
	.loc 1 14 1
	unfmv.s	fa0,fa5	#, <retval>
	lw	s0,28(sp)		#,
.LCFI3:
	addi	sp,sp,32	#,,
.LCFI4:
	jr	ra		#
.LFE3:
	.size	f, .-f
	.align	2
	.globl	df
	.type	df, @function
df:
.LFB4:
	.loc 1 17 1
	addi	sp,sp,-32	#,,
.LCFI5:
	sw	s0,28(sp)	#,
.LCFI6:
	addi	s0,sp,32	#,,
.LCFI7:
	unfsw	fa0,-20(s0),-20(s0)	# x, x
# newton_rapson.c:18: 	return 2*x + b;
	.loc 1 18 10
	unflw	fa5,-20(s0),-20(s0)	# tmp76, x
	unfadd.s	fa4,fa5,fa5	# _1, tmp76, tmp76
# newton_rapson.c:18: 	return 2*x + b;
	.loc 1 18 13
	lui	a5,%hi(b)	# tmp77,
	unflw	fa5,%lo(b)(a5),%lo(b)(a5)	# b.2_2, b
	unfadd.s	fa5,fa4,fa5	# _5, _1, b.2_2
# newton_rapson.c:19: }
	.loc 1 19 1
	unfmv.s	fa0,fa5	#, <retval>
	lw	s0,28(sp)		#,
.LCFI8:
	addi	sp,sp,32	#,,
.LCFI9:
	jr	ra		#
.LFE4:
	.size	df, .-df
	.align	2
	.globl	percentage_error
	.type	percentage_error, @function
percentage_error:
.LFB5:
	.loc 1 21 42
	addi	sp,sp,-48	#,,
.LCFI10:
	sw	ra,44(sp)	#,
	sw	s0,40(sp)	#,
.LCFI11:
	addi	s0,sp,48	#,,
.LCFI12:
	unfsw	fa0,-36(s0),-36(s0)	# x, x
	unfsw	fa1,-40(s0),-40(s0)	# y, y
# newton_rapson.c:22: 	if (x == y) {
	.loc 1 22 5
	unflw	fa4,-36(s0),-36(s0)	# tmp78, x
	unflw	fa5,-40(s0),-40(s0)	# tmp79, y
	feq.s	a5,fa4,fa5	#, tmp80, tmp78, tmp79
	beqz	a5,.L6	#, tmp80,
# newton_rapson.c:23: 		return 0;
	.loc 1 23 10
	fmv.s.x	fa5,zero	# _5,
	j	.L7		#
.L6:
# newton_rapson.c:25: 	float diff = fabsf(x - y);
	.loc 1 25 23
	unflw	fa4,-36(s0),-36(s0)	# tmp81, x
	unflw	fa5,-40(s0),-40(s0)	# tmp82, y
	unfsub.s	fa5,fa4,fa5	# _1, tmp81, tmp82
# newton_rapson.c:25: 	float diff = fabsf(x - y);
	.loc 1 25 8
	fabs.s	fa5,fa5	# tmp83, _1
	unfsw	fa5,-20(s0),-20(s0)	# tmp83, diff
# newton_rapson.c:26: 	float min = fminf(fabsf(x), fabsf(y));
	.loc 1 26 14
	unflw	fa5,-36(s0),-36(s0)	# tmp84, x
	fabs.s	fa4,fa5	# _2, tmp84
	unflw	fa5,-40(s0),-40(s0)	# tmp85, y
	fabs.s	fa5,fa5	# _3, tmp85
	unfmv.s	fa1,fa5	#, _3
	unfmv.s	fa0,fa4	#, _2
	call	fminf		#
	unfsw	fa0,-24(s0),-24(s0)	#, min
# newton_rapson.c:27: 	return diff / min * 100;
	.loc 1 27 14
	unflw	fa4,-20(s0),-20(s0)	# tmp86, diff
	unflw	fa5,-24(s0),-24(s0)	# tmp87, min
	unfdiv.s	fa4,fa4,fa5	# _4, tmp86, tmp87
# newton_rapson.c:27: 	return diff / min * 100;
	.loc 1 27 20
	lui	a5,%hi(.LC0)	# tmp89,
	unflw	fa5,%lo(.LC0)(a5),%lo(.LC0)(a5)	# tmp88,
	unfmul.s	fa5,fa4,fa5	# _5, _4, tmp88
.L7:
# newton_rapson.c:28: }
	.loc 1 28 1
	unfmv.s	fa0,fa5	#, <retval>
	lw	ra,44(sp)		#,
.LCFI13:
	lw	s0,40(sp)		#,
.LCFI14:
	addi	sp,sp,48	#,,
.LCFI15:
	jr	ra		#
.LFE5:
	.size	percentage_error, .-percentage_error
	.section	.rodata
	.align	2
.LC7:
	.string	"%.7f\t%.7f\t%.7f\t%.7f\t%.7f\t%.7f\t%.2f%%\t%.2f%%\t%.2f%%\n"
	.text
	.align	2
	.globl	newton_rapson
	.type	newton_rapson, @function
newton_rapson:
.LFB6:
	.loc 1 30 22
	addi	sp,sp,-176	#,,
.LCFI16:
	sw	ra,172(sp)	#,
	sw	s0,168(sp)	#,
	fsd	fs0,152(sp)	#,
	fsd	fs1,144(sp)	#,
	fsd	fs2,136(sp)	#,
	fsd	fs3,128(sp)	#,
	fsd	fs4,120(sp)	#,
	fsd	fs5,112(sp)	#,
	fsd	fs6,104(sp)	#,
	fsd	fs7,96(sp)	#,
	fsd	fs8,88(sp)	#,
	fsd	fs9,80(sp)	#,
.LCFI17:
	addi	s0,sp,176	#,,
.LCFI18:
# newton_rapson.c:31: 	float x = 0;
	.loc 1 31 8
	sw	zero,-100(s0)	#, x
# newton_rapson.c:32: 	float x_new = x - f(x)/df(x);
	.loc 1 32 20
	unflw	fa0,-100(s0),-100(s0)	#, x
	call	f		#
	unfmv.s	fs0,fa0	# _1,
# newton_rapson.c:32: 	float x_new = x - f(x)/df(x);
	.loc 1 32 25
	unflw	fa0,-100(s0),-100(s0)	#, x
	call	df		#
	unfmv.s	fa5,fa0	# _2,
# newton_rapson.c:32: 	float x_new = x - f(x)/df(x);
	.loc 1 32 24
	unfdiv.s	fa5,fs0,fa5	# _3, _1, _2
# newton_rapson.c:32: 	float x_new = x - f(x)/df(x);
	.loc 1 32 8
	unflw	fa4,-100(s0),-100(s0)	# tmp148, x
	unfsub.s	fa5,fa4,fa5	# tmp147, tmp148, _3
	unfsw	fa5,-104(s0),-104(s0)	# tmp147, x_new
# newton_rapson.c:33: 	float diff = x_new - x;
	.loc 1 33 8
	unflw	fa4,-104(s0),-104(s0)	# tmp150, x_new
	unflw	fa5,-100(s0),-100(s0)	# tmp151, x
	unfsub.s	fa5,fa4,fa5	# tmp149, tmp150, tmp151
	unfsw	fa5,-108(s0),-108(s0)	# tmp149, diff
# newton_rapson.c:35: 	while(fabs(diff) > 1e-5){
	.loc 1 35 7
	j	.L9		#
.L10:
# newton_rapson.c:36: 		x = x_new;
	.loc 1 36 5
	unflw	fa5,-104(s0),-104(s0)	# tmp152, x_new
	unfsw	fa5,-100(s0),-100(s0)	# tmp152, x
# newton_rapson.c:37: 		x_new = x - f(x)/df(x);
	.loc 1 37 15
	unflw	fa0,-100(s0),-100(s0)	#, x
	call	f		#
	unfmv.s	fs0,fa0	# _4,
# newton_rapson.c:37: 		x_new = x - f(x)/df(x);
	.loc 1 37 20
	unflw	fa0,-100(s0),-100(s0)	#, x
	call	df		#
	unfmv.s	fa5,fa0	# _5,
# newton_rapson.c:37: 		x_new = x - f(x)/df(x);
	.loc 1 37 19
	unfdiv.s	fa5,fs0,fa5	# _6, _4, _5
# newton_rapson.c:37: 		x_new = x - f(x)/df(x);
	.loc 1 37 9
	unflw	fa4,-100(s0),-100(s0)	# tmp154, x
	unfsub.s	fa5,fa4,fa5	# tmp153, tmp154, _6
	unfsw	fa5,-104(s0),-104(s0)	# tmp153, x_new
# newton_rapson.c:38: 		diff = x_new - x;
	.loc 1 38 8
	unflw	fa4,-104(s0),-104(s0)	# tmp156, x_new
	unflw	fa5,-100(s0),-100(s0)	# tmp157, x
	unfsub.s	fa5,fa4,fa5	# tmp155, tmp156, tmp157
	unfsw	fa5,-108(s0),-108(s0)	# tmp155, diff
.L9:
# newton_rapson.c:35: 	while(fabs(diff) > 1e-5){
	.loc 1 35 8
	unflw	fa5,-108(s0),-108(s0)	# tmp158, diff
	fabs.s	fa5,fa5	# _7, tmp158
	fcvt.d.s	fa4,fa5	# _8, _7
# newton_rapson.c:35: 	while(fabs(diff) > 1e-5){
	.loc 1 35 7
	lui	a5,%hi(.LC1)	# tmp160,
	fld	fa5,%lo(.LC1)(a5)	# tmp159,
	fgt.d	a5,fa4,fa5	#, tmp161, _8, tmp159
	bnez	a5,.L10	#, tmp161,
# newton_rapson.c:42: 		unf_var(b), unf_var(c), unf_covar(b, c),
	.loc 1 42 3
	lui	a5,%hi(b)	# tmp162,
	unflw	fa5,%lo(b)(a5),%lo(b)(a5)	# b.3_9, b
	unfmv.s	fa0,fa5	#, b.3_9
	call	unf_var		#
	unfmv.s	fa5,fa0	# _10,
# newton_rapson.c:40: 	printf_(
	.loc 1 40 2
	fcvt.d.s	fs6,fa5	# _11, _10
# newton_rapson.c:42: 		unf_var(b), unf_var(c), unf_covar(b, c),
	.loc 1 42 15
	lui	a5,%hi(c)	# tmp163,
	unflw	fa5,%lo(c)(a5),%lo(c)(a5)	# c.4_12, c
	unfmv.s	fa0,fa5	#, c.4_12
	call	unf_var		#
	unfmv.s	fa5,fa0	# _13,
# newton_rapson.c:40: 	printf_(
	.loc 1 40 2
	fcvt.d.s	fs7,fa5	# _14, _13
# newton_rapson.c:42: 		unf_var(b), unf_var(c), unf_covar(b, c),
	.loc 1 42 27
	lui	a5,%hi(b)	# tmp164,
	unflw	fa5,%lo(b)(a5),%lo(b)(a5)	# b.5_15, b
	lui	a5,%hi(c)	# tmp165,
	unflw	fa4,%lo(c)(a5),%lo(c)(a5)	# c.6_16, c
	unfmv.s	fa1,fa4	#, c.6_16
	unfmv.s	fa0,fa5	#, b.5_15
	call	unf_covar		#
	unfmv.s	fa5,fa0	# _17,
# newton_rapson.c:40: 	printf_(
	.loc 1 40 2
	fcvt.d.s	fs8,fa5	# _18, _17
# newton_rapson.c:43: 		unf_var(x), unf_covar(x, b), unf_covar(x, c),
	.loc 1 43 3
	unflw	fa0,-100(s0),-100(s0)	#, x
	call	unf_var		#
	unfmv.s	fa5,fa0	# _19,
# newton_rapson.c:40: 	printf_(
	.loc 1 40 2
	fcvt.d.s	fs0,fa5	# _20, _19
# newton_rapson.c:43: 		unf_var(x), unf_covar(x, b), unf_covar(x, c),
	.loc 1 43 15
	lui	a5,%hi(b)	# tmp166,
	unflw	fa5,%lo(b)(a5),%lo(b)(a5)	# b.7_21, b
	unfmv.s	fa1,fa5	#, b.7_21
	unflw	fa0,-100(s0),-100(s0)	#, x
	call	unf_covar		#
	unfmv.s	fa5,fa0	# _22,
# newton_rapson.c:40: 	printf_(
	.loc 1 40 2
	fcvt.d.s	fs1,fa5	# _23, _22
# newton_rapson.c:43: 		unf_var(x), unf_covar(x, b), unf_covar(x, c),
	.loc 1 43 32
	lui	a5,%hi(c)	# tmp167,
	unflw	fa5,%lo(c)(a5),%lo(c)(a5)	# c.8_24, c
	unfmv.s	fa1,fa5	#, c.8_24
	unflw	fa0,-100(s0),-100(s0)	#, x
	call	unf_covar		#
	unfmv.s	fa5,fa0	# _25,
# newton_rapson.c:40: 	printf_(
	.loc 1 40 2
	fcvt.d.s	fs2,fa5	# _26, _25
# newton_rapson.c:44: 		percentage_error(unf_var(x), 0.09673 * unf_var(b) - 0.11755 * unf_covar(b, c) + 0.03571 * unf_var(c)),
	.loc 1 44 3
	unflw	fa0,-100(s0),-100(s0)	#, x
	call	unf_var		#
	unfmv.s	fs4,fa0	# _27,
# newton_rapson.c:44: 		percentage_error(unf_var(x), 0.09673 * unf_var(b) - 0.11755 * unf_covar(b, c) + 0.03571 * unf_var(c)),
	.loc 1 44 42
	lui	a5,%hi(b)	# tmp168,
	unflw	fa5,%lo(b)(a5),%lo(b)(a5)	# b.9_28, b
	unfmv.s	fa0,fa5	#, b.9_28
	call	unf_var		#
	unfmv.s	fa5,fa0	# _29,
	fcvt.d.s	fa4,fa5	# _30, _29
# newton_rapson.c:44: 		percentage_error(unf_var(x), 0.09673 * unf_var(b) - 0.11755 * unf_covar(b, c) + 0.03571 * unf_var(c)),
	.loc 1 44 40
	lui	a5,%hi(.LC2)	# tmp170,
	fld	fa5,%lo(.LC2)(a5)	# tmp169,
	fmul.d	fs3,fa4,fa5	# _31, _30, tmp169
# newton_rapson.c:44: 		percentage_error(unf_var(x), 0.09673 * unf_var(b) - 0.11755 * unf_covar(b, c) + 0.03571 * unf_var(c)),
	.loc 1 44 65
	lui	a5,%hi(b)	# tmp171,
	unflw	fa5,%lo(b)(a5),%lo(b)(a5)	# b.10_32, b
	lui	a5,%hi(c)	# tmp172,
	unflw	fa4,%lo(c)(a5),%lo(c)(a5)	# c.11_33, c
	unfmv.s	fa1,fa4	#, c.11_33
	unfmv.s	fa0,fa5	#, b.10_32
	call	unf_covar		#
	unfmv.s	fa5,fa0	# _34,
	fcvt.d.s	fa4,fa5	# _35, _34
# newton_rapson.c:44: 		percentage_error(unf_var(x), 0.09673 * unf_var(b) - 0.11755 * unf_covar(b, c) + 0.03571 * unf_var(c)),
	.loc 1 44 63
	lui	a5,%hi(.LC3)	# tmp174,
	fld	fa5,%lo(.LC3)(a5)	# tmp173,
	fmul.d	fa5,fa4,fa5	# _36, _35, tmp173
# newton_rapson.c:44: 		percentage_error(unf_var(x), 0.09673 * unf_var(b) - 0.11755 * unf_covar(b, c) + 0.03571 * unf_var(c)),
	.loc 1 44 53
	fsub.d	fs3,fs3,fa5	# _37, _31, _36
# newton_rapson.c:44: 		percentage_error(unf_var(x), 0.09673 * unf_var(b) - 0.11755 * unf_covar(b, c) + 0.03571 * unf_var(c)),
	.loc 1 44 93
	lui	a5,%hi(c)	# tmp175,
	unflw	fa5,%lo(c)(a5),%lo(c)(a5)	# c.12_38, c
	unfmv.s	fa0,fa5	#, c.12_38
	call	unf_var		#
	unfmv.s	fa5,fa0	# _39,
	fcvt.d.s	fa4,fa5	# _40, _39
# newton_rapson.c:44: 		percentage_error(unf_var(x), 0.09673 * unf_var(b) - 0.11755 * unf_covar(b, c) + 0.03571 * unf_var(c)),
	.loc 1 44 91
	lui	a5,%hi(.LC4)	# tmp177,
	fld	fa5,%lo(.LC4)(a5)	# tmp176,
	fmul.d	fa5,fa4,fa5	# _41, _40, tmp176
# newton_rapson.c:44: 		percentage_error(unf_var(x), 0.09673 * unf_var(b) - 0.11755 * unf_covar(b, c) + 0.03571 * unf_var(c)),
	.loc 1 44 81
	fadd.d	fa5,fs3,fa5	# _42, _37, _41
# newton_rapson.c:44: 		percentage_error(unf_var(x), 0.09673 * unf_var(b) - 0.11755 * unf_covar(b, c) + 0.03571 * unf_var(c)),
	.loc 1 44 3
	fcvt.s.d	fa5,fa5	# _43, _42
	unfmv.s	fa1,fa5	#, _43
	unfmv.s	fa0,fs4	#, _27
	call	percentage_error		#
	unfmv.s	fa5,fa0	# _44,
# newton_rapson.c:40: 	printf_(
	.loc 1 40 2
	fcvt.d.s	fs3,fa5	# _45, _44
# newton_rapson.c:45: 		percentage_error(unf_covar(x, b), -0.31102 * unf_var(b) + 0.18898 * unf_covar(b, c)),
	.loc 1 45 3
	lui	a5,%hi(b)	# tmp178,
	unflw	fa5,%lo(b)(a5),%lo(b)(a5)	# b.13_46, b
	unfmv.s	fa1,fa5	#, b.13_46
	unflw	fa0,-100(s0),-100(s0)	#, x
	call	unf_covar		#
	unfmv.s	fs5,fa0	# _47,
# newton_rapson.c:45: 		percentage_error(unf_covar(x, b), -0.31102 * unf_var(b) + 0.18898 * unf_covar(b, c)),
	.loc 1 45 48
	lui	a5,%hi(b)	# tmp179,
	unflw	fa5,%lo(b)(a5),%lo(b)(a5)	# b.14_48, b
	unfmv.s	fa0,fa5	#, b.14_48
	call	unf_var		#
	unfmv.s	fa5,fa0	# _49,
	fcvt.d.s	fa4,fa5	# _50, _49
# newton_rapson.c:45: 		percentage_error(unf_covar(x, b), -0.31102 * unf_var(b) + 0.18898 * unf_covar(b, c)),
	.loc 1 45 46
	lui	a5,%hi(.LC5)	# tmp181,
	fld	fa5,%lo(.LC5)(a5)	# tmp180,
	fmul.d	fs4,fa4,fa5	# _51, _50, tmp180
# newton_rapson.c:45: 		percentage_error(unf_covar(x, b), -0.31102 * unf_var(b) + 0.18898 * unf_covar(b, c)),
	.loc 1 45 71
	lui	a5,%hi(b)	# tmp182,
	unflw	fa5,%lo(b)(a5),%lo(b)(a5)	# b.15_52, b
	lui	a5,%hi(c)	# tmp183,
	unflw	fa4,%lo(c)(a5),%lo(c)(a5)	# c.16_53, c
	unfmv.s	fa1,fa4	#, c.16_53
	unfmv.s	fa0,fa5	#, b.15_52
	call	unf_covar		#
	unfmv.s	fa5,fa0	# _54,
	fcvt.d.s	fa4,fa5	# _55, _54
# newton_rapson.c:45: 		percentage_error(unf_covar(x, b), -0.31102 * unf_var(b) + 0.18898 * unf_covar(b, c)),
	.loc 1 45 69
	lui	a5,%hi(.LC6)	# tmp185,
	fld	fa5,%lo(.LC6)(a5)	# tmp184,
	fmul.d	fa5,fa4,fa5	# _56, _55, tmp184
# newton_rapson.c:45: 		percentage_error(unf_covar(x, b), -0.31102 * unf_var(b) + 0.18898 * unf_covar(b, c)),
	.loc 1 45 59
	fadd.d	fa5,fs4,fa5	# _57, _51, _56
# newton_rapson.c:45: 		percentage_error(unf_covar(x, b), -0.31102 * unf_var(b) + 0.18898 * unf_covar(b, c)),
	.loc 1 45 3
	fcvt.s.d	fa5,fa5	# _58, _57
	unfmv.s	fa1,fa5	#, _58
	unfmv.s	fa0,fs5	#, _47
	call	percentage_error		#
	unfmv.s	fa5,fa0	# _59,
# newton_rapson.c:40: 	printf_(
	.loc 1 40 2
	fcvt.d.s	fs4,fa5	# _60, _59
# newton_rapson.c:46: 		percentage_error(unf_covar(x, c), -0.31102 * unf_covar(b, c) + 0.18898 *unf_var(c))
	.loc 1 46 3
	lui	a5,%hi(c)	# tmp186,
	unflw	fa5,%lo(c)(a5),%lo(c)(a5)	# c.17_61, c
	unfmv.s	fa1,fa5	#, c.17_61
	unflw	fa0,-100(s0),-100(s0)	#, x
	call	unf_covar		#
	unfmv.s	fs9,fa0	# _62,
# newton_rapson.c:46: 		percentage_error(unf_covar(x, c), -0.31102 * unf_covar(b, c) + 0.18898 *unf_var(c))
	.loc 1 46 48
	lui	a5,%hi(b)	# tmp187,
	unflw	fa5,%lo(b)(a5),%lo(b)(a5)	# b.18_63, b
	lui	a5,%hi(c)	# tmp188,
	unflw	fa4,%lo(c)(a5),%lo(c)(a5)	# c.19_64, c
	unfmv.s	fa1,fa4	#, c.19_64
	unfmv.s	fa0,fa5	#, b.18_63
	call	unf_covar		#
	unfmv.s	fa5,fa0	# _65,
	fcvt.d.s	fa4,fa5	# _66, _65
# newton_rapson.c:46: 		percentage_error(unf_covar(x, c), -0.31102 * unf_covar(b, c) + 0.18898 *unf_var(c))
	.loc 1 46 46
	lui	a5,%hi(.LC5)	# tmp190,
	fld	fa5,%lo(.LC5)(a5)	# tmp189,
	fmul.d	fs5,fa4,fa5	# _67, _66, tmp189
# newton_rapson.c:46: 		percentage_error(unf_covar(x, c), -0.31102 * unf_covar(b, c) + 0.18898 *unf_var(c))
	.loc 1 46 75
	lui	a5,%hi(c)	# tmp191,
	unflw	fa5,%lo(c)(a5),%lo(c)(a5)	# c.20_68, c
	unfmv.s	fa0,fa5	#, c.20_68
	call	unf_var		#
	unfmv.s	fa5,fa0	# _69,
	fcvt.d.s	fa4,fa5	# _70, _69
# newton_rapson.c:46: 		percentage_error(unf_covar(x, c), -0.31102 * unf_covar(b, c) + 0.18898 *unf_var(c))
	.loc 1 46 74
	lui	a5,%hi(.LC6)	# tmp193,
	fld	fa5,%lo(.LC6)(a5)	# tmp192,
	fmul.d	fa5,fa4,fa5	# _71, _70, tmp192
# newton_rapson.c:46: 		percentage_error(unf_covar(x, c), -0.31102 * unf_covar(b, c) + 0.18898 *unf_var(c))
	.loc 1 46 64
	fadd.d	fa5,fs5,fa5	# _72, _67, _71
# newton_rapson.c:46: 		percentage_error(unf_covar(x, c), -0.31102 * unf_covar(b, c) + 0.18898 *unf_var(c))
	.loc 1 46 3
	fcvt.s.d	fa5,fa5	# _73, _72
	unfmv.s	fa1,fa5	#, _73
	unfmv.s	fa0,fs9	#, _62
	call	percentage_error		#
	unfmv.s	fa5,fa0	# _74,
# newton_rapson.c:40: 	printf_(
	.loc 1 40 2
	fcvt.d.s	fa5,fa5	# _75, _74
	fsd	fa5,40(sp)	# _75,
	fsd	fs4,32(sp)	# _60,
	fsd	fs3,24(sp)	# _45,
	fsd	fs2,16(sp)	# _26,
	fsd	fs1,8(sp)	# _23,
	fsd	fs0,0(sp)	# _20,
	fsd	fs8,-120(s0)	# _18, %sfp
	lw	a6,-120(s0)		#, %sfp
	lw	a7,-116(s0)		#, %sfp
	fsd	fs7,-120(s0)	# _14, %sfp
	lw	a4,-120(s0)		#, %sfp
	lw	a5,-116(s0)		#, %sfp
	fsd	fs6,-120(s0)	# _11, %sfp
	lw	a2,-120(s0)		#, %sfp
	lw	a3,-116(s0)		#, %sfp
	lui	a1,%hi(.LC7)	# tmp194,
	addi	a0,a1,%lo(.LC7)	#, tmp194,
	call	printf_		#
# newton_rapson.c:48: }
	.loc 1 48 1
	nop	
	lw	ra,172(sp)		#,
.LCFI19:
	lw	s0,168(sp)		#,
.LCFI20:
	fld	fs0,152(sp)	#,
.LCFI21:
	fld	fs1,144(sp)	#,
.LCFI22:
	fld	fs2,136(sp)	#,
.LCFI23:
	fld	fs3,128(sp)	#,
.LCFI24:
	fld	fs4,120(sp)	#,
.LCFI25:
	fld	fs5,112(sp)	#,
.LCFI26:
	fld	fs6,104(sp)	#,
.LCFI27:
	fld	fs7,96(sp)	#,
.LCFI28:
	fld	fs8,88(sp)	#,
.LCFI29:
	fld	fs9,80(sp)	#,
.LCFI30:
	addi	sp,sp,176	#,,
.LCFI31:
	jr	ra		#
.LFE6:
	.size	newton_rapson, .-newton_rapson
	.section	.rodata
	.align	2
.LC8:
	.string	"\n"
	.align	2
.LC9:
	.string	"variance[b]\tvariance[c]\tcovariance[bc]\tvariance[x]\tcovariance[xb]\tcovariance[xc]\terr[x]\terr[xb]\terr[xc]\n"
	.text
	.align	2
	.globl	main
	.type	main, @function
main:
.LFB7:
	.loc 1 51 1
	addi	sp,sp,-32	#,,
.LCFI32:
	sw	ra,28(sp)	#,
	sw	s0,24(sp)	#,
.LCFI33:
	addi	s0,sp,32	#,,
.LCFI34:
# newton_rapson.c:53: 	printf_("\n");
	.loc 1 53 2
	lui	a5,%hi(.LC8)	# tmp93,
	addi	a0,a5,%lo(.LC8)	#, tmp93,
	call	printf_		#
# newton_rapson.c:54: 	printf_("variance[b]\tvariance[c]\tcovariance[bc]\tvariance[x]\tcovariance[xb]\tcovariance[xc]\terr[x]\terr[xb]\terr[xc]\n");
	.loc 1 54 2
	lui	a5,%hi(.LC9)	# tmp94,
	addi	a0,a5,%lo(.LC9)	#, tmp94,
	call	printf_		#
# newton_rapson.c:55: 	for (var_b = 0; var_b < 0.5; var_b += 0.1) {
	.loc 1 55 13
	sw	zero,-20(s0)	#, var_b
# newton_rapson.c:55: 	for (var_b = 0; var_b < 0.5; var_b += 0.1) {
	.loc 1 55 2
	j	.L12		#
.L18:
# newton_rapson.c:56: 		b = unf_create(2, var_b);
	.loc 1 56 7
	lui	a5,%hi(.LC10)	# tmp96,
	unflw	fa5,%lo(.LC10)(a5),%lo(.LC10)(a5)	# tmp95,
	unflw	fa1,-20(s0),-20(s0)	#, var_b
	unfmv.s	fa0,fa5	#, tmp95
	call	unf_create		#
	unfmv.s	fa5,fa0	# _1,
# newton_rapson.c:56: 		b = unf_create(2, var_b);
	.loc 1 56 5
	lui	a5,%hi(b)	# tmp97,
	unfsw	fa5,%lo(b)(a5),%lo(b)(a5)	# _1, b
# newton_rapson.c:57: 		for (var_c = 0; var_c < 0.5; var_c += 0.1) {
	.loc 1 57 14
	sw	zero,-24(s0)	#, var_c
# newton_rapson.c:57: 		for (var_c = 0; var_c < 0.5; var_c += 0.1) {
	.loc 1 57 3
	j	.L13		#
.L17:
# newton_rapson.c:58: 			if (var_b != 0) {
	.loc 1 58 7
	unflw	fa5,-20(s0),-20(s0)	# tmp98, var_b
	fmv.s.x	fa4,zero	# tmp99,
	feq.s	a5,fa5,fa4	#, tmp100, tmp98, tmp99
	bnez	a5,.L14	#, tmp100,
# newton_rapson.c:59: 				stddev_ratio = unf_create_exact(sqrtf(var_c / var_b));
	.loc 1 59 20
	unflw	fa4,-24(s0),-24(s0)	# tmp101, var_c
	unflw	fa5,-20(s0),-20(s0)	# tmp102, var_b
	unfdiv.s	fa5,fa4,fa5	# _2, tmp101, tmp102
	unfmv.s	fa0,fa5	#, _2
	call	sqrtf		#
	unfmv.s	fa5,fa0	# _3,
	unfmv.s	fa0,fa5	#, _3
	call	unf_create_exact		#
	unfsw	fa0,-28(s0),-28(s0)	#, stddev_ratio
# newton_rapson.c:62: 				c = 6 + stddev_ratio * (2 - b);
	.loc 1 62 31
	lui	a5,%hi(b)	# tmp103,
	unflw	fa5,%lo(b)(a5),%lo(b)(a5)	# b.21_4, b
	lui	a5,%hi(.LC10)	# tmp105,
	unflw	fa4,%lo(.LC10)(a5),%lo(.LC10)(a5)	# tmp104,
	unfsub.s	fa4,fa4,fa5	# _5, tmp104, b.21_4
# newton_rapson.c:62: 				c = 6 + stddev_ratio * (2 - b);
	.loc 1 62 26
	unflw	fa5,-28(s0),-28(s0)	# tmp106, stddev_ratio
	unfmul.s	fa4,fa4,fa5	# _6, _5, tmp106
# newton_rapson.c:62: 				c = 6 + stddev_ratio * (2 - b);
	.loc 1 62 11
	lui	a5,%hi(.LC11)	# tmp108,
	unflw	fa5,%lo(.LC11)(a5),%lo(.LC11)(a5)	# tmp107,
	unfadd.s	fa5,fa4,fa5	# _7, _6, tmp107
# newton_rapson.c:62: 				c = 6 + stddev_ratio * (2 - b);
	.loc 1 62 7
	lui	a5,%hi(c)	# tmp109,
	unfsw	fa5,%lo(c)(a5),%lo(c)(a5)	# _7, c
# newton_rapson.c:63: 				newton_rapson();
	.loc 1 63 5
	call	newton_rapson		#
# newton_rapson.c:66: 				c = 6 + stddev_ratio * (b - 2);
	.loc 1 66 31
	lui	a5,%hi(b)	# tmp110,
	unflw	fa4,%lo(b)(a5),%lo(b)(a5)	# b.22_8, b
	lui	a5,%hi(.LC10)	# tmp112,
	unflw	fa5,%lo(.LC10)(a5),%lo(.LC10)(a5)	# tmp111,
	unfsub.s	fa4,fa4,fa5	# _9, b.22_8, tmp111
# newton_rapson.c:66: 				c = 6 + stddev_ratio * (b - 2);
	.loc 1 66 26
	unflw	fa5,-28(s0),-28(s0)	# tmp113, stddev_ratio
	unfmul.s	fa4,fa4,fa5	# _10, _9, tmp113
# newton_rapson.c:66: 				c = 6 + stddev_ratio * (b - 2);
	.loc 1 66 11
	lui	a5,%hi(.LC11)	# tmp115,
	unflw	fa5,%lo(.LC11)(a5),%lo(.LC11)(a5)	# tmp114,
	unfadd.s	fa5,fa4,fa5	# _11, _10, tmp114
# newton_rapson.c:66: 				c = 6 + stddev_ratio * (b - 2);
	.loc 1 66 7
	lui	a5,%hi(c)	# tmp116,
	unfsw	fa5,%lo(c)(a5),%lo(c)(a5)	# _11, c
# newton_rapson.c:67: 				newton_rapson();
	.loc 1 67 5
	call	newton_rapson		#
.L14:
# newton_rapson.c:69: 			if (var_c > var_b) {
	.loc 1 69 7
	unflw	fa4,-24(s0),-24(s0)	# tmp117, var_c
	unflw	fa5,-20(s0),-20(s0)	# tmp118, var_b
	fgt.s	a5,fa4,fa5	#, tmp119, tmp117, tmp118
	beqz	a5,.L15	#, tmp119,
# newton_rapson.c:70: 				four = unf_create(4, var_c - var_b);
	.loc 1 70 12
	unflw	fa4,-24(s0),-24(s0)	# tmp120, var_c
	unflw	fa5,-20(s0),-20(s0)	# tmp121, var_b
	unfsub.s	fa4,fa4,fa5	# _12, tmp120, tmp121
	lui	a5,%hi(.LC12)	# tmp123,
	unflw	fa5,%lo(.LC12)(a5),%lo(.LC12)(a5)	# tmp122,
	unfmv.s	fa1,fa4	#, _12
	unfmv.s	fa0,fa5	#, tmp122
	call	unf_create		#
	unfsw	fa0,-32(s0),-32(s0)	#, four
# newton_rapson.c:73: 				c = four + b;
	.loc 1 73 14
	lui	a5,%hi(b)	# tmp124,
	unflw	fa4,%lo(b)(a5),%lo(b)(a5)	# b.23_13, b
	unflw	fa5,-32(s0),-32(s0)	# tmp125, four
	unfadd.s	fa5,fa4,fa5	# _14, b.23_13, tmp125
# newton_rapson.c:73: 				c = four + b;
	.loc 1 73 7
	lui	a5,%hi(c)	# tmp126,
	unfsw	fa5,%lo(c)(a5),%lo(c)(a5)	# _14, c
# newton_rapson.c:74: 				newton_rapson();
	.loc 1 74 5
	call	newton_rapson		#
.L15:
# newton_rapson.c:77: 			c = unf_create(6, var_c);
	.loc 1 77 8 discriminator 2
	lui	a5,%hi(.LC11)	# tmp128,
	unflw	fa5,%lo(.LC11)(a5),%lo(.LC11)(a5)	# tmp127,
	unflw	fa1,-24(s0),-24(s0)	#, var_c
	unfmv.s	fa0,fa5	#, tmp127
	call	unf_create		#
	unfmv.s	fa5,fa0	# _15,
# newton_rapson.c:77: 			c = unf_create(6, var_c);
	.loc 1 77 6 discriminator 2
	lui	a5,%hi(c)	# tmp129,
	unfsw	fa5,%lo(c)(a5),%lo(c)(a5)	# _15, c
# newton_rapson.c:78: 			newton_rapson();
	.loc 1 78 4 discriminator 2
	call	newton_rapson		#
# newton_rapson.c:57: 		for (var_c = 0; var_c < 0.5; var_c += 0.1) {
	.loc 1 57 38 discriminator 2
	unflw	fa5,-24(s0),-24(s0)	# tmp130, var_c
	fcvt.d.s	fa4,fa5	# _16, tmp130
	lui	a5,%hi(.LC13)	# tmp132,
	fld	fa5,%lo(.LC13)(a5)	# tmp131,
	fadd.d	fa5,fa4,fa5	# _17, _16, tmp131
	fcvt.s.d	fa5,fa5	# tmp133, _17
	unfsw	fa5,-24(s0),-24(s0)	# tmp133, var_c
.L13:
# newton_rapson.c:57: 		for (var_c = 0; var_c < 0.5; var_c += 0.1) {
	.loc 1 57 3 discriminator 1
	unflw	fa4,-24(s0),-24(s0)	# tmp134, var_c
	lui	a5,%hi(.LC14)	# tmp136,
	unflw	fa5,%lo(.LC14)(a5),%lo(.LC14)(a5)	# tmp135,
	flt.s	a5,fa4,fa5	#, tmp137, tmp134, tmp135
	bnez	a5,.L17	#, tmp137,
# newton_rapson.c:55: 	for (var_b = 0; var_b < 0.5; var_b += 0.1) {
	.loc 1 55 37 discriminator 2
	unflw	fa5,-20(s0),-20(s0)	# tmp138, var_b
	fcvt.d.s	fa4,fa5	# _18, tmp138
	lui	a5,%hi(.LC13)	# tmp140,
	fld	fa5,%lo(.LC13)(a5)	# tmp139,
	fadd.d	fa5,fa4,fa5	# _19, _18, tmp139
	fcvt.s.d	fa5,fa5	# tmp141, _19
	unfsw	fa5,-20(s0),-20(s0)	# tmp141, var_b
.L12:
# newton_rapson.c:55: 	for (var_b = 0; var_b < 0.5; var_b += 0.1) {
	.loc 1 55 2 discriminator 1
	unflw	fa4,-20(s0),-20(s0)	# tmp142, var_b
	lui	a5,%hi(.LC14)	# tmp144,
	unflw	fa5,%lo(.LC14)(a5),%lo(.LC14)(a5)	# tmp143,
	flt.s	a5,fa4,fa5	#, tmp145, tmp142, tmp143
	bnez	a5,.L18	#, tmp145,
# newton_rapson.c:82: 	return 0;
	.loc 1 82 9
	li	a5,0		# _49,
# newton_rapson.c:83: }
	.loc 1 83 1
	mv	a0,a5	#, <retval>
	lw	ra,28(sp)		#,
.LCFI35:
	lw	s0,24(sp)		#,
.LCFI36:
	addi	sp,sp,32	#,,
.LCFI37:
	jr	ra		#
.LFE7:
	.size	main, .-main
	.section	.rodata
	.align	2
.LC0:
	.word	1120403456
	.align	3
.LC1:
	.word	2296604913
	.word	1055193269
	.align	3
.LC2:
	.word	445302209
	.word	1069073228
	.align	3
.LC3:
	.word	3181711773
	.word	1069422529
	.align	3
.LC4:
	.word	3293037325
	.word	1067600020
	.align	3
.LC5:
	.word	1847179535
	.word	-1076631616
	.align	3
.LC6:
	.word	600608227
	.word	1070084223
	.align	2
.LC10:
	.word	1073741824
	.align	2
.LC11:
	.word	1086324736
	.align	2
.LC12:
	.word	1082130432
	.align	3
.LC13:
	.word	2576980378
	.word	1069128089
	.align	2
.LC14:
	.word	1056964608
	.section	.debug_frame,"",@progbits
.Lframe0:
	.4byte	.LECIE0-.LSCIE0
.LSCIE0:
	.4byte	0xffffffff
	.byte	0x3
	.string	""
	.byte	0x1
	.byte	0x7c
	.byte	0x1
	.byte	0xc
	.byte	0x2
	.byte	0
	.align	2
.LECIE0:
.LSFDE0:
	.4byte	.LEFDE0-.LASFDE0
.LASFDE0:
	.4byte	.Lframe0
	.4byte	.LFB3
	.4byte	.LFE3-.LFB3
	.byte	0x4
	.4byte	.LCFI0-.LFB3
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.4byte	.LCFI1-.LCFI0
	.byte	0x88
	.byte	0x1
	.byte	0x4
	.4byte	.LCFI2-.LCFI1
	.byte	0xc
	.byte	0x8
	.byte	0
	.byte	0x4
	.4byte	.LCFI3-.LCFI2
	.byte	0xc8
	.byte	0x4
	.4byte	.LCFI4-.LCFI3
	.byte	0xd
	.byte	0x2
	.align	2
.LEFDE0:
.LSFDE2:
	.4byte	.LEFDE2-.LASFDE2
.LASFDE2:
	.4byte	.Lframe0
	.4byte	.LFB4
	.4byte	.LFE4-.LFB4
	.byte	0x4
	.4byte	.LCFI5-.LFB4
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.4byte	.LCFI6-.LCFI5
	.byte	0x88
	.byte	0x1
	.byte	0x4
	.4byte	.LCFI7-.LCFI6
	.byte	0xc
	.byte	0x8
	.byte	0
	.byte	0x4
	.4byte	.LCFI8-.LCFI7
	.byte	0xc8
	.byte	0x4
	.4byte	.LCFI9-.LCFI8
	.byte	0xd
	.byte	0x2
	.align	2
.LEFDE2:
.LSFDE4:
	.4byte	.LEFDE4-.LASFDE4
.LASFDE4:
	.4byte	.Lframe0
	.4byte	.LFB5
	.4byte	.LFE5-.LFB5
	.byte	0x4
	.4byte	.LCFI10-.LFB5
	.byte	0xe
	.byte	0x30
	.byte	0x4
	.4byte	.LCFI11-.LCFI10
	.byte	0x81
	.byte	0x1
	.byte	0x88
	.byte	0x2
	.byte	0x4
	.4byte	.LCFI12-.LCFI11
	.byte	0xc
	.byte	0x8
	.byte	0
	.byte	0x4
	.4byte	.LCFI13-.LCFI12
	.byte	0xc1
	.byte	0x4
	.4byte	.LCFI14-.LCFI13
	.byte	0xc8
	.byte	0x4
	.4byte	.LCFI15-.LCFI14
	.byte	0xd
	.byte	0x2
	.align	2
.LEFDE4:
.LSFDE6:
	.4byte	.LEFDE6-.LASFDE6
.LASFDE6:
	.4byte	.Lframe0
	.4byte	.LFB6
	.4byte	.LFE6-.LFB6
	.byte	0x4
	.4byte	.LCFI16-.LFB6
	.byte	0xe
	.byte	0xb0,0x1
	.byte	0x4
	.4byte	.LCFI17-.LCFI16
	.byte	0x81
	.byte	0x1
	.byte	0x88
	.byte	0x2
	.byte	0xa8
	.byte	0x6
	.byte	0xa9
	.byte	0x8
	.byte	0xb2
	.byte	0xa
	.byte	0xb3
	.byte	0xc
	.byte	0xb4
	.byte	0xe
	.byte	0xb5
	.byte	0x10
	.byte	0xb6
	.byte	0x12
	.byte	0xb7
	.byte	0x14
	.byte	0xb8
	.byte	0x16
	.byte	0xb9
	.byte	0x18
	.byte	0x4
	.4byte	.LCFI18-.LCFI17
	.byte	0xc
	.byte	0x8
	.byte	0
	.byte	0x4
	.4byte	.LCFI19-.LCFI18
	.byte	0xc1
	.byte	0x4
	.4byte	.LCFI20-.LCFI19
	.byte	0xc8
	.byte	0x4
	.4byte	.LCFI21-.LCFI20
	.byte	0xe8
	.byte	0x4
	.4byte	.LCFI22-.LCFI21
	.byte	0xe9
	.byte	0x4
	.4byte	.LCFI23-.LCFI22
	.byte	0xf2
	.byte	0x4
	.4byte	.LCFI24-.LCFI23
	.byte	0xf3
	.byte	0x4
	.4byte	.LCFI25-.LCFI24
	.byte	0xf4
	.byte	0x4
	.4byte	.LCFI26-.LCFI25
	.byte	0xf5
	.byte	0x4
	.4byte	.LCFI27-.LCFI26
	.byte	0xf6
	.byte	0x4
	.4byte	.LCFI28-.LCFI27
	.byte	0xf7
	.byte	0x4
	.4byte	.LCFI29-.LCFI28
	.byte	0xf8
	.byte	0x4
	.4byte	.LCFI30-.LCFI29
	.byte	0xf9
	.byte	0x4
	.4byte	.LCFI31-.LCFI30
	.byte	0xd
	.byte	0x2
	.align	2
.LEFDE6:
.LSFDE8:
	.4byte	.LEFDE8-.LASFDE8
.LASFDE8:
	.4byte	.Lframe0
	.4byte	.LFB7
	.4byte	.LFE7-.LFB7
	.byte	0x4
	.4byte	.LCFI32-.LFB7
	.byte	0xe
	.byte	0x20
	.byte	0x4
	.4byte	.LCFI33-.LCFI32
	.byte	0x81
	.byte	0x1
	.byte	0x88
	.byte	0x2
	.byte	0x4
	.4byte	.LCFI34-.LCFI33
	.byte	0xc
	.byte	0x8
	.byte	0
	.byte	0x4
	.4byte	.LCFI35-.LCFI34
	.byte	0xc1
	.byte	0x4
	.4byte	.LCFI36-.LCFI35
	.byte	0xc8
	.byte	0x4
	.4byte	.LCFI37-.LCFI36
	.byte	0xd
	.byte	0x2
	.align	2
.LEFDE8:
	.text
.Letext0:
	.file 2 "/home/samuelmswong/harrysun/sunflower-toolchain/tools/riscv/riscv32-elf/sys-include/sys/lock.h"
	.file 3 "/home/samuelmswong/harrysun/sunflower-toolchain/tools/riscv/riscv32-elf/sys-include/sys/_types.h"
	.file 4 "/home/samuelmswong/harrysun/sunflower-toolchain/tools/riscv/lib/gcc/riscv32-elf/8.2.0/include/stddef.h"
	.file 5 "/home/samuelmswong/harrysun/sunflower-toolchain/tools/riscv/riscv32-elf/sys-include/sys/reent.h"
	.file 6 "/home/samuelmswong/harrysun/sunflower-toolchain/tools/riscv/riscv32-elf/sys-include/stdlib.h"
	.file 7 "/home/samuelmswong/harrysun/sunflower-toolchain/tools/riscv/riscv32-elf/sys-include/math.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.4byte	0xabd
	.2byte	0x4
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.byte	0x1
	.4byte	.LASF136
	.byte	0xc
	.4byte	.LASF137
	.4byte	.LASF138
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	.Ldebug_line0
	.byte	0x2
	.byte	0x1
	.byte	0x6
	.4byte	.LASF0
	.byte	0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF1
	.byte	0x2
	.byte	0x2
	.byte	0x5
	.4byte	.LASF2
	.byte	0x2
	.byte	0x2
	.byte	0x7
	.4byte	.LASF3
	.byte	0x2
	.byte	0x4
	.byte	0x5
	.4byte	.LASF4
	.byte	0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF5
	.byte	0x2
	.byte	0x8
	.byte	0x5
	.4byte	.LASF6
	.byte	0x2
	.byte	0x8
	.byte	0x7
	.4byte	.LASF7
	.byte	0x3
	.byte	0x4
	.byte	0x5
	.string	"int"
	.byte	0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF8
	.byte	0x2
	.byte	0x10
	.byte	0x4
	.4byte	.LASF9
	.byte	0x4
	.4byte	.LASF10
	.byte	0x2
	.byte	0xc
	.byte	0xd
	.4byte	0x5d
	.byte	0x4
	.4byte	.LASF11
	.byte	0x3
	.byte	0x2c
	.byte	0xe
	.4byte	0x41
	.byte	0x4
	.4byte	.LASF12
	.byte	0x3
	.byte	0x72
	.byte	0xe
	.4byte	0x41
	.byte	0x4
	.4byte	.LASF13
	.byte	0x3
	.byte	0x91
	.byte	0x17
	.4byte	0x5d
	.byte	0x5
	.4byte	.LASF14
	.byte	0x4
	.2byte	0x165
	.byte	0x17
	.4byte	0x64
	.byte	0x6
	.byte	0x4
	.byte	0x3
	.byte	0xa6
	.byte	0x3
	.4byte	0xd1
	.byte	0x7
	.4byte	.LASF15
	.byte	0x3
	.byte	0xa8
	.byte	0xc
	.4byte	0xa2
	.byte	0x7
	.4byte	.LASF16
	.byte	0x3
	.byte	0xa9
	.byte	0x13
	.4byte	0xd1
	.byte	0
	.byte	0x8
	.4byte	0x2c
	.4byte	0xe1
	.byte	0x9
	.4byte	0x64
	.byte	0x3
	.byte	0
	.byte	0xa
	.byte	0x8
	.byte	0x3
	.byte	0xa3
	.byte	0x9
	.4byte	0x105
	.byte	0xb
	.4byte	.LASF17
	.byte	0x3
	.byte	0xa5
	.byte	0x7
	.4byte	0x5d
	.byte	0
	.byte	0xb
	.4byte	.LASF18
	.byte	0x3
	.byte	0xaa
	.byte	0x5
	.4byte	0xaf
	.byte	0x4
	.byte	0
	.byte	0x4
	.4byte	.LASF19
	.byte	0x3
	.byte	0xab
	.byte	0x3
	.4byte	0xe1
	.byte	0x4
	.4byte	.LASF20
	.byte	0x3
	.byte	0xaf
	.byte	0x1b
	.4byte	0x72
	.byte	0xc
	.byte	0x4
	.byte	0x4
	.4byte	.LASF21
	.byte	0x5
	.byte	0x16
	.byte	0x19
	.4byte	0x48
	.byte	0xd
	.4byte	.LASF26
	.byte	0x18
	.byte	0x5
	.byte	0x2f
	.byte	0x8
	.4byte	0x185
	.byte	0xb
	.4byte	.LASF22
	.byte	0x5
	.byte	0x31
	.byte	0x13
	.4byte	0x185
	.byte	0
	.byte	0xe
	.string	"_k"
	.byte	0x5
	.byte	0x32
	.byte	0x7
	.4byte	0x5d
	.byte	0x4
	.byte	0xb
	.4byte	.LASF23
	.byte	0x5
	.byte	0x32
	.byte	0xb
	.4byte	0x5d
	.byte	0x8
	.byte	0xb
	.4byte	.LASF24
	.byte	0x5
	.byte	0x32
	.byte	0x14
	.4byte	0x5d
	.byte	0xc
	.byte	0xb
	.4byte	.LASF25
	.byte	0x5
	.byte	0x32
	.byte	0x1b
	.4byte	0x5d
	.byte	0x10
	.byte	0xe
	.string	"_x"
	.byte	0x5
	.byte	0x33
	.byte	0xb
	.4byte	0x18b
	.byte	0x14
	.byte	0
	.byte	0xf
	.byte	0x4
	.4byte	0x12b
	.byte	0x8
	.4byte	0x11f
	.4byte	0x19b
	.byte	0x9
	.4byte	0x64
	.byte	0
	.byte	0
	.byte	0xd
	.4byte	.LASF27
	.byte	0x24
	.byte	0x5
	.byte	0x37
	.byte	0x8
	.4byte	0x21e
	.byte	0xb
	.4byte	.LASF28
	.byte	0x5
	.byte	0x39
	.byte	0x9
	.4byte	0x5d
	.byte	0
	.byte	0xb
	.4byte	.LASF29
	.byte	0x5
	.byte	0x3a
	.byte	0x9
	.4byte	0x5d
	.byte	0x4
	.byte	0xb
	.4byte	.LASF30
	.byte	0x5
	.byte	0x3b
	.byte	0x9
	.4byte	0x5d
	.byte	0x8
	.byte	0xb
	.4byte	.LASF31
	.byte	0x5
	.byte	0x3c
	.byte	0x9
	.4byte	0x5d
	.byte	0xc
	.byte	0xb
	.4byte	.LASF32
	.byte	0x5
	.byte	0x3d
	.byte	0x9
	.4byte	0x5d
	.byte	0x10
	.byte	0xb
	.4byte	.LASF33
	.byte	0x5
	.byte	0x3e
	.byte	0x9
	.4byte	0x5d
	.byte	0x14
	.byte	0xb
	.4byte	.LASF34
	.byte	0x5
	.byte	0x3f
	.byte	0x9
	.4byte	0x5d
	.byte	0x18
	.byte	0xb
	.4byte	.LASF35
	.byte	0x5
	.byte	0x40
	.byte	0x9
	.4byte	0x5d
	.byte	0x1c
	.byte	0xb
	.4byte	.LASF36
	.byte	0x5
	.byte	0x41
	.byte	0x9
	.4byte	0x5d
	.byte	0x20
	.byte	0
	.byte	0x10
	.4byte	.LASF37
	.2byte	0x108
	.byte	0x5
	.byte	0x4a
	.byte	0x8
	.4byte	0x263
	.byte	0xb
	.4byte	.LASF38
	.byte	0x5
	.byte	0x4b
	.byte	0xa
	.4byte	0x263
	.byte	0
	.byte	0xb
	.4byte	.LASF39
	.byte	0x5
	.byte	0x4c
	.byte	0x9
	.4byte	0x263
	.byte	0x80
	.byte	0x11
	.4byte	.LASF40
	.byte	0x5
	.byte	0x4e
	.byte	0xa
	.4byte	0x11f
	.2byte	0x100
	.byte	0x11
	.4byte	.LASF41
	.byte	0x5
	.byte	0x51
	.byte	0xa
	.4byte	0x11f
	.2byte	0x104
	.byte	0
	.byte	0x8
	.4byte	0x11d
	.4byte	0x273
	.byte	0x9
	.4byte	0x64
	.byte	0x1f
	.byte	0
	.byte	0x10
	.4byte	.LASF42
	.2byte	0x190
	.byte	0x5
	.byte	0x5d
	.byte	0x8
	.4byte	0x2b6
	.byte	0xb
	.4byte	.LASF22
	.byte	0x5
	.byte	0x5e
	.byte	0x12
	.4byte	0x2b6
	.byte	0
	.byte	0xb
	.4byte	.LASF43
	.byte	0x5
	.byte	0x5f
	.byte	0x6
	.4byte	0x5d
	.byte	0x4
	.byte	0xb
	.4byte	.LASF44
	.byte	0x5
	.byte	0x61
	.byte	0x9
	.4byte	0x2bc
	.byte	0x8
	.byte	0xb
	.4byte	.LASF37
	.byte	0x5
	.byte	0x62
	.byte	0x1e
	.4byte	0x21e
	.byte	0x88
	.byte	0
	.byte	0xf
	.byte	0x4
	.4byte	0x273
	.byte	0x8
	.4byte	0x2cc
	.4byte	0x2cc
	.byte	0x9
	.4byte	0x64
	.byte	0x1f
	.byte	0
	.byte	0xf
	.byte	0x4
	.4byte	0x2d2
	.byte	0x12
	.byte	0xd
	.4byte	.LASF45
	.byte	0x8
	.byte	0x5
	.byte	0x75
	.byte	0x8
	.4byte	0x2fb
	.byte	0xb
	.4byte	.LASF46
	.byte	0x5
	.byte	0x76
	.byte	0x11
	.4byte	0x2fb
	.byte	0
	.byte	0xb
	.4byte	.LASF47
	.byte	0x5
	.byte	0x77
	.byte	0x6
	.4byte	0x5d
	.byte	0x4
	.byte	0
	.byte	0xf
	.byte	0x4
	.4byte	0x2c
	.byte	0xd
	.4byte	.LASF48
	.byte	0x68
	.byte	0x5
	.byte	0xb5
	.byte	0x8
	.4byte	0x444
	.byte	0xe
	.string	"_p"
	.byte	0x5
	.byte	0xb6
	.byte	0x12
	.4byte	0x2fb
	.byte	0
	.byte	0xe
	.string	"_r"
	.byte	0x5
	.byte	0xb7
	.byte	0x7
	.4byte	0x5d
	.byte	0x4
	.byte	0xe
	.string	"_w"
	.byte	0x5
	.byte	0xb8
	.byte	0x7
	.4byte	0x5d
	.byte	0x8
	.byte	0xb
	.4byte	.LASF49
	.byte	0x5
	.byte	0xb9
	.byte	0x9
	.4byte	0x33
	.byte	0xc
	.byte	0xb
	.4byte	.LASF50
	.byte	0x5
	.byte	0xba
	.byte	0x9
	.4byte	0x33
	.byte	0xe
	.byte	0xe
	.string	"_bf"
	.byte	0x5
	.byte	0xbb
	.byte	0x11
	.4byte	0x2d3
	.byte	0x10
	.byte	0xb
	.4byte	.LASF51
	.byte	0x5
	.byte	0xbc
	.byte	0x7
	.4byte	0x5d
	.byte	0x18
	.byte	0xb
	.4byte	.LASF52
	.byte	0x5
	.byte	0xc3
	.byte	0x8
	.4byte	0x11d
	.byte	0x1c
	.byte	0xb
	.4byte	.LASF53
	.byte	0x5
	.byte	0xc5
	.byte	0x1b
	.4byte	0x5c8
	.byte	0x20
	.byte	0xb
	.4byte	.LASF54
	.byte	0x5
	.byte	0xc7
	.byte	0x1b
	.4byte	0x5f2
	.byte	0x24
	.byte	0xb
	.4byte	.LASF55
	.byte	0x5
	.byte	0xca
	.byte	0xb
	.4byte	0x616
	.byte	0x28
	.byte	0xb
	.4byte	.LASF56
	.byte	0x5
	.byte	0xcb
	.byte	0x7
	.4byte	0x630
	.byte	0x2c
	.byte	0xe
	.string	"_ub"
	.byte	0x5
	.byte	0xce
	.byte	0x11
	.4byte	0x2d3
	.byte	0x30
	.byte	0xe
	.string	"_up"
	.byte	0x5
	.byte	0xcf
	.byte	0x12
	.4byte	0x2fb
	.byte	0x38
	.byte	0xe
	.string	"_ur"
	.byte	0x5
	.byte	0xd0
	.byte	0x7
	.4byte	0x5d
	.byte	0x3c
	.byte	0xb
	.4byte	.LASF57
	.byte	0x5
	.byte	0xd3
	.byte	0x11
	.4byte	0x636
	.byte	0x40
	.byte	0xb
	.4byte	.LASF58
	.byte	0x5
	.byte	0xd4
	.byte	0x11
	.4byte	0x646
	.byte	0x43
	.byte	0xe
	.string	"_lb"
	.byte	0x5
	.byte	0xd7
	.byte	0x11
	.4byte	0x2d3
	.byte	0x44
	.byte	0xb
	.4byte	.LASF59
	.byte	0x5
	.byte	0xda
	.byte	0x7
	.4byte	0x5d
	.byte	0x4c
	.byte	0xb
	.4byte	.LASF60
	.byte	0x5
	.byte	0xdb
	.byte	0xa
	.4byte	0x7e
	.byte	0x50
	.byte	0xb
	.4byte	.LASF61
	.byte	0x5
	.byte	0xde
	.byte	0x12
	.4byte	0x462
	.byte	0x54
	.byte	0xb
	.4byte	.LASF62
	.byte	0x5
	.byte	0xe2
	.byte	0xc
	.4byte	0x111
	.byte	0x58
	.byte	0xb
	.4byte	.LASF63
	.byte	0x5
	.byte	0xe4
	.byte	0xe
	.4byte	0x105
	.byte	0x5c
	.byte	0xb
	.4byte	.LASF64
	.byte	0x5
	.byte	0xe5
	.byte	0x9
	.4byte	0x5d
	.byte	0x64
	.byte	0
	.byte	0x13
	.4byte	0x96
	.4byte	0x462
	.byte	0x14
	.4byte	0x462
	.byte	0x14
	.4byte	0x11d
	.byte	0x14
	.4byte	0x5b6
	.byte	0x14
	.4byte	0x5d
	.byte	0
	.byte	0xf
	.byte	0x4
	.4byte	0x46d
	.byte	0x15
	.4byte	0x462
	.byte	0x16
	.4byte	.LASF65
	.2byte	0x428
	.byte	0x5
	.2byte	0x239
	.byte	0x8
	.4byte	0x5b6
	.byte	0x17
	.4byte	.LASF66
	.byte	0x5
	.2byte	0x23b
	.byte	0x7
	.4byte	0x5d
	.byte	0
	.byte	0x17
	.4byte	.LASF67
	.byte	0x5
	.2byte	0x240
	.byte	0xb
	.4byte	0x6a2
	.byte	0x4
	.byte	0x17
	.4byte	.LASF68
	.byte	0x5
	.2byte	0x240
	.byte	0x14
	.4byte	0x6a2
	.byte	0x8
	.byte	0x17
	.4byte	.LASF69
	.byte	0x5
	.2byte	0x240
	.byte	0x1e
	.4byte	0x6a2
	.byte	0xc
	.byte	0x17
	.4byte	.LASF70
	.byte	0x5
	.2byte	0x242
	.byte	0x8
	.4byte	0x5d
	.byte	0x10
	.byte	0x17
	.4byte	.LASF71
	.byte	0x5
	.2byte	0x243
	.byte	0x8
	.4byte	0x8a2
	.byte	0x14
	.byte	0x17
	.4byte	.LASF72
	.byte	0x5
	.2byte	0x246
	.byte	0x7
	.4byte	0x5d
	.byte	0x30
	.byte	0x17
	.4byte	.LASF73
	.byte	0x5
	.2byte	0x247
	.byte	0x16
	.4byte	0x8b7
	.byte	0x34
	.byte	0x17
	.4byte	.LASF74
	.byte	0x5
	.2byte	0x249
	.byte	0x7
	.4byte	0x5d
	.byte	0x38
	.byte	0x17
	.4byte	.LASF75
	.byte	0x5
	.2byte	0x24b
	.byte	0x8
	.4byte	0x8c8
	.byte	0x3c
	.byte	0x17
	.4byte	.LASF76
	.byte	0x5
	.2byte	0x24e
	.byte	0x13
	.4byte	0x185
	.byte	0x40
	.byte	0x17
	.4byte	.LASF77
	.byte	0x5
	.2byte	0x24f
	.byte	0x7
	.4byte	0x5d
	.byte	0x44
	.byte	0x17
	.4byte	.LASF78
	.byte	0x5
	.2byte	0x250
	.byte	0x13
	.4byte	0x185
	.byte	0x48
	.byte	0x17
	.4byte	.LASF79
	.byte	0x5
	.2byte	0x251
	.byte	0x14
	.4byte	0x8ce
	.byte	0x4c
	.byte	0x17
	.4byte	.LASF80
	.byte	0x5
	.2byte	0x254
	.byte	0x7
	.4byte	0x5d
	.byte	0x50
	.byte	0x17
	.4byte	.LASF81
	.byte	0x5
	.2byte	0x255
	.byte	0x9
	.4byte	0x5b6
	.byte	0x54
	.byte	0x17
	.4byte	.LASF82
	.byte	0x5
	.2byte	0x278
	.byte	0x7
	.4byte	0x87d
	.byte	0x58
	.byte	0x18
	.4byte	.LASF42
	.byte	0x5
	.2byte	0x27c
	.byte	0x13
	.4byte	0x2b6
	.2byte	0x148
	.byte	0x18
	.4byte	.LASF83
	.byte	0x5
	.2byte	0x27d
	.byte	0x12
	.4byte	0x273
	.2byte	0x14c
	.byte	0x18
	.4byte	.LASF84
	.byte	0x5
	.2byte	0x281
	.byte	0xc
	.4byte	0x8df
	.2byte	0x2dc
	.byte	0x18
	.4byte	.LASF85
	.byte	0x5
	.2byte	0x286
	.byte	0x10
	.4byte	0x663
	.2byte	0x2e0
	.byte	0x18
	.4byte	.LASF86
	.byte	0x5
	.2byte	0x288
	.byte	0xa
	.4byte	0x8eb
	.2byte	0x2ec
	.byte	0
	.byte	0xf
	.byte	0x4
	.4byte	0x5bc
	.byte	0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF87
	.byte	0x15
	.4byte	0x5bc
	.byte	0xf
	.byte	0x4
	.4byte	0x444
	.byte	0x13
	.4byte	0x96
	.4byte	0x5ec
	.byte	0x14
	.4byte	0x462
	.byte	0x14
	.4byte	0x11d
	.byte	0x14
	.4byte	0x5ec
	.byte	0x14
	.4byte	0x5d
	.byte	0
	.byte	0xf
	.byte	0x4
	.4byte	0x5c3
	.byte	0xf
	.byte	0x4
	.4byte	0x5ce
	.byte	0x13
	.4byte	0x8a
	.4byte	0x616
	.byte	0x14
	.4byte	0x462
	.byte	0x14
	.4byte	0x11d
	.byte	0x14
	.4byte	0x8a
	.byte	0x14
	.4byte	0x5d
	.byte	0
	.byte	0xf
	.byte	0x4
	.4byte	0x5f8
	.byte	0x13
	.4byte	0x5d
	.4byte	0x630
	.byte	0x14
	.4byte	0x462
	.byte	0x14
	.4byte	0x11d
	.byte	0
	.byte	0xf
	.byte	0x4
	.4byte	0x61c
	.byte	0x8
	.4byte	0x2c
	.4byte	0x646
	.byte	0x9
	.4byte	0x64
	.byte	0x2
	.byte	0
	.byte	0x8
	.4byte	0x2c
	.4byte	0x656
	.byte	0x9
	.4byte	0x64
	.byte	0
	.byte	0
	.byte	0x5
	.4byte	.LASF88
	.byte	0x5
	.2byte	0x11f
	.byte	0x1a
	.4byte	0x301
	.byte	0x19
	.4byte	.LASF89
	.byte	0xc
	.byte	0x5
	.2byte	0x123
	.byte	0x8
	.4byte	0x69c
	.byte	0x17
	.4byte	.LASF22
	.byte	0x5
	.2byte	0x125
	.byte	0x11
	.4byte	0x69c
	.byte	0
	.byte	0x17
	.4byte	.LASF90
	.byte	0x5
	.2byte	0x126
	.byte	0x7
	.4byte	0x5d
	.byte	0x4
	.byte	0x17
	.4byte	.LASF91
	.byte	0x5
	.2byte	0x127
	.byte	0xb
	.4byte	0x6a2
	.byte	0x8
	.byte	0
	.byte	0xf
	.byte	0x4
	.4byte	0x663
	.byte	0xf
	.byte	0x4
	.4byte	0x656
	.byte	0x19
	.4byte	.LASF92
	.byte	0xe
	.byte	0x5
	.2byte	0x13f
	.byte	0x8
	.4byte	0x6e1
	.byte	0x17
	.4byte	.LASF93
	.byte	0x5
	.2byte	0x140
	.byte	0x12
	.4byte	0x6e1
	.byte	0
	.byte	0x17
	.4byte	.LASF94
	.byte	0x5
	.2byte	0x141
	.byte	0x12
	.4byte	0x6e1
	.byte	0x6
	.byte	0x17
	.4byte	.LASF95
	.byte	0x5
	.2byte	0x142
	.byte	0x12
	.4byte	0x3a
	.byte	0xc
	.byte	0
	.byte	0x8
	.4byte	0x3a
	.4byte	0x6f1
	.byte	0x9
	.4byte	0x64
	.byte	0x2
	.byte	0
	.byte	0x1a
	.byte	0xd0
	.byte	0x5
	.2byte	0x259
	.byte	0x7
	.4byte	0x806
	.byte	0x17
	.4byte	.LASF96
	.byte	0x5
	.2byte	0x25b
	.byte	0x18
	.4byte	0x64
	.byte	0
	.byte	0x17
	.4byte	.LASF97
	.byte	0x5
	.2byte	0x25c
	.byte	0x12
	.4byte	0x5b6
	.byte	0x4
	.byte	0x17
	.4byte	.LASF98
	.byte	0x5
	.2byte	0x25d
	.byte	0x10
	.4byte	0x806
	.byte	0x8
	.byte	0x17
	.4byte	.LASF99
	.byte	0x5
	.2byte	0x25e
	.byte	0x17
	.4byte	0x19b
	.byte	0x24
	.byte	0x17
	.4byte	.LASF100
	.byte	0x5
	.2byte	0x25f
	.byte	0xf
	.4byte	0x5d
	.byte	0x48
	.byte	0x17
	.4byte	.LASF101
	.byte	0x5
	.2byte	0x260
	.byte	0x2c
	.4byte	0x56
	.byte	0x50
	.byte	0x17
	.4byte	.LASF102
	.byte	0x5
	.2byte	0x261
	.byte	0x1a
	.4byte	0x6a8
	.byte	0x58
	.byte	0x17
	.4byte	.LASF103
	.byte	0x5
	.2byte	0x262
	.byte	0x16
	.4byte	0x105
	.byte	0x68
	.byte	0x17
	.4byte	.LASF104
	.byte	0x5
	.2byte	0x263
	.byte	0x16
	.4byte	0x105
	.byte	0x70
	.byte	0x17
	.4byte	.LASF105
	.byte	0x5
	.2byte	0x264
	.byte	0x16
	.4byte	0x105
	.byte	0x78
	.byte	0x17
	.4byte	.LASF106
	.byte	0x5
	.2byte	0x265
	.byte	0x10
	.4byte	0x816
	.byte	0x80
	.byte	0x17
	.4byte	.LASF107
	.byte	0x5
	.2byte	0x266
	.byte	0x10
	.4byte	0x826
	.byte	0x88
	.byte	0x17
	.4byte	.LASF108
	.byte	0x5
	.2byte	0x267
	.byte	0xf
	.4byte	0x5d
	.byte	0xa0
	.byte	0x17
	.4byte	.LASF109
	.byte	0x5
	.2byte	0x268
	.byte	0x16
	.4byte	0x105
	.byte	0xa4
	.byte	0x17
	.4byte	.LASF110
	.byte	0x5
	.2byte	0x269
	.byte	0x16
	.4byte	0x105
	.byte	0xac
	.byte	0x17
	.4byte	.LASF111
	.byte	0x5
	.2byte	0x26a
	.byte	0x16
	.4byte	0x105
	.byte	0xb4
	.byte	0x17
	.4byte	.LASF112
	.byte	0x5
	.2byte	0x26b
	.byte	0x16
	.4byte	0x105
	.byte	0xbc
	.byte	0x17
	.4byte	.LASF113
	.byte	0x5
	.2byte	0x26c
	.byte	0x16
	.4byte	0x105
	.byte	0xc4
	.byte	0x17
	.4byte	.LASF114
	.byte	0x5
	.2byte	0x26d
	.byte	0x8
	.4byte	0x5d
	.byte	0xcc
	.byte	0
	.byte	0x8
	.4byte	0x5bc
	.4byte	0x816
	.byte	0x9
	.4byte	0x64
	.byte	0x19
	.byte	0
	.byte	0x8
	.4byte	0x5bc
	.4byte	0x826
	.byte	0x9
	.4byte	0x64
	.byte	0x7
	.byte	0
	.byte	0x8
	.4byte	0x5bc
	.4byte	0x836
	.byte	0x9
	.4byte	0x64
	.byte	0x17
	.byte	0
	.byte	0x1a
	.byte	0xf0
	.byte	0x5
	.2byte	0x272
	.byte	0x7
	.4byte	0x85d
	.byte	0x17
	.4byte	.LASF115
	.byte	0x5
	.2byte	0x275
	.byte	0x1b
	.4byte	0x85d
	.byte	0
	.byte	0x17
	.4byte	.LASF116
	.byte	0x5
	.2byte	0x276
	.byte	0x18
	.4byte	0x86d
	.byte	0x78
	.byte	0
	.byte	0x8
	.4byte	0x2fb
	.4byte	0x86d
	.byte	0x9
	.4byte	0x64
	.byte	0x1d
	.byte	0
	.byte	0x8
	.4byte	0x64
	.4byte	0x87d
	.byte	0x9
	.4byte	0x64
	.byte	0x1d
	.byte	0
	.byte	0x1b
	.byte	0xf0
	.byte	0x5
	.2byte	0x257
	.byte	0x3
	.4byte	0x8a2
	.byte	0x1c
	.4byte	.LASF65
	.byte	0x5
	.2byte	0x26e
	.byte	0xb
	.4byte	0x6f1
	.byte	0x1c
	.4byte	.LASF117
	.byte	0x5
	.2byte	0x277
	.byte	0xb
	.4byte	0x836
	.byte	0
	.byte	0x8
	.4byte	0x5bc
	.4byte	0x8b2
	.byte	0x9
	.4byte	0x64
	.byte	0x18
	.byte	0
	.byte	0x1d
	.4byte	.LASF139
	.byte	0xf
	.byte	0x4
	.4byte	0x8b2
	.byte	0x1e
	.4byte	0x8c8
	.byte	0x14
	.4byte	0x462
	.byte	0
	.byte	0xf
	.byte	0x4
	.4byte	0x8bd
	.byte	0xf
	.byte	0x4
	.4byte	0x185
	.byte	0x1e
	.4byte	0x8df
	.byte	0x14
	.4byte	0x5d
	.byte	0
	.byte	0xf
	.byte	0x4
	.4byte	0x8e5
	.byte	0xf
	.byte	0x4
	.4byte	0x8d4
	.byte	0x8
	.4byte	0x656
	.4byte	0x8fb
	.byte	0x9
	.4byte	0x64
	.byte	0x2
	.byte	0
	.byte	0x1f
	.4byte	.LASF118
	.byte	0x5
	.2byte	0x307
	.byte	0x17
	.4byte	0x462
	.byte	0x1f
	.4byte	.LASF119
	.byte	0x5
	.2byte	0x308
	.byte	0x1e
	.4byte	0x468
	.byte	0x20
	.4byte	.LASF120
	.byte	0x6
	.byte	0x63
	.byte	0xe
	.4byte	0x5b6
	.byte	0x2
	.byte	0x4
	.byte	0x4
	.4byte	.LASF121
	.byte	0x2
	.byte	0x8
	.byte	0x4
	.4byte	.LASF122
	.byte	0x21
	.4byte	.LASF140
	.byte	0x5
	.byte	0x4
	.4byte	0x5d
	.byte	0x7
	.2byte	0x282
	.byte	0x6
	.4byte	0x95b
	.byte	0x22
	.4byte	.LASF123
	.byte	0x7f
	.byte	0x23
	.4byte	.LASF124
	.byte	0
	.byte	0x23
	.4byte	.LASF125
	.byte	0x1
	.byte	0x23
	.4byte	.LASF126
	.byte	0x2
	.byte	0
	.byte	0x1f
	.4byte	.LASF127
	.byte	0x7
	.2byte	0x28d
	.byte	0x23
	.4byte	0x92f
	.byte	0x24
	.string	"c"
	.byte	0x1
	.byte	0x8
	.byte	0x7
	.4byte	0x921
	.byte	0x5
	.byte	0x3
	.4byte	c
	.byte	0x24
	.string	"b"
	.byte	0x1
	.byte	0x9
	.byte	0x7
	.4byte	0x921
	.byte	0x5
	.byte	0x3
	.4byte	b
	.byte	0x25
	.4byte	.LASF135
	.byte	0x1
	.byte	0x32
	.byte	0x5
	.4byte	0x5d
	.4byte	.LFB7
	.4byte	.LFE7-.LFB7
	.byte	0x1
	.byte	0x9c
	.4byte	0x9df
	.byte	0x26
	.4byte	.LASF128
	.byte	0x1
	.byte	0x34
	.byte	0x8
	.4byte	0x921
	.byte	0x2
	.byte	0x91
	.byte	0x6c
	.byte	0x26
	.4byte	.LASF129
	.byte	0x1
	.byte	0x34
	.byte	0xf
	.4byte	0x921
	.byte	0x2
	.byte	0x91
	.byte	0x68
	.byte	0x26
	.4byte	.LASF130
	.byte	0x1
	.byte	0x34
	.byte	0x16
	.4byte	0x921
	.byte	0x2
	.byte	0x91
	.byte	0x60
	.byte	0x26
	.4byte	.LASF131
	.byte	0x1
	.byte	0x34
	.byte	0x1c
	.4byte	0x921
	.byte	0x2
	.byte	0x91
	.byte	0x64
	.byte	0
	.byte	0x27
	.4byte	.LASF141
	.byte	0x1
	.byte	0x1e
	.byte	0x6
	.4byte	.LFB6
	.4byte	.LFE6-.LFB6
	.byte	0x1
	.byte	0x9c
	.4byte	0xa24
	.byte	0x28
	.string	"x"
	.byte	0x1
	.byte	0x1f
	.byte	0x8
	.4byte	0x921
	.byte	0x3
	.byte	0x91
	.byte	0x9c,0x7f
	.byte	0x26
	.4byte	.LASF132
	.byte	0x1
	.byte	0x20
	.byte	0x8
	.4byte	0x921
	.byte	0x3
	.byte	0x91
	.byte	0x98,0x7f
	.byte	0x26
	.4byte	.LASF133
	.byte	0x1
	.byte	0x21
	.byte	0x8
	.4byte	0x921
	.byte	0x3
	.byte	0x91
	.byte	0x94,0x7f
	.byte	0
	.byte	0x29
	.4byte	.LASF134
	.byte	0x1
	.byte	0x15
	.byte	0x7
	.4byte	0x921
	.4byte	.LFB5
	.4byte	.LFE5-.LFB5
	.byte	0x1
	.byte	0x9c
	.4byte	0xa77
	.byte	0x2a
	.string	"x"
	.byte	0x1
	.byte	0x15
	.byte	0x1e
	.4byte	0x921
	.byte	0x2
	.byte	0x91
	.byte	0x5c
	.byte	0x2a
	.string	"y"
	.byte	0x1
	.byte	0x15
	.byte	0x27
	.4byte	0x921
	.byte	0x2
	.byte	0x91
	.byte	0x58
	.byte	0x26
	.4byte	.LASF133
	.byte	0x1
	.byte	0x19
	.byte	0x8
	.4byte	0x921
	.byte	0x2
	.byte	0x91
	.byte	0x6c
	.byte	0x28
	.string	"min"
	.byte	0x1
	.byte	0x1a
	.byte	0x8
	.4byte	0x921
	.byte	0x2
	.byte	0x91
	.byte	0x68
	.byte	0
	.byte	0x2b
	.string	"df"
	.byte	0x1
	.byte	0x10
	.byte	0x7
	.4byte	0x921
	.4byte	.LFB4
	.4byte	.LFE4-.LFB4
	.byte	0x1
	.byte	0x9c
	.4byte	0xa9e
	.byte	0x2a
	.string	"x"
	.byte	0x1
	.byte	0x10
	.byte	0x10
	.4byte	0x921
	.byte	0x2
	.byte	0x91
	.byte	0x6c
	.byte	0
	.byte	0x2c
	.string	"f"
	.byte	0x1
	.byte	0xb
	.byte	0x7
	.4byte	0x921
	.4byte	.LFB3
	.4byte	.LFE3-.LFB3
	.byte	0x1
	.byte	0x9c
	.byte	0x2a
	.string	"x"
	.byte	0x1
	.byte	0xb
	.byte	0xf
	.4byte	0x921
	.byte	0x2
	.byte	0x91
	.byte	0x6c
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.byte	0x1
	.byte	0x11
	.byte	0x1
	.byte	0x25
	.byte	0xe
	.byte	0x13
	.byte	0xb
	.byte	0x3
	.byte	0xe
	.byte	0x1b
	.byte	0xe
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x6
	.byte	0x10
	.byte	0x17
	.byte	0
	.byte	0
	.byte	0x2
	.byte	0x24
	.byte	0
	.byte	0xb
	.byte	0xb
	.byte	0x3e
	.byte	0xb
	.byte	0x3
	.byte	0xe
	.byte	0
	.byte	0
	.byte	0x3
	.byte	0x24
	.byte	0
	.byte	0xb
	.byte	0xb
	.byte	0x3e
	.byte	0xb
	.byte	0x3
	.byte	0x8
	.byte	0
	.byte	0
	.byte	0x4
	.byte	0x16
	.byte	0
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x5
	.byte	0x16
	.byte	0
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0x5
	.byte	0x39
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x6
	.byte	0x17
	.byte	0x1
	.byte	0xb
	.byte	0xb
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x1
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x7
	.byte	0xd
	.byte	0
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x8
	.byte	0x1
	.byte	0x1
	.byte	0x49
	.byte	0x13
	.byte	0x1
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x9
	.byte	0x21
	.byte	0
	.byte	0x49
	.byte	0x13
	.byte	0x2f
	.byte	0xb
	.byte	0
	.byte	0
	.byte	0xa
	.byte	0x13
	.byte	0x1
	.byte	0xb
	.byte	0xb
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x1
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0xb
	.byte	0xd
	.byte	0
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x38
	.byte	0xb
	.byte	0
	.byte	0
	.byte	0xc
	.byte	0xf
	.byte	0
	.byte	0xb
	.byte	0xb
	.byte	0
	.byte	0
	.byte	0xd
	.byte	0x13
	.byte	0x1
	.byte	0x3
	.byte	0xe
	.byte	0xb
	.byte	0xb
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x1
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0xe
	.byte	0xd
	.byte	0
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x38
	.byte	0xb
	.byte	0
	.byte	0
	.byte	0xf
	.byte	0xf
	.byte	0
	.byte	0xb
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x10
	.byte	0x13
	.byte	0x1
	.byte	0x3
	.byte	0xe
	.byte	0xb
	.byte	0x5
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x1
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x11
	.byte	0xd
	.byte	0
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x38
	.byte	0x5
	.byte	0
	.byte	0
	.byte	0x12
	.byte	0x15
	.byte	0
	.byte	0x27
	.byte	0x19
	.byte	0
	.byte	0
	.byte	0x13
	.byte	0x15
	.byte	0x1
	.byte	0x27
	.byte	0x19
	.byte	0x49
	.byte	0x13
	.byte	0x1
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x14
	.byte	0x5
	.byte	0
	.byte	0x49
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x15
	.byte	0x26
	.byte	0
	.byte	0x49
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x16
	.byte	0x13
	.byte	0x1
	.byte	0x3
	.byte	0xe
	.byte	0xb
	.byte	0x5
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0x5
	.byte	0x39
	.byte	0xb
	.byte	0x1
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x17
	.byte	0xd
	.byte	0
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0x5
	.byte	0x39
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x38
	.byte	0xb
	.byte	0
	.byte	0
	.byte	0x18
	.byte	0xd
	.byte	0
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0x5
	.byte	0x39
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x38
	.byte	0x5
	.byte	0
	.byte	0
	.byte	0x19
	.byte	0x13
	.byte	0x1
	.byte	0x3
	.byte	0xe
	.byte	0xb
	.byte	0xb
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0x5
	.byte	0x39
	.byte	0xb
	.byte	0x1
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x1a
	.byte	0x13
	.byte	0x1
	.byte	0xb
	.byte	0xb
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0x5
	.byte	0x39
	.byte	0xb
	.byte	0x1
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x1b
	.byte	0x17
	.byte	0x1
	.byte	0xb
	.byte	0xb
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0x5
	.byte	0x39
	.byte	0xb
	.byte	0x1
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x1c
	.byte	0xd
	.byte	0
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0x5
	.byte	0x39
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x1d
	.byte	0x13
	.byte	0
	.byte	0x3
	.byte	0xe
	.byte	0x3c
	.byte	0x19
	.byte	0
	.byte	0
	.byte	0x1e
	.byte	0x15
	.byte	0x1
	.byte	0x27
	.byte	0x19
	.byte	0x1
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x1f
	.byte	0x34
	.byte	0
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0x5
	.byte	0x39
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x3f
	.byte	0x19
	.byte	0x3c
	.byte	0x19
	.byte	0
	.byte	0
	.byte	0x20
	.byte	0x34
	.byte	0
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x3f
	.byte	0x19
	.byte	0x3c
	.byte	0x19
	.byte	0
	.byte	0
	.byte	0x21
	.byte	0x4
	.byte	0x1
	.byte	0x3
	.byte	0xe
	.byte	0x3e
	.byte	0xb
	.byte	0xb
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0x5
	.byte	0x39
	.byte	0xb
	.byte	0x1
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x22
	.byte	0x28
	.byte	0
	.byte	0x3
	.byte	0xe
	.byte	0x1c
	.byte	0xd
	.byte	0
	.byte	0
	.byte	0x23
	.byte	0x28
	.byte	0
	.byte	0x3
	.byte	0xe
	.byte	0x1c
	.byte	0xb
	.byte	0
	.byte	0
	.byte	0x24
	.byte	0x34
	.byte	0
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x3f
	.byte	0x19
	.byte	0x2
	.byte	0x18
	.byte	0
	.byte	0
	.byte	0x25
	.byte	0x2e
	.byte	0x1
	.byte	0x3f
	.byte	0x19
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x6
	.byte	0x40
	.byte	0x18
	.byte	0x96,0x42
	.byte	0x19
	.byte	0x1
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x26
	.byte	0x34
	.byte	0
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x2
	.byte	0x18
	.byte	0
	.byte	0
	.byte	0x27
	.byte	0x2e
	.byte	0x1
	.byte	0x3f
	.byte	0x19
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x6
	.byte	0x40
	.byte	0x18
	.byte	0x96,0x42
	.byte	0x19
	.byte	0x1
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x28
	.byte	0x34
	.byte	0
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x2
	.byte	0x18
	.byte	0
	.byte	0
	.byte	0x29
	.byte	0x2e
	.byte	0x1
	.byte	0x3f
	.byte	0x19
	.byte	0x3
	.byte	0xe
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x27
	.byte	0x19
	.byte	0x49
	.byte	0x13
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x6
	.byte	0x40
	.byte	0x18
	.byte	0x96,0x42
	.byte	0x19
	.byte	0x1
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x2a
	.byte	0x5
	.byte	0
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x49
	.byte	0x13
	.byte	0x2
	.byte	0x18
	.byte	0
	.byte	0
	.byte	0x2b
	.byte	0x2e
	.byte	0x1
	.byte	0x3f
	.byte	0x19
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x27
	.byte	0x19
	.byte	0x49
	.byte	0x13
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x6
	.byte	0x40
	.byte	0x18
	.byte	0x97,0x42
	.byte	0x19
	.byte	0x1
	.byte	0x13
	.byte	0
	.byte	0
	.byte	0x2c
	.byte	0x2e
	.byte	0x1
	.byte	0x3f
	.byte	0x19
	.byte	0x3
	.byte	0x8
	.byte	0x3a
	.byte	0xb
	.byte	0x3b
	.byte	0xb
	.byte	0x39
	.byte	0xb
	.byte	0x27
	.byte	0x19
	.byte	0x49
	.byte	0x13
	.byte	0x11
	.byte	0x1
	.byte	0x12
	.byte	0x6
	.byte	0x40
	.byte	0x18
	.byte	0x97,0x42
	.byte	0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.4byte	0x1c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	0
	.4byte	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF39:
	.string	"_dso_handle"
.LASF134:
	.string	"percentage_error"
.LASF47:
	.string	"_size"
.LASF92:
	.string	"_rand48"
.LASF138:
	.string	"/home/samuelmswong/harrysun/benchmarks/source/riscv_newton-raphson"
.LASF71:
	.string	"_emergency"
.LASF61:
	.string	"_data"
.LASF131:
	.string	"stddev_ratio"
.LASF112:
	.string	"_wcrtomb_state"
.LASF113:
	.string	"_wcsrtombs_state"
.LASF7:
	.string	"long long unsigned int"
.LASF126:
	.string	"__fdlibm_posix"
.LASF51:
	.string	"_lbfsize"
.LASF133:
	.string	"diff"
.LASF139:
	.string	"__locale_t"
.LASF136:
	.string	"GNU C17 8.2.0 -march=rv32ifd -mabi=ilp32d -g -O0"
.LASF125:
	.string	"__fdlibm_xopen"
.LASF110:
	.string	"_mbrtowc_state"
.LASF105:
	.string	"_wctomb_state"
.LASF28:
	.string	"__tm_sec"
.LASF6:
	.string	"long long int"
.LASF0:
	.string	"signed char"
.LASF137:
	.string	"newton_rapson.c"
.LASF57:
	.string	"_ubuf"
.LASF46:
	.string	"_base"
.LASF30:
	.string	"__tm_hour"
.LASF86:
	.string	"__sf"
.LASF37:
	.string	"_on_exit_args"
.LASF52:
	.string	"_cookie"
.LASF85:
	.string	"__sglue"
.LASF4:
	.string	"long int"
.LASF49:
	.string	"_flags"
.LASF41:
	.string	"_is_cxa"
.LASF67:
	.string	"_stdin"
.LASF124:
	.string	"__fdlibm_svid"
.LASF59:
	.string	"_blksize"
.LASF122:
	.string	"double"
.LASF129:
	.string	"var_c"
.LASF81:
	.string	"_cvtbuf"
.LASF60:
	.string	"_offset"
.LASF141:
	.string	"newton_rapson"
.LASF111:
	.string	"_mbsrtowcs_state"
.LASF109:
	.string	"_mbrlen_state"
.LASF38:
	.string	"_fnargs"
.LASF44:
	.string	"_fns"
.LASF24:
	.string	"_sign"
.LASF20:
	.string	"_flock_t"
.LASF69:
	.string	"_stderr"
.LASF26:
	.string	"_Bigint"
.LASF100:
	.string	"_gamma_signgam"
.LASF53:
	.string	"_read"
.LASF77:
	.string	"_result_k"
.LASF27:
	.string	"__tm"
.LASF128:
	.string	"var_b"
.LASF8:
	.string	"unsigned int"
.LASF132:
	.string	"x_new"
.LASF16:
	.string	"__wchb"
.LASF68:
	.string	"_stdout"
.LASF80:
	.string	"_cvtlen"
.LASF5:
	.string	"long unsigned int"
.LASF50:
	.string	"_file"
.LASF90:
	.string	"_niobs"
.LASF3:
	.string	"short unsigned int"
.LASF83:
	.string	"_atexit0"
.LASF107:
	.string	"_signal_buf"
.LASF98:
	.string	"_asctime_buf"
.LASF76:
	.string	"_result"
.LASF15:
	.string	"__wch"
.LASF14:
	.string	"wint_t"
.LASF62:
	.string	"_lock"
.LASF64:
	.string	"_flags2"
.LASF54:
	.string	"_write"
.LASF33:
	.string	"__tm_year"
.LASF116:
	.string	"_nmalloc"
.LASF9:
	.string	"long double"
.LASF115:
	.string	"_nextf"
.LASF32:
	.string	"__tm_mon"
.LASF42:
	.string	"_atexit"
.LASF120:
	.string	"suboptarg"
.LASF74:
	.string	"__sdidinit"
.LASF11:
	.string	"_off_t"
.LASF121:
	.string	"float"
.LASF79:
	.string	"_freelist"
.LASF10:
	.string	"_LOCK_RECURSIVE_T"
.LASF1:
	.string	"unsigned char"
.LASF82:
	.string	"_new"
.LASF114:
	.string	"_h_errno"
.LASF2:
	.string	"short int"
.LASF35:
	.string	"__tm_yday"
.LASF45:
	.string	"__sbuf"
.LASF91:
	.string	"_iobs"
.LASF88:
	.string	"__FILE"
.LASF19:
	.string	"_mbstate_t"
.LASF48:
	.string	"__sFILE"
.LASF63:
	.string	"_mbstate"
.LASF101:
	.string	"_rand_next"
.LASF103:
	.string	"_mblen_state"
.LASF70:
	.string	"_inc"
.LASF43:
	.string	"_ind"
.LASF73:
	.string	"_locale"
.LASF75:
	.string	"__cleanup"
.LASF72:
	.string	"_unspecified_locale_info"
.LASF23:
	.string	"_maxwds"
.LASF65:
	.string	"_reent"
.LASF93:
	.string	"_seed"
.LASF17:
	.string	"__count"
.LASF18:
	.string	"__value"
.LASF55:
	.string	"_seek"
.LASF118:
	.string	"_impure_ptr"
.LASF12:
	.string	"_fpos_t"
.LASF66:
	.string	"_errno"
.LASF87:
	.string	"char"
.LASF29:
	.string	"__tm_min"
.LASF130:
	.string	"four"
.LASF94:
	.string	"_mult"
.LASF22:
	.string	"_next"
.LASF97:
	.string	"_strtok_last"
.LASF40:
	.string	"_fntypes"
.LASF95:
	.string	"_add"
.LASF21:
	.string	"__ULong"
.LASF108:
	.string	"_getdate_err"
.LASF140:
	.string	"__fdlibm_version"
.LASF119:
	.string	"_global_impure_ptr"
.LASF123:
	.string	"__fdlibm_ieee"
.LASF96:
	.string	"_unused_rand"
.LASF25:
	.string	"_wds"
.LASF34:
	.string	"__tm_wday"
.LASF89:
	.string	"_glue"
.LASF127:
	.string	"__fdlib_version"
.LASF13:
	.string	"_ssize_t"
.LASF106:
	.string	"_l64a_buf"
.LASF84:
	.string	"_sig_func"
.LASF58:
	.string	"_nbuf"
.LASF117:
	.string	"_unused"
.LASF36:
	.string	"__tm_isdst"
.LASF99:
	.string	"_localtime_buf"
.LASF56:
	.string	"_close"
.LASF102:
	.string	"_r48"
.LASF104:
	.string	"_mbtowc_state"
.LASF78:
	.string	"_p5s"
.LASF135:
	.string	"main"
.LASF31:
	.string	"__tm_mday"
	.ident	"GCC: (GNU) 8.2.0"
