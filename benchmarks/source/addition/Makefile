TREEROOT        = ../../..
include $(TREEROOT)/conf/setup.conf

PROGRAM		= bsort
LIBOS		= eOS

PORT		= ../port
LIBOS		= eOS
TOOLSLIB      	= $(TREEROOT)/tools/tools-lib
INCLUDES        = -I../port/  -I$(TREEROOT)/sys/include -I$(TREEROOT)/sys/kern/superH -I$(TREEROOT)/benchmarks/include -I$(TREEROOT)/sys/libOS/port
OPTFLAGS	= -gstabs3 -O0
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -Wall ##-Werror -Winline
LDFLAGS       	= -Ttext $(LOADADDR) -TsuperH.ld -L$(TOOLSLIB)/$(TARGET) -L$(TREEROOT)/sys/libOS/$(LIBOS) -Map $(PROGRAM1).map
LOADADDR	= 0x08004000
DATA2ARRAY	= ../../utils/data2array


OBJS	=\
	init.o\
	misc.o\
	devrtc.o\
	$(PROGRAM).o\


all:	$(PROGRAM) $(PROGRAM).sr

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -l$(LIBOS)-$(TARGET) -o $@ -lgcc

$(PROGRAM).sr:$(PROGRAM)
	$(OBJCOPY) -O srec $(PROGRAM) $@

$(PROGRAM).o: $(PROGRAM).c $(PROGRAM)-input.h Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PROGRAM).c

misc.o: ../port/misc.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c ../port/misc.c

devrtc.o: ../port/devrtc.c Makefile
	$(CC) $(CFLAGS) $(WFLAGS) $(INCLUDES) -c ../port/devrtc.c

init.o: init.S
	$(CPP) init.S > init.i; $(AS) init.i -o $@

#$(PROGRAM)-input.h:
#	$(DATA2ARRAY) $(INPUT) bsort_input > $(PROGRAM)-input.h

clean:
	$(RM) init.i *.o $(PROGRAM) $(PROGRAM).sr $(PROGRAM).map # $(PROGRAM)-input.h

install: all
	cp $(PROGRAM).sr ../../../
