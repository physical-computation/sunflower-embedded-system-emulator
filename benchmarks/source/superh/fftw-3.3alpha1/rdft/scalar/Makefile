#
#	Makefile for fftw-3.3alpha1/rdft/scalar on Sunflowersim
#
TREEROOT	= ../../../../../..

include $(TREEROOT)/conf/setup.conf

TARGET		= superH
TARGET-ARCH	= sh-elf

INCLUDEDIR      = $(TREEROOT)/sys/include
INCLUDES        = -I$(INCLUDEDIR) -I../ -I../../ -I../../kernel -I../../../port -I$(TREEROOT)/sys/libOS/port -I$(TREEROOT)/sys/kern/superH -I$(TREEROOT)/sim
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -Wall
OPTFLAGS	= -gstabs3 -O0

OBJS	=\
	hc2c.o\
	hfb.o\
	r2c.o\
	r2r.o\

all:	$(OBJS)

%.o:	%.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) $(OPTFLAGS) -c $<

clean:
	$(RM) *.o
