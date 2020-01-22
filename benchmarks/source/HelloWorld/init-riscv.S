.globl	_start
.globl	__errno
.align	4


_start:
init:
.option push
.option norelax
	la	gp, __global_pointer$
.option pop
	call	main;
	call	exit;


__errno:
	.long	0
