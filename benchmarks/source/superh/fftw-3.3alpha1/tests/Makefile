#
#	Makefile for fftw-3.3alpha1/tests on Sunflowersim
#
TREEROOT	= ../../../../..

include $(TREEROOT)/conf/setup.conf

PROGRAM		= fftw-bench
INCLUDEDIR      = $(TREEROOT)/sys/include
INCLUDES        = -I$(INCLUDEDIR) -I. -I../ -I../api -I../dft -I../rdft -I../libbench2 -I../kernel -I../../port -I$(TREEROOT)/sys/libOS/port -I$(TREEROOT)/sys/kern/superH -I$(TREEROOT)/sim
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -Wall
LDFLAGS		= -Ttext $(LOADADDR) -TsuperH.ld -L$(TREEROOT)/tools/tools-lib/superH -L$(TREEROOT)/sys/libOS/eOS -L$(TREEROOT)/benchmarks/source/libsfpthread -L../ -Map $(PROGRAM).map
PORT		= ../../port
OPTFLAGS	= -gstabs3 -O0
LOADADDR	= 0x08004000

#
#	print.o (which in turn needs misc.o for sccputc()) is needed only for lprint in libsfpthread
#
OBJS	=\
	init.o\
	devrtc.o\
	print.o\
	misc.o\
	sunflowersim.o\
	bench.o\
	fftw-bench.o\
	hook.o\

all: $(PROGRAM) $(PROGRAM).sr

$(PROGRAM):$(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -lfftw_threads-$(TARGET) -lfftw-$(TARGET) -lfftwbench2-$(TARGET) -o $(PROGRAM) -lsfpthread -lm -lc -lgcc

$(PROGRAM).sr:$(PROGRAM)
	$(OBJCOPY) -O srec $(PROGRAM) $@

%.o:	%.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) $(OPTFLAGS) -c $<

devrtc.o: $(PORT)/devrtc.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devrtc.c

misc.o: $(PORT)/misc.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/misc.c

print.o: $(TREEROOT)/sys/libOS/port/print.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(TREEROOT)/sys/libOS/port/print.c

init.o: init.S
	$(CPP) init.S > init.i; $(AS) init.i -o $@

clean:
	$(RM) init.i *.o $(PROGRAM) $(PROGRAM).map $(PROGRAM).sr
