# Makefile for mpeg2decode

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

PROGRAM		= mpeg2decode
LIBOS		= mOS

PORT		= ../port
INCLUDEDIR      = $(TREEROOT)/sys/include
TOOLSLIB      	= $(TREEROOT)/tools-lib
INCLUDES        = -I$(INCLUDEDIR) -I../port -I$(TREEROOT)/sys/libOS/port -I$(TREEROOT)/sys/kern/superH -I$(TREEROOT)/sim
##pip CFLAGS = -ax -pg -O2 $(USE_DISP) $(USE_SHMEM) $(INCLUDEDIR) $(TRACE) $(VERBOSE) $(VERIFY) $(WARNINGS)
CFLAGS = -O2 $(TRACE) $(VERBOSE) $(VERIFY) $(WARNINGS)
LDFLAGS         = -Ttext $(LOADADDR) -TsuperH.ld -L$(TOOLSLIB)/$(TARGET) -L$(TREEROOT)/sys/libOS/$(LIBOS) -Map $(PROGRAM).map
LOADADDR	= 0x08001000

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
	mpeg2dec.o\
	getpic.o\
	motion.o\
	getvlc.o\
	gethdr.o\
	getblk.o\
	getbits.o\
	store.o\
	recon.o\
	spatscal.o\
	idct.o\
	idctref.o\
	display.o\
	systems.o\
	subspic.o\
	verify.o\

all:	$(PROGRAM) $(PROGRAM).sr

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ -lc -lgcc -lm -l$(LIBOS)-$(TARGET)


$(PROGRAM).sr:$(PROGRAM)
	$(OBJCOPY) -O srec $(PROGRAM) $@

init.o: init.S
	$(CPP) init.S > init.i; $(AS) init.i -o $@

mpeg2dec.o: mpeg2dec.c config.h global.h mpeg2dec.h Makefile
	$(CC) $(CFLAGS) $(INCLUDES) -c mpeg2dec.c

display.o:	display.c config.h global.h mpeg2dec.h
	$(CC) $(CFLAGS) $(INCLUDES) -c display.c

getbits.o:	getbits.c config.h global.h mpeg2dec.h
	$(CC) $(CFLAGS) $(INCLUDES) -c getbits.c

getblk.o:	getblk.c config.h global.h mpeg2dec.h
	$(CC) $(CFLAGS) $(INCLUDES) -c getblk.c

gethdr.o:	gethdr.c config.h global.h mpeg2dec.h
	$(CC) $(CFLAGS) $(INCLUDES) -c gethdr.c

getpic.o:	getpic.c config.h global.h mpeg2dec.h
	$(CC) $(CFLAGS) $(INCLUDES) -c getpic.c

getvlc.o:	getvlc.c config.h global.h mpeg2dec.h getvlc.h
	$(CC) $(CFLAGS) $(INCLUDES) -c getvlc.c

idct.o:		idct.c config.h
	$(CC) $(CFLAGS) $(INCLUDES) -c idct.c

idctref.o:	idctref.c config.h
	$(CC) $(CFLAGS) $(INCLUDES) -c idctref.c

motion.o:	motion.c config.h global.h mpeg2dec.h
	$(CC) $(CFLAGS) $(INCLUDES) -c motion.c

recon.o:	recon.c config.h global.h mpeg2dec.h
	$(CC) $(CFLAGS) $(INCLUDES) -c recon.c

spatscal.o:	spatscal.c config.h global.h mpeg2dec.h
	$(CC) $(CFLAGS) $(INCLUDES) -c spatscal.c

store.o:	store.c config.h global.h mpeg2dec.h
	$(CC) $(CFLAGS) $(INCLUDES) -c store.c

systems.o:	systems.c config.h global.h mpeg2dec.h
	$(CC) $(CFLAGS) $(INCLUDES) -c systems.c

subspic.o:	subspic.c config.h global.h mpeg2dec.h
	$(CC) $(CFLAGS) $(INCLUDES) -c subspic.c

verify.o:	verify.c config.h global.h mpeg2dec.h
	$(CC) $(CFLAGS) $(INCLUDES) -c verify.c




clean:
	$(RM) init.i *.o $(PROGRAM) $(PROGRAM).sr $(PROGRAM).map

install: $(PROGRAM).sr
	cp $(PROGRAM).sr ../../../

nuke:
	rm -f ../../../$(PROGRAM).sr

