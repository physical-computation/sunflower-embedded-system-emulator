TREEROOT	= ../../..
include $(TREEROOT)/conf/setup.conf

PROGRAM		= 1

OPTFLAGS	= -g -O0
CFLAGS		= $(TARGET-ARCH-FLAGS) -Wall # Do not do since we are linking mOS for libc implementation:	-nostdlib -fno-builtin 
LDFLAGS		= -Ttext $(LOADADDR)
LOADADDR	= 0x08004000


OBJS	=\
	$(PROGRAM).o\


all:	$(PROGRAM) $(PROGRAM).sr Makefile

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ -lm -lc

$(PROGRAM).sr:$(PROGRAM)
	$(OBJCOPY) -O srec $(PROGRAM) $@

$(PROGRAM).o: $(PROGRAM).c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) -c $(PROGRAM).c

clean:
	$(RM) init.i *.o $(PROGRAM) $(PROGRAM).sr $(PROGRAM).map
