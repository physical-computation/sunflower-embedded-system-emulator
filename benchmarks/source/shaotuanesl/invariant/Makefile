TREEROOT        = ../../../../..
include $(TREEROOT)/conf/setup.conf

PROGRAM		= fft

LIBOS		= eOS
SIMDIR		= $(TREEROOT)/sim
TOOLSLIB      	= $(TREEROOT)/tools/tools-lib
INCLUDES        = -I$(TREEROOT)/sys/include -I$(TREEROOT)/sys/kern/superH -I$(SIMDIR) -I$(TREEROOT)/benchmarks/source/port
OPTFLAGS	= -gstabs3 -O0
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -Wall ##-Werror -Winline
LDFLAGS       	= -Ttext $(LOADADDR) -TsuperH.ld -L$(TOOLSLIB)/$(TARGET) -L$(TREEROOT)/sys/libOS/$(LIBOS) -Map $(PROGRAM).map
LOADADDR	= 0x08004000


OBJS	=\
	init.o\
	fftmisc.o\
	fourierf.o\
	main.o\

all:	$(PROGRAM).sr

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ -lc -lm -lgcc -l$(LIBOS)-$(TARGET)

$(PROGRAM).sr:$(PROGRAM)
	$(OBJCOPY) -O srec $(PROGRAM) $@
	$(OBJDUMP) -G $(PROGRAM) > $(PROGRAM).stabs

init.o: init.S
	$(CPP) init.S > init.i; $(AS) init.i -o $@

fftmisc.o: fftmisc.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c fftmisc.c

fourierf.o: fourierf.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c fourierf.c

main.o: main.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c main.c

clean:
	$(RM) init.i *.o $(PROGRAM) $(PROGRAM).sr $(PROGRAM).map output* *.stabs

nuke:	clean
	$(RM) simlog.node* sunflower.out *.stabs
