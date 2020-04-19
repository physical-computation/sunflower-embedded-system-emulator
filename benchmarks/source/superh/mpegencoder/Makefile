#makefile for mpeg2encode -partitioned version
#Koushik : 04/11/2004


# Copyright (C) 1996, MPEG Software Simulation Group. All Rights Reserved.

#
# Disclaimer of Warranty
#
# These software programs are available to the user without any license fee or
# royalty on an "as is" basis.  The MPEG Software Simulation Group disclaims
# any and all warranties, whether express, implied, or statuary, including any
# implied warranties or merchantability or of fitness for a particular
# purpose.  In no event shall the copyright-holder be liable for any
# incidental, punitive, or consequential damages of any kind whatsoever
# arising from the use of these programs.
#
# This disclaimer of warranty extends to the user of these programs and user's
# customers, employees, agents, transferees, successors, and assigns.
#
# The MPEG Software Simulation Group does not represent or warrant that the
# programs furnished hereunder are free of infringement of any third-party
# patents.
#
# Commercial implementations of MPEG-1 and MPEG-2 video, including shareware,
# are subject to royalty fees to patent holders.  Many of these patents are
# general enough such that they are unavoidable regardless of implementation
# design.
#
# Additions for compilation on Sunflower simulator by Phillip Stanley-Marbell
# 
TREEROOT        = ../../..
include $(TREEROOT)/conf/setup.conf

PROGRAM		= mpeg2encode
LIBOS		= eOS

PORT		= ../../port
INCLUDEDIR      = $(TREEROOT)/sys/include
TOOLSLIB      	= $(TREEROOT)/tools/tools-lib
INCLUDES        = -I$(INCLUDEDIR) -I../../port -I$(TREEROOT)/sys/libOS/port -I$(TREEROOT)/sys/kern/superH -I$(TREEROOT)/sim
##pip CFLAGS = -ax -pg -O2 $(USE_DISP) $(USE_SHMEM) $(INCLUDEDIR) $(TRACE) $(VERBOSE) $(VERIFY) $(WARNINGS)
CFLAGS = -O0 $(TRACE) $(VERBOSE) $(VERIFY) $(WARNINGS)
LDFLAGS         = -Ttext $(LOADADDR) -TsuperH.ld -L$(TOOLSLIB)/$(TARGET) -L$(TREEROOT)/sys/libOS/$(LIBOS) -Map $(PROGRAM).map
LOADADDR	= 0x08000000

#WARNINGS = -Wall
#VERIFY = -DVERIFY

#disable this flag if you do not want bitstream element tracing 
#this will speed up the decoder some since it does not have to test
#the trace flag at several critical inner loop locations.
TRACE = -DTRACE

#disable this flag if you do not need verbose trace, such as
#header information
VERBOSE = -DVERBOSE

OBJS	=\
	init.o\
	mpeg2enc.o\
	conform.o\
	motion.o\
	putseq.o\
	putpic.o\
	putmpg.o\
	putvlc.o\
	puthdr.o\
	putbits.o\
	predict.o\
	readpic.o\
	writepic.o\
	transfrm.o\
	fdctref.o\
	idct.o\
	quantize.o\
	ratectl.o\
	stats.o\

all:	$(PROGRAM) $(PROGRAM).sr

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ -lc -lm -lgcc -l$(LIBOS)-$(TARGET) 


$(PROGRAM).sr:$(PROGRAM)
	$(OBJCOPY) -O srec $(PROGRAM) $@

init.o: init.S
	$(CPP) init.S > init.i; $(AS) init.i -o $@

devnet.o: $(PORT)/devnet.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devnet.c

devtag.o: $(PORT)/devtag.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devtag.c

devrtc.o: $(PORT)/devrtc.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devrtc.c

devloc.o: $(PORT)/devloc.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devloc.c

devlog.o: $(PORT)/devlog.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devlog.c

devrand.o: $(PORT)/devrand.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devrand.c

devexcp.o: $(PORT)/devexcp.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/devexcp.c

misc.o: $(PORT)/misc.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PORT)/misc.c

mpeg2enc.o:	mpeg2enc.c config.h global.h mpeg2enc.h Makefile
	$(CC) $(CFLAGS) $(INCLUDES) -c mpeg2enc.c

conform.o : 	conform.c config.h global.h mpeg2enc.h 
	$(CC) $(CFLAGS) $(INCLUDES) -c conform.c

idct.o : idct.c config.h 
	$(CC) $(CFLAGS) $(INCLUDES) -c idct.c

motion.o : motion.c config.h global.h mpeg2enc.h
	$(CC) $(CFLAGS) $(INCLUDES) -c motion.c

fdctref.o : fdctref.c config.h
	$(CC) $(CFLAGS) $(INCLUDES) -c fdctref.c

predict.o : predict.c config.h global.h mpeg2enc.h
	$(CC) $(CFLAGS) $(INCLUDES) -c predict.c

putbits.o : putbits.c config.h
	$(CC) $(CFLAGS) $(INCLUDES) -c putbits.c

puthdr.o : puthdr.c config.h global.h mpeg2enc.h
	$(CC) $(CFLAGS) $(INCLUDES) -c puthdr.c

putmpg.o : putmpg.c config.h global.h mpeg2enc.h
	$(CC) $(CFLAGS) $(INCLUDES) -c putmpg.c

putpic.o : putpic.c config.h global.h mpeg2enc.h
	$(CC) $(CFLAGS) $(INCLUDES) -c putpic.c

putseq.o : putseq.c config.h global.h mpeg2enc.h 
	$(CC) $(CFLAGS) $(INCLUDES) -c putseq.c

putvlc.o : putvlc.c config.h global.h mpeg2enc.h vlc.h
	$(CC) $(CFLAGS) $(INCLUDES) -c putvlc.c

quantize.o : quantize.c config.h global.h mpeg2enc.h
	$(CC) $(CFLAGS) $(INCLUDES) -c quantize.c

ratectl.o : ratectl.c config.h global.h mpeg2enc.h 
	$(CC) $(CFLAGS) $(INCLUDES) -c ratectl.c

readpic.o : readpic.c config.h global.h mpeg2enc.h 
	$(CC) $(CFLAGS) $(INCLUDES) -c readpic.c

stats.o : stats.c config.h global.h mpeg2enc.h
	$(CC) $(CFLAGS) $(INCLUDES) -c stats.c

transfrm.o : transfrm.c config.h global.h mpeg2enc.h 
	$(CC) $(CFLAGS) $(INCLUDES) -c transfrm.c

writepic.o : writepic.c config.h global.h mpeg2enc.h
	$(CC) $(CFLAGS) $(INCLUDES) -c writepic.c




clean:
	$(RM) init.i *.o $(PROGRAM) $(PROGRAM).sr $(PROGRAM).map

install: $(PROGRAM).sr
	cp $(PROGRAM).sr ../../../../

nuke:
	rm -f ../../../../$(PROGRAM).sr






