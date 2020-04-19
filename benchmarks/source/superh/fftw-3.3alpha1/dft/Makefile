#
#	Makefile for fftw-3.3alpha1/dft on Sunflowersim
#
TREEROOT	= ../../../../..

include $(TREEROOT)/conf/setup.conf

INCLUDEDIR      = $(TREEROOT)/sys/include
INCLUDES        = -I$(INCLUDEDIR) -I../ -I../kernel -I../../port -I$(TREEROOT)/sys/libOS/port -I$(TREEROOT)/sys/kern/superH -I$(TREEROOT)/sim
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -Wall
OPTFLAGS	= -gstabs3 -O0

OBJS	=\
	bluestein.o\
	buffered.o\
	conf.o\
	ct.o\
	dftw-direct.o\
	dftw-directsq.o\
	dftw-generic.o\
	dftw-genericbuf.o\
	direct.o\
	generic.o\
	indirect-transpose.o\
	indirect.o\
	kdft-dif.o\
	kdft-difsq.o\
	kdft-dit.o\
	kdft.o\
	nop.o\
	plan.o\
	problem.o\
	rader.o\
	rank-geq2.o\
	solve.o\
	vrank-geq1.o\
	zero.o\


all:	$(OBJS)

%.o:	%.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) $(OPTFLAGS) -c $<

clean:
	$(RM) *.o
