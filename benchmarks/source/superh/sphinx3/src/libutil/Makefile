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
TREEROOT	= ../../../../..
CC		= $(TREEROOT)/tools/bin/sh-elf-gcc
AR		= $(TREEROOT)/tools/bin/sh-elf-ar
RANLIB		= $(TREEROOT)/tools/bin/sh-elf-ranlib
CFLAGS		= -DSFLRSIM ${$(MACHINE)_CFLAGS} ## -DSFLRSIM is for check in lubutil/unlimit.h -- pip

$(TARGET): $(OBJS) ${$(MACHINE)_OBJS}
	$(AR) crv $@ $?
	$(RANLIB) $@

install: $(TARGET)
	mv $(TARGET) $(LIBDIR)

clean:
	rm -f *.o *.a *.BAK *.CKP .*.BAK .*.CKP *~ .*~ #*#
