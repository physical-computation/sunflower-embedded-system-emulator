# By default, the code is compiled for a "big endian" machine.
# To compile on a "little endian" machine set the LITTLE_ENDIAN flag.
# To make smaller object code, but run a little slower, don't use UNROLL_LOOPS.
# To use NIST's modified SHA of 7/11/94, define USE_MODIFIED_SHA

TREEROOT        = ../../../../..
include $(TREEROOT)/conf/setup.conf

PROGRAM		= sha

LIBOS		= eOS
SIMDIR		= $(TREEROOT)/sim
TOOLSLIB      	= $(TREEROOT)/tools/tools-lib
INCLUDES        = -I$(TREEROOT)/sys/include -I$(TREEROOT)/sys/kern/superH -I$(SIMDIR)
OPTFLAGS	= -gstabs3 -O0
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -Wall ##-Werror -Winline
LDFLAGS       	= -Ttext $(LOADADDR) -TsuperH.ld -L$(TOOLSLIB)/$(TARGET) -L$(TREEROOT)/sys/libOS/$(LIBOS) -Map $(PROGRAM).map
LOADADDR	= 0x08004000


OBJS	=\
	init.o\
	sha_driver.o\
	sha.o\

all:	$(PROGRAM).sr

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ -lc -lm -lgcc -l$(LIBOS)-$(TARGET)

$(PROGRAM).sr:$(PROGRAM)
	$(OBJCOPY) -O srec $(PROGRAM) $@
	$(OBJDUMP) -G $(PROGRAM) > $(PROGRAM).stabs

init.o: init.S
	$(CPP) init.S > init.i; $(AS) init.i -o $@

sha_driver.o: sha_driver.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c sha_driver.c

sha.o: sha.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c sha.c

clean:
	$(RM) init.i *.o $(PROGRAM) $(PROGRAM).sr $(PROGRAM).map out* *.stabs

nuke:	clean
	$(RM) simlog.node* sunflower.out
