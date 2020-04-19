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

TARGET = libfeat.a

OBJS = feat.o feat_s2_4x.o feat_s3_1x39.o feat_1s_c.o feat_1s_c_d.o feat_1s_c_dd.o feat_1s_c_d_dd.o feat_1s_c_wd_dd.o feat_1s_c_d_cd.o feat_1s_c_d_ld_dd.o

TREEROOT	= ../../../../../..
CC		= $(TREEROOT)/tools/bin/sh-elf-gcc
AR		= $(TREEROOT)/tools/bin/sh-elf-ar
RANLIB 		= $(TREEROOT)/tools/bin/sh-elf-ranlib
CFLAGS 		= -I.. ${$(MACHINE)_CFLAGS}

$(TARGET): $(OBJS)
	$(AR) crv $@ $?
	$(RANLIB) $@

install: $(TARGET)
	mv $(TARGET) $(LIBDIR)

clean:
	rm -f *.o *.a *.BAK *.CKP .*.BAK .*.CKP *~ .*~ #*#

testfeat: testfeat.c
	cc $(CFLAGS) -L. -L../libutil -o testfeat testfeat.c -lfeat -lutil
