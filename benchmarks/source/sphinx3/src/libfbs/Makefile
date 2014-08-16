#
# Makefile
# 
# HISTORY
# 
# 23-Dec-95	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
# 		Created.
#

S3ROOT		= ../..
include $(S3ROOT)/Makefile.defines


TREEROOT	= ../../../../..
TOOLSLIB      	= $(TREEROOT)/tools/tools-lib/superH
LOADADDR	= 0x08004000


VPATH = .:..


TARGET = s3decode

OBJS =	test.o\
	misc.o\
	agc.o \
	cmn.o \
	gauden.o \
	senone.o \
	interp.o \
	tmat.o \
	logs3.o \
	vector.o \
	dict.o \
	mdef.o \
	lm.o \
	fillpen.o \
	mllr.o \
	fwd.o \
	main.o

CC	= $(TREEROOT)/tools/bin/sh-coff-gcc
LD	= $(TREEROOT)/tools/bin/sh-coff-ld
CFLAGS = -I.. -I../../../port -I$(TREEROOT)/sim -I$(TREEROOT)/sys/libOS/port -I$(TREEROOT)/sys/kern/superH $(S3DEBUG) $(S2HMMTOPO) ${$(MACHINE)_CFLAGS}

$(TARGET): $(OBJS)
	$(LD) -Ttext $(LOADADDR) -TsuperH.ld -Map s3decode.map $(CFLAGS) -L$(TOOLSLIB) -L$(LIBDIR) -o $(TARGET) $(OBJS) -lc -lm -lgcc -L../libfeat -lfeat -L../libio -lio -L../libutil -lutil -lc -lm -lgcc
	$(TREEROOT)/tools/bin/sh-coff-objcopy -O srec s3decode s3decode.sr

install: $(TARGET)
	- mv $(BINDIR)/$(TARGET) $(BINDIR)/$(TARGET).old
	cp $(TARGET) $(BINDIR)
	rm -f $(BINDIR)/$(TARGET).old

logs3: logs3.c
	$(CC) -g -D_LOGS3_TEST_=1 $(CFLAGS) -L$(LIBDIR) -o logs3 logs3.c -lutil -lm

clean:
	rm -f *.o *.a *.BAK *.CKP .*.BAK .*.CKP *~ .*~ #*#
