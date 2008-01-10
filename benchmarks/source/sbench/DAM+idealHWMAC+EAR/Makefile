TREEROOT	= ../../../..

include $(TREEROOT)/conf/setup.conf

PROGRAM		= damv2
SINK		= sink
ROUTE		= ear-route
ROUTEPATH	= ../EAR-route
LIBOS		= eOS

PORT		= ../../port
LIBOS		= eOS
SIMDIR		= $(TREEROOT)/sim
TOOLSLIB      	= $(TREEROOT)/tools-lib
INCLUDES        = -I$(ROUTEPATH) -I$(PORT)  -I$(TREEROOT)/sys/include -I$(TREEROOT)/sys/kern/superH -I$(SIMDIR) -I$(TREEROOT)/sys/libOS/port
OPTFLAGS	= -gstabs3 -O0
CFLAGS       	= -DEAR_ORACLE_LOCATIONS=1 -DEAR_ORACLE_NEIGHBORS=1 -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -Wall -DLOGMARKERS##-Werror -Winline
PLDFLAGS       	= -Ttext $(LOADADDR) -TsuperH.ld -L$(TOOLSLIB)/$(TARGET) -L$(TREEROOT)/sys/libOS/$(LIBOS) -Map $(PROGRAM).map
SLDFLAGS       	= -Ttext $(LOADADDR) -TsuperH.ld -L$(TOOLSLIB)/$(TARGET) -L$(TREEROOT)/sys/libOS/$(LIBOS) -Map $(SINK).map

LOADADDR	= 0x08004000

NODEOBJS	=\
	init.o\
	devrtc.o\
	devrand.o\
	devexcp.o\
	misc.o\
	devsensor.o\
	devloc.o\
	devnet.o\
	devmac.o\
	$(ROUTE).o\
	$(PROGRAM).o\

SINKOBJS	=\
	init.o\
	devrtc.o\
	devrand.o\
	devexcp.o\
	misc.o\
	devsensor.o\
	devloc.o\
	devnet.o\
	devmac.o\
	$(ROUTE).o\
	$(SINK).o\


all:	$(PROGRAM) $(PROGRAM).sr $(SINK) $(SINK).sr

$(PROGRAM): $(NODEOBJS)
	$(LD) $(PLDFLAGS) $(NODEOBJS) -l$(LIBOS)-$(TARGET) -o $@ -lm -lc -lgcc

$(SINK): $(SINKOBJS)
	$(LD) $(SLDFLAGS) $(SINKOBJS) -l$(LIBOS)-$(TARGET) -o $@ -lm -lc -lgcc

$(PROGRAM).sr:$(PROGRAM)
	$(OBJCOPY) -O srec $(PROGRAM) $@

$(SINK).sr:$(SINK)
	$(OBJCOPY) -O srec $(SINK) $@

$(PROGRAM).o: $(PROGRAM).c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PROGRAM).c

$(SINK).o: $(SINK).c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(SINK).c

$(ROUTE).o: $(ROUTEPATH)/$(ROUTE).c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(ROUTEPATH)/$(ROUTE).c

devnet.o: $(PORT)/devnet.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devnet.c

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
	$(RM) init.i *.o $(PROGRAM) $(PROGRAM).sr $(PROGRAM).map $(SINK) $(SINK).sr $(SINK).map

install: $(PROGRAM).sr
	cp $(PROGRAM).sr ../../../

nuke:
	rm -f ../../../$(PROGRAM).sr
