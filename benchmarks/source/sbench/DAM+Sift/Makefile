TREEROOT	= ../../../..

include $(TREEROOT)/conf/setup.conf

PROGRAM		= damv2
MAC		= sift-mac
MACPATH		= ../Sift-MAC
LIBOS		= eOS

PORT		= ../../port
LIBOS		= eOS
SIMDIR		= $(TREEROOT)/sim
TOOLSLIB      	= $(TREEROOT)/tools-lib
INCLUDES        = -I$(MACPATH) -I$(PORT)  -I$(TREEROOT)/sys/include -I$(TREEROOT)/sys/kern/superH -I$(SIMDIR) -I$(TREEROOT)/sys/libOS/port
OPTFLAGS	= -gstabs3 -O0
CFLAGS       	= -DSIFT_RANDTABLE -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -Wall -DLOGMARKERS## -Werror -Winline
LDFLAGS       	= -Ttext $(LOADADDR) -TsuperH.ld -L$(TOOLSLIB)/$(TARGET) -L$(TREEROOT)/sys/libOS/$(LIBOS) -Map $(PROGRAM).map
LOADADDR	= 0x08004000

OBJS	=\
	init.o\
	devrtc.o\
	devrand.o\
	devexcp.o\
	misc.o\
	devsensor.o\
	devloc.o\
	devmac.o\
	$(MAC).o\
	$(PROGRAM).o\


all:	$(PROGRAM) $(PROGRAM).sr

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -l$(LIBOS)-$(TARGET) -o $@ -lm -lc -lgcc

$(PROGRAM).sr:$(PROGRAM)
	$(OBJCOPY) -O srec $(PROGRAM) $@

$(PROGRAM).o: $(PROGRAM).c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PROGRAM).c

$(MAC).o: $(MACPATH)/$(MAC).c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(MACPATH)/$(MAC).c

devmac.o: $(PORT)/devmac.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devmac.c

devsensor.o: $(PORT)/devsensor.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devsensor.c

devtag.o: $(PORT)/devtag.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devtag.c

devrtc.o: $(PORT)/devrtc.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devrtc.c

devrand.o: $(PORT)/devrand.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devrand.c

devexcp.o: $(PORT)/devexcp.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devexcp.c

devlog.o: $(PORT)/devlog.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devlog.c

devloc.o: $(PORT)/devloc.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devloc.c

misc.o: $(PORT)/misc.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/misc.c

init.o: init.S
	$(CPP) init.S > init.i; $(AS) init.i -o $@

clean:
	$(RM) init.i *.o $(PROGRAM) $(PROGRAM).sr $(PROGRAM).map

install: $(PROGRAM).sr
	cp $(PROGRAM).sr ../../../

nuke:
	rm -f ../../../$(PROGRAM).sr
