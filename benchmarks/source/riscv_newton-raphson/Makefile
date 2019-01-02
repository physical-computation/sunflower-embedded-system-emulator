TREEROOT	= ../../..
include $(TREEROOT)/conf/setup.conf

PROGRAM		= 1
INIT			= init

OPTFLAGS	= -g -O0
CFLAGS		= $(TARGET-ARCH-FLAGS) -Wall # Do not do since we are linking mOS for libc implementation:	-nostdlib -fno-builtin 
LDFLAGS		= -Ttext $(LOADADDR)
LOADADDR	= 0x08004000


OBJS	=\
	$(PROGRAM).o\


all:	$(PROGRAM) $(PROGRAM).sr Makefile

$(INIT).o: $(INIT).s
	$(CC) -O0 -c -o $(INIT).o $(INIT).s

$(PROGRAM): $(OBJS) $(INIT).o
	$(LD) $(LDFLAGS) $(INIT).o $(OBJS) -o $@ -lm -lc -Map=1.map

$(PROGRAM).sr:$(PROGRAM)
	$(OBJCOPY) -O srec $(PROGRAM) $@

$(PROGRAM).o: $(PROGRAM).c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) -c $(PROGRAM).c

clean:
	$(RM) init.i *.o $(PROGRAM) $(PROGRAM).sr $(PROGRAM).map
