TREEROOT	= ../../..

include $(TREEROOT)/conf/setup.conf

PROGRAM		= sieve
LIBOS		= eOS
PORT		= ../port

PTHREAD		= ../libsfpthread
SIMDIR		= $(TREEROOT)/sim
TOOLSLIB      	= $(TREEROOT)/tools/tools-lib
INCLUDES        = -I$(TREEROOT)/sys/include -I$(TREEROOT)/sys/kern/superH -I$(SIMDIR) -I$(PTHREAD)
OPTFLAGS	= -O3 -funroll-loops -finline-functions -fomit-frame-pointer
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) $(OPTFLAGS) -Wall ##-Werror -Winline
LDFLAGS         = -Ttext $(LOADADDR) -TsuperH.ld -L$(TOOLSLIB)/$(TARGET) -L$(PTHREAD) -L$(TREEROOT)/sys/libOS/$(LIBOS) -Map $(PROGRAM).map
LOADADDR	= 0x08004000

OBJS	=\
	init.o\
	devrtc.o\
	misc.o\
	$(PROGRAM).o\



all:	$(PROGRAM) $(PROGRAM).sr

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ -lsfpthread -lc -lm -lgcc -l$(LIBOS)-$(TARGET)

$(PROGRAM).sr:$(PROGRAM)
	$(OBJCOPY) -O srec $(PROGRAM) $@

$(PROGRAM).o: $(PROGRAM).c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PROGRAM).c

init.o: init.S
	$(CPP) init.S > init.i; $(AS) init.i -o $@

misc.o: $(PORT)/misc.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/misc.c

devrtc.o: $(PORT)/devrtc.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devrtc.c



clean:
	$(RM) init.i *.o $(PROGRAM) $(PROGRAM).sr $(PROGRAM).map

install: $(PROGRAM).sr
	cp $(PROGRAM).sr ../

nuke:	clean
	$(RM) simlog.node* sunflower.out
