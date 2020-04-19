TREEROOT        = ../../../../..
include $(TREEROOT)/conf/setup.conf

PROGRAM		= rijndael

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
	aes.o\
	aesxam.o\

all:	$(PROGRAM).sr

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ -lc -lm -lgcc -l$(LIBOS)-$(TARGET)

$(PROGRAM).sr:$(PROGRAM)
	$(OBJCOPY) -O srec $(PROGRAM) $@
	$(OBJDUMP) -G $(PROGRAM) > $(PROGRAM).stabs

init.o: init.S
	$(CPP) init.S > init.i; $(AS) init.i -o $@

aes.o: aes.c aes.h aestab.h aesxam.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c aes.c

aesxam.o: aes.c aes.h aestab.h aesxam.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c aesxam.c

clean:
	$(RM) init.i *.o $(PROGRAM) $(PROGRAM).sr $(PROGRAM).map out* *.stabs

nuke:	clean
	$(RM) simlog.node* sunflower.out simlog.node0
