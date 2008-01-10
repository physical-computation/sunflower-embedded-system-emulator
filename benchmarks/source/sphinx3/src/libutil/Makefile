#
# Makefile
# 
# HISTORY
# 
# 23-Dec-95	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
# 		Created.
#

include $(S3ROOT)/Makefile.defines
VPATH = .:..

TARGET = libutil.a

OBJS = cmd_ln.o \
	ckd_alloc.o \
	err.o \
	nextword.o \
	hash.o \
	linklist.o \
	unlimit.o \
	line2words.o \
	io.o \
	profile.o

alpha_OBJS = rpcc.o
ROOT	= /home/pip/eEK
CC	= $(ROOT)/tools/bin/sh-coff-gcc
AR	= $(ROOT)/tools/bin/sh-coff-ar
RANLIB	= $(ROOT)/tools/bin/sh-coff-ranlib
CFLAGS = -DMYRMIGKI ${$(MACHINE)_CFLAGS} ## -DMYRMIGKI is for check in lubutil/unlimit.h -- pip

$(TARGET): $(OBJS) ${$(MACHINE)_OBJS}
	$(AR) crv $@ $?
	$(RANLIB) $@

install: $(TARGET)
	mv $(TARGET) $(LIBDIR)

clean:
	rm -f *.o *.a *.BAK *.CKP .*.BAK .*.CKP *~ .*~ #*#
