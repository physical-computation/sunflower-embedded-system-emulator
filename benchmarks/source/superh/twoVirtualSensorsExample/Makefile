TREEROOT	= ../../../..
include $(TREEROOT)/conf/setup.conf

TARGET		= superH
TARGET-ARCH	= sh-elf

PROGRAM		= virtualSensorExample

PORT		= ../../port
LIBOS		= mOS
TOOLSLIB	= $(TREEROOT)/tools/tools-lib
INCLUDES	= -I../port/  -I$(TREEROOT)/sys/include -I$(TREEROOT)/sys/kern/superH -I$(TREEROOT)/benchmarks/include -I$(TREEROOT)/sys/libOS/port
OPTFLAGS	= -gstabs3 -O0
CFLAGS		= $(TARGET-ARCH-FLAGS) -nostdlib -fno-builtin -Wall # Do not do since we are linking mOS for libc implementation:	-nostdlib -fno-builtin 
LDFLAGS		= -Ttext $(LOADADDR) -TsuperH.ld -L$(TOOLSLIB)/$(TARGET) -L$(TREEROOT)/sys/libOS/$(LIBOS) -Map $(PROGRAM).map
LOADADDR	= 0x08004000


OBJS	=\
	init-$(TARGET).o\
	devrtc.o\
	misc.o\
	devsensor.o\
	$(PROGRAM).o\


all:	$(PROGRAM) $(PROGRAM).sr

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ -lm -l$(LIBOS)-$(TARGET) -lc -lgcc

$(PROGRAM).sr:$(PROGRAM)
	$(OBJCOPY) -O srec $(PROGRAM) $@

$(PROGRAM).o: $(PROGRAM).c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PROGRAM).c

devsensor.o: $(PORT)/devsensor.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devsensor.c

devrtc.o: $(PORT)/devrtc.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devrtc.c

misc.o: $(PORT)/misc.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/misc.c

init-$(TARGET).o: init-$(TARGET).S
	$(CPP) init-$(TARGET).S > init-$(TARGET).i; $(AS) init-$(TARGET).i -o $@

clean:
	$(RM) init-$(TARGET).i *.o $(PROGRAM) $(PROGRAM).sr $(PROGRAM).map

install: all
	cp $(PROGRAM).sr ../../../
