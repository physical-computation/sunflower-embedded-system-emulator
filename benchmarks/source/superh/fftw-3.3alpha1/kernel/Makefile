#
#	Makefile for fftw-3.3alpha1/libkernel on Sunflowersim
#
TREEROOT	= ../../../..

include $(TREEROOT)/conf/setup.conf

INCLUDEDIR      = $(TREEROOT)/sys/include
INCLUDES        = -I$(INCLUDEDIR) -I../ -I../../port -I$(TREEROOT)/sys/libOS/port -I$(TREEROOT)/sys/kern/superH -I$(TREEROOT)/sim
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -Wall
OPTFLAGS	= -gstabs3 -O0

OBJS	=\
	align.o\
	alloc.o\
	assert.o\
	awake.o\
	buffered.o\
	cpy1d.o\
	cpy2d-pair.o\
	cpy2d.o\
	ct.o\
	debug.o\
	extract-reim.o\
	hash.o\
	iabs.o\
	kalloc.o\
	md5-1.o\
	md5.o\
	minmax.o\
	ops.o\
	pickdim.o\
	plan.o\
	planner.o\
	primes.o\
	print.o\
	problem.o\
	rader.o\
	scan.o\
	solver.o\
	solvtab.o\
	stride.o\
	tensor.o\
	tensor1.o\
	tensor2.o\
	tensor3.o\
	tensor4.o\
	tensor5.o\
	tensor7.o\
	tensor8.o\
	tensor9.o\
	tile2d.o\
	timer.o\
	transpose.o\
	trig.o\
	twiddle.o\

all:	$(OBJS)

%.o:	%.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) $(OPTFLAGS) -c $<

clean:
	$(RM) *.o
