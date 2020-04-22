#
#	Makefile for fftw-3.3alpha1/threads on Sunflowersim
#
TREEROOT	= ../../../../..

LIB		= ../libfftw_threads
include $(TREEROOT)/conf/setup.conf

TARGET		= superH
TARGET-ARCH	= sh-elf

INCLUDEDIR      = $(TREEROOT)/sys/include
INCLUDES        = -I$(INCLUDEDIR) -I../../libsfpthread -I../ -I../dft -I../rdft -I../kernel -I../api -I../../port -I$(TREEROOT)/sys/libOS/port -I$(TREEROOT)/sys/kern/superH -I$(TREEROOT)/sim
CFLAGS       	= -DSUPERH -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -Wall # -DSUPERH is needed by sfpthread for arch-dependent work
OPTFLAGS	= -gstabs3 -O0

OBJS	=\
	api.o\
	f77api.o\
	conf.o\
	hc2hc.o\
	vrank-geq1-rdft2.o\
	ct.o\
	openmp.o\
	dft-vrank-geq1.o\
	rdft-vrank-geq1.o\
	threads.o\


$(LIB)-$(TARGET).a:     $(OBJS)
	$(AR) r $(LIB)-$(TARGET).a $(OBJS)

%.o:	%.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) $(OPTFLAGS) -c $<

clean:
	$(RM) *.o
