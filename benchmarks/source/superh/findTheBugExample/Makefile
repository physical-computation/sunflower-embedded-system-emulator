TREEROOT	= ../../../..

include $(TREEROOT)/conf/setup.conf

TARGET		= superH
TARGET-ARCH	= sh-elf

PROGRAM		= findTheBugExample
LIBOS		= mOS

PORT		= ../port
SIMDIR		= $(TREEROOT)/sim
TOOLSLIB	= $(TREEROOT)/tools/tools-lib
INCLUDES	= -I$(MACPATH) -I$(PORT)  -I$(TREEROOT)/sys/include -I$(TREEROOT)/sys/kern/superH -I$(SIMDIR) -I$(TREEROOT)/sys/libOS/port
OPTFLAGS	= -gstabs3 -O0
CFLAGS		= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -Wall -DLOGMARKERS ##-Werror -Winline
LDFLAGS		= -Ttext $(LOADADDR) -TsuperH.ld -L$(TOOLSLIB)/$(TARGET) -L$(TREEROOT)/sys/libOS/$(LIBOS) -Map $(PROGRAM).map
LOADADDR	= 0x08004000

OBJS	=\
	init.o\
	misc.o\
	devrtc.o\
	$(PROGRAM).o\


all:	$(PROGRAM) $(PROGRAM).sr

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -l$(LIBOS)-$(TARGET) -o $@ -lc -lgcc
	$(OBJDUMP) -G $@ > $@.stabs

$(PROGRAM).sr:$(PROGRAM)
	$(OBJCOPY) -O srec $(PROGRAM) $@

$(PROGRAM).o: $(PROGRAM).c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PROGRAM).c

init.o: init.S
	$(CPP) init.S > init.i; $(AS) init.i -o $@

devrtc.o: $(PORT)/devrtc.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devrtc.c

misc.o: $(PORT)/misc.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/misc.c

clean:
	$(RM) init.i *.o $(PROGRAM) $(PROGRAM).sr $(PROGRAM).map $(PROGRAM).stabs

install: $(PROGRAM).sr
	cp $(PROGRAM).sr ../../../

nuke:
	rm -f ../../../$(PROGRAM).sr
