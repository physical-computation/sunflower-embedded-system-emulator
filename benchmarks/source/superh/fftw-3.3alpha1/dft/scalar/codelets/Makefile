#
#	Makefile for fftw-3.3alpha1/dft/scalar/codelets on Sunflowersim
#
TREEROOT	= ../../../../../../..

include $(TREEROOT)/conf/setup.conf

INCLUDEDIR      = $(TREEROOT)/sys/include
INCLUDES        = -I$(INCLUDEDIR) -I../ -I../../ -I../../../ -I../../../kernel -I../../../../port -I$(TREEROOT)/sys/libOS/port -I$(TREEROOT)/sys/kern/superH -I$(TREEROOT)/sim
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -Wall
OPTFLAGS	= -gstabs3 -O0


OBJS	=\
	codlist.o\
	n1_10.o\
	n1_11.o\
	n1_12.o\
	n1_13.o\
	n1_14.o\
	n1_15.o\
	n1_16.o\
	n1_2.o\
	n1_20.o\
	n1_25.o\
	n1_3.o\
	n1_32.o\
	n1_4.o\
	n1_5.o\
	n1_6.o\
	n1_64.o\
	n1_7.o\
	n1_8.o\
	n1_9.o\
	q1_2.o\
	q1_3.o\
	q1_4.o\
	q1_5.o\
	q1_6.o\
	q1_8.o\
	t1_10.o\
	t1_12.o\
	t1_15.o\
	t1_16.o\
	t1_2.o\
	t1_20.o\
	t1_25.o\
	t1_3.o\
	t1_32.o\
	t1_4.o\
	t1_5.o\
	t1_6.o\
	t1_64.o\
	t1_7.o\
	t1_8.o\
	t1_9.o\
	t2_10.o\
	t2_16.o\
	t2_20.o\
	t2_25.o\
	t2_32.o\
	t2_4.o\
	t2_5.o\
	t2_64.o\
	t2_8.o\

all:	$(OBJS)

%.o:	%.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) $(OPTFLAGS) -c $<

clean:
	$(RM) *.o
