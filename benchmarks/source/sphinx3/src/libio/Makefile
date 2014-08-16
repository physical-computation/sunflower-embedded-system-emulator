#
# Makefile
# 
# HISTORY
# 
# 23-Dec-95	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
# 		Created.
#


S3ROOT = ../..
include $(S3ROOT)/Makefile.defines
VPATH = .:..

TARGET = libio.a

OBJS =	bio.o

TREEROOT	= ../../../../..
CC		= $(TREEROOT)/tools/bin/sh-coff-gcc
AR		= $(TREEROOT)/tools/bin/sh-coff-ar
RANLIB		= $(TREEROOT)/tools/bin/sh-coff-ranlib
CFLAGS		= -I.. $(S3DEBUG) ${$(MACHINE)_CFLAGS}

$(TARGET): $(OBJS)
	$(AR) crv $@ $?
	$(RANLIB) $@

%.o: %.c
	$(CC) $(CFLAGS) -c -g $(@:%.o=%.c)


install: $(TARGET)
	mv $(TARGET) $(LIBDIR)

clean:
	rm -f *.o *.a *.BAK *.CKP .*.BAK .*.CKP *~ .*~ #*#
