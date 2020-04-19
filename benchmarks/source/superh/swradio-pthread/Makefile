TREEROOT	= ../../../..

include $(TREEROOT)/conf/setup.conf

PROGRAM		= swradio
LIBOS		= eOS
PORT		= ../port

PTHREAD		= ../libsfpthread
SIMDIR		= $(TREEROOT)/sim
TOOLSLIB      	= $(TREEROOT)/tools/tools-lib
INCLUDES        = -I$(TREEROOT)/sys/include -I$(TREEROOT)/sys/libOS/port -I$(TREEROOT)/sys/kern/superH -I$(SIMDIR) -I$(PTHREAD) -I$(PORT)
OPTFLAGS	= -O3 -funroll-loops -finline-functions -fomit-frame-pointer
CFLAGS       	= -DSUPERH -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) #-Wall -Werror -Winline
LDFLAGS         = -Ttext $(LOADADDR) -TsuperH.ld -L$(TOOLSLIB)/$(TARGET) -L$(PTHREAD) -L$(TREEROOT)/sys/libOS/$(LIBOS) -Map $(PROGRAM).map
LOADADDR	= 0x08004000

OBJS	=\
	init.o\
	devrtc.o\
	devlog.o\
	misc.o\
	$(PROGRAM).o\
	swradio-common.o\



all:	$(PROGRAM) $(PROGRAM).sr

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -l$(LIBOS)-$(TARGET) -o $@ -lsfpthread -lc -lm -lgcc

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

devlog.o: $(PORT)/devlog.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devlog.c

swradio-common.o: swradio-common.c swradio.h Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c swradio-common.c



clean:
	$(RM) init.i *.o $(PROGRAM) $(PROGRAM).sr $(PROGRAM).map

install: $(PROGRAM).sr
	cp $(PROGRAM).sr ../

nuke:	clean
	$(RM) simlog.node* sunflower.out
