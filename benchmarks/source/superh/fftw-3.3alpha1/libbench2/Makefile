#
#	Makefile for fftw-3.3alpha1/libbench2 on Sunflowersim
#
TREEROOT	= ../../../../..

LIB		= ../libfftwbench2

include $(TREEROOT)/conf/setup.conf
INCLUDEDIR      = $(TREEROOT)/sys/include
INCLUDES        = -I$(INCLUDEDIR) -I../ -I../dft/ -I../kernel -I../../port -I$(TREEROOT)/sys/libOS/port -I$(TREEROOT)/sys/kern/superH -I$(TREEROOT)/sim
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -Wall
OPTFLAGS	= -gstabs3 -O0

OBJS	=\
	after-ccopy-from.o\
	after-ccopy-to.o\
	after-hccopy-from.o\
	after-hccopy-to.o\
	after-rcopy-from.o\
	after-rcopy-to.o\
	aligned-main.o\
	allocate.o\
	aset.o\
	bench-cost-postprocess.o\
	bench-exit.o\
	bench-main.o\
	can-do.o\
	caset.o\
	dotens2.o\
	info.o\
	main.o\
	mflops.o\
	mp.o\
	my-getopt.o\
	ovtpvt.o\
	pow2.o\
	problem.o\
	report.o\
	speed.o\
	tensor.o\
	timer.o\
	useropt.o\
	util.o\
	verify-dft.o\
	verify-lib.o\
	verify-r2r.o\
	verify-rdft2.o\
	verify.o\
	zero.o\

$(LIB)-$(TARGET).a:	$(OBJS)
	$(AR) r $(LIB)-$(TARGET).a $(OBJS)

%.o:	%.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) $(OPTFLAGS) -c $<

clean:
	$(RM) *.o $(LIB)-$(TARGET).a
