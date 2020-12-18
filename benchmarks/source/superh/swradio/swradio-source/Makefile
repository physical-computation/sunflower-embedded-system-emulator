TREEROOT	= ../../../../..

include $(TREEROOT)/conf/setup.conf

TARGET		= superH
TARGET-ARCH	= sh-elf

PROGRAM		= swradiosource
LIBOS		= eOS

PORT		= ../../../port
INCLUDEDIR      = $(TREEROOT)/sys/include
TOOLSLIB      	= $(TREEROOT)/tools/tools-lib
INCLUDES        = -I$(INCLUDEDIR) -I../ -I../../port -I$(TREEROOT)/sys/libOS/port -I$(TREEROOT)/sys/kern/superH -I$(TREEROOT)/sim
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -O3 -Wall ##-Werror 
LDFLAGS         = -Ttext $(LOADADDR) -TsuperH.ld -L$(TOOLSLIB)/$(TARGET) -L$(TREEROOT)/sys/libOS/$(LIBOS) -Map $(PROGRAM).map
LOADADDR	= 0x08004000

OBJS	=\
	init.o\
	devnet.o\
	devrtc.o\
	devexcp.o\
	misc.o\
	superHspecific.o\
	$(PROGRAM).o\
	swradio-common.o\


all:	$(PROGRAM) $(PROGRAM).sr

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -l$(LIBOS)-$(TARGET) -o $@ -lm -lgcc

$(PROGRAM).sr:$(PROGRAM)
	$(OBJCOPY) -O srec $(PROGRAM) $@

$(PROGRAM).o: $(PROGRAM).c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PROGRAM).c

swradio-common.o: ../swradio-common.c ../swradio.h Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c ../swradio-common.c

devnet.o: $(PORT)/devnet.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devnet.c

devtag.o: $(PORT)/devtag.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devtag.c

devrtc.o: $(PORT)/devrtc.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devrtc.c

devexcp.o: $(PORT)/devexcp.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devexcp.c

devlog.o: $(PORT)/devlog.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devlog.c

misc.o: $(PORT)/misc.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/misc.c

superHspecific.o: $(PORT)/superHspecific.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/superHspecific.c

init.o: init.S
	$(CPP) init.S > init.i; $(AS) init.i -o $@

clean:
	$(RM) init.i *.o $(PROGRAM) $(PROGRAM).sr $(PROGRAM).map

install: $(PROGRAM).sr
	cp $(PROGRAM).sr ../
