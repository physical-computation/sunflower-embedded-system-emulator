#
#	Makefile for fftw-3.3alpha1/rdft on Sunflowersim
#
TREEROOT	= ../../../..

include $(TREEROOT)/conf/setup.conf
INCLUDEDIR      = $(TREEROOT)/sys/include
INCLUDES        = -I$(INCLUDEDIR) -I../ -I../dft/ -I../kernel -I../../port -I$(TREEROOT)/sys/libOS/port -I$(TREEROOT)/sys/kern/superH -I$(TREEROOT)/sim
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -Wall
OPTFLAGS	= -gstabs3 -O0

OBJS	=\
	buffered.o\
	buffered2.o\
	conf.o\
	ct-hc2c-direct.o\
	ct-hc2c.o\
	dft-r2hc.o\
	dht-r2hc.o\
	dht-rader.o\
	direct-r2c.o\
	direct-r2r.o\
	direct2.o\
	generic.o\
	hc2hc-direct.o\
	hc2hc-generic.o\
	hc2hc.o\
	indirect.o\
	khc2c.o\
	khc2hc.o\
	kr2c.o\
	kr2r.o\
	nop.o\
	nop2.o\
	plan.o\
	plan2.o\
	problem.o\
	problem2.o\
	rank-geq2-rdft2.o\
	rank-geq2.o\
	rank0-rdft2.o\
	rank0.o\
	rdft-dht.o\
	rdft2-inplace-strides.o\
	rdft2-rdft.o\
	rdft2-strides.o\
	rdft2-tensor-max-index.o\
	solve.o\
	solve2.o\
	vrank-geq1-rdft2.o\
	vrank-geq1.o\
	vrank3-transpose.o\

all:	$(OBJS)

%.o:	%.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) $(OPTFLAGS) -c $<

clean:
	$(RM) *.o
