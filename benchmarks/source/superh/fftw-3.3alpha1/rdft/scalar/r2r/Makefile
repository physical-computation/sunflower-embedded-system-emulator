#
#	Makefile for fftw-3.3alpha1/rdft/scalar/r2r on Sunflowersim
#
TREEROOT	= ../../../../../../..

include $(TREEROOT)/conf/setup.conf

TARGET		= superH
TARGET-ARCH	= sh-elf

INCLUDEDIR      = $(TREEROOT)/sys/include
INCLUDES        = -I$(INCLUDEDIR) -I../ -I../../ -I../../../ -I../../../kernel -I../../../../port -I$(TREEROOT)/sys/libOS/port -I$(TREEROOT)/sys/kern/superH -I$(TREEROOT)/sim
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -Wall
OPTFLAGS	= -gstabs3 -O0

OBJS	=\
	codlist.o\
	e01_8.o\
	e10_8.o\

all:	$(OBJS)

%.o:	%.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) $(OPTFLAGS) -c $<

clean:
	$(RM) *.o
