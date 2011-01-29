#
#	Makefile for fftw-3.3alpha1/reodft on Sunflowersim
#
TREEROOT	= ../../../..

include $(TREEROOT)/conf/setup.conf

INCLUDEDIR      = $(TREEROOT)/sys/include
INCLUDES        = -I$(INCLUDEDIR) -I../ -I../rdft/ -I../kernel -I../../port -I$(TREEROOT)/sys/libOS/port -I$(TREEROOT)/sys/kern/superH -I$(TREEROOT)/sim
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -Wall
OPTFLAGS	= -gstabs3 -O0

OBJS	=\
	conf.o\
	redft00e-r2hc-pad.o\
	redft00e-r2hc.o\
	reodft00e-splitradix.o\
	reodft010e-r2hc.o\
	reodft11e-r2hc-odd.o\
	reodft11e-r2hc.o\
	reodft11e-radix2.o\
	rodft00e-r2hc-pad.o\
	rodft00e-r2hc.o\

all:	$(OBJS)

%.o:	%.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) $(OPTFLAGS) -c $<

clean:
	$(RM) *.o
